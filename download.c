#include "ao.h"


void start_download(environ_t *env) {
	dl_prepare(env);
	dl_start(env);
	//clean up
	if (env->has_log) Unlink(env->logfile);
}



void dl_start(environ_t *env) {
	int i, status, nfds;
	int fd_count = env->partition + 2; // timer_fd and signal_fd
	struct epoll_event ev[fd_count], timer_ev, signal_ev;
	task_t *task;

	env->file_fd = Open(env->filename, O_WRONLY | O_CREAT, 0644);

	// register to epoll
	for (i = 0; i < env->partition; i++) {
		task = &env->tasks[i];
		Epoll_ctl(env->epoll_fd, EPOLL_CTL_ADD, task->socket_fd, &task->event);
	}
	timer_ev.data.fd = env->timer_fd;
	timer_ev.events = EPOLLIN;
	Epoll_ctl(env->epoll_fd, EPOLL_CTL_ADD, env->timer_fd, &timer_ev);
	if (env->support_range) {
		Sigprocmask(SIG_BLOCK, &env->sigset);
		signal_ev.data.fd = env->signal_fd;
		signal_ev.events = EPOLLIN;
		Epoll_ctl(env->epoll_fd, EPOLL_CTL_ADD, env->signal_fd, &signal_ev);
	}

	Gettimeofday(&env->t1);
	set_timer(env->timer_fd, 200);

	while (fd_count > 2) { // timer_fd && signal_fd remain
		nfds = Epoll_wait(env->epoll_fd, ev, fd_count, -1);
		for (i = 0; i < nfds; i++) {
			if (ev[i].data.fd == env->timer_fd) { // timer
				Gettimeofday(&env->t2);
				output_progress_bar(env);
				set_timer(env->timer_fd, 200);
			} else if (ev[i].data.fd == env->signal_fd) {
				dl_save_status(env);
				exit(EXIT_SUCCESS);
			} else if ((ev[i].events & EPOLLERR)
					|| (ev[i].events & EPOLLHUP)) { // error
				fprintf(stderr, "[ao] epollerr or epollhup\n");
				exit(EXIT_FAILURE);
			} else if (ev[i].events & EPOLLOUT) { // send
				task = get_task_by_fd(env, ev[i].data.fd);
				status = task->todo(task);
				if (status == 0) // request is sent
					Epoll_ctl(env->epoll_fd, EPOLL_CTL_MOD,
							task->socket_fd, &task->event);
			} else if (ev[i].events & EPOLLIN) { // read
				task = get_task_by_fd(env, ev[i].data.fd);
				status = task->todo(task);
				if (status == 0) { // finished
					switch (task->flag) {
						case FLAG_RESPONSE_STOP: // return by recv_response
							string2response(task->response);
							switch (task->response->status[0]) {
								case '2': // 2xx
									break;
								case '3': // 3xx
									task_prepare_redirection(task);
									Epoll_ctl(env->epoll_fd, EPOLL_CTL_MOD,
											task->socket_fd, &task->event);
									break;
								default: // 1xx, 4xx, 5xx
									fprintf(stderr, "[ao] http error: '%s'\n",
											task->response->status);
									exit(EXIT_FAILURE);
							}
							break;
						case FLAG_DOWNLOAD_FINISHED: // return by save_data
							Epoll_ctl(env->epoll_fd, EPOLL_CTL_DEL,
									task->socket_fd, NULL);
							fd_count--;
							break;
					}
				} else if (status == 2) { // save_data get some data
					Pwrite(env->file_fd, task->response->data,
							task->size, task->offset);
					task->offset += task->size;
				}
			}
		}
	}
}




void dl_prepare(environ_t *env) {
	task_t *task = new_task(0, 1);
	// start
	dl_get_response(env->epoll_fd, task);
	// get filename
	if (env->filename[0] == '\0')
		get_filename_from_path(env->filename, task->url->path);
	// whether file existed or log file existed
	dl_check_file(env);
	if (env->has_log) {
		// support range
		del_task(task);
		dl_get_info_from_log(env);
	} else {
		env->tasks = task;
		dl_get_info_from_task(env);
	}
}



void dl_get_response(int epfd, task_t *task) {
	int status;
	struct epoll_event ev;
	Epoll_ctl(epfd, EPOLL_CTL_ADD, task->socket_fd, &task->event);

	while (1) {
		Epoll_wait(epfd, &ev, 1, -1);
		if ((ev.events & EPOLLERR) || (ev.events & EPOLLHUP)) {
			fprintf(stderr, "[ao] epoll error\n");
			exit(EXIT_FAILURE);
		} else if (ev.events & EPOLLOUT) {
			// wait connect && send request
			status = task->todo(task);
			if (status == 0)
				Epoll_ctl(epfd, EPOLL_CTL_MOD, task->socket_fd, &task->event);
		} else if (ev.events & EPOLLIN) {
			// recv response
			status = task->todo(task);
			if (status == 0) {
				string2response(task->response);
				switch (task->response->status[0]) {
					case '2': // 2xx
						Epoll_ctl(epfd, EPOLL_CTL_DEL,
								task->socket_fd, NULL);
						return;
					case '3': // 3xx
						task_prepare_redirection(task);
						Epoll_ctl(epfd, EPOLL_CTL_MOD,
								task->socket_fd, &task->event);
						break;
					default: // 1xx, 4xx, 5xx
						fprintf(stderr, "[ao] http error: '%s'\n",
								task->response->status);
						exit(EXIT_FAILURE);
				}
			}
		}
	}
}



// if file existed, check log file
// if not log file, auto rename
void dl_check_file(environ_t *env) {
	int fix = 0;
	char *file_format = "%s.%d";
	char *log_format = "%s.log";
	char filename[SHORT_STR];
	static_copy(filename, SHORT_STR, env->filename, strlen(env->filename));

	while (1) {
		// get log file's name
		snprintf(env->logfile, SHORT_STR, log_format, filename);
		if (access(filename, F_OK) == -1) {
			// file not found
			env->has_log = false;
			static_copy(env->filename, SHORT_STR, filename, strlen(filename));
			printf("[ao] save to '%s'.\n", env->filename);
			return;
		} else if (access(env->logfile, F_OK) == 0) {
			// file exist and log file found
			env->has_log = true;
			static_copy(env->filename, SHORT_STR, filename, strlen(filename));
			printf("[ao] save to '%s'.\n", env->filename);
			return;
		} else {
			// test next name
			snprintf(filename, SHORT_STR, file_format, env->filename, fix++);
		}
	}
}



void dl_get_info_from_log(environ_t *env) {
	FILE *fp = Fopen(env->logfile, "rb");
	// update environ
	Close(env->epoll_fd);
	Close(env->timer_fd);
	Close(env->signal_fd);
	fread(env, sizeof(environ_t), 1, fp);
	environ_update_by_log(env);
	// create tasks
	env->tasks = Malloc(env->partition * sizeof(task_t));
	// update tasks
	for (int i = 0; i < env->partition; i++) {
		fread(&env->tasks[i], sizeof(task_t), 1, fp);
		task_update_by_log(&env->tasks[i]);
	}
	Fclose(fp);
}



void dl_get_info_from_task(environ_t *env) {
	char *val;
	header_field_t *hfptr = env->tasks->response->hf;
	val = get_header(hfptr, "Content-Range");
	if (val == NULL) {
		env->support_range = false;
		env->partition = 1;
		printf("[ao] remote server not support multi-download.\n");
		val = get_header(hfptr, "Content-Length");
		if (val == NULL) {
			env->filesize = 0;
			printf("[ao] filesize not found.\n");
		} else {
			env->filesize = atol(val);
			printf("[ao] filesize: %ld.\n", env->filesize);
		}
	} else {
		// range
		env->support_range = true;
		// filesize
		char *pos = strchr(val, '/');
		env->filesize = atol(++pos);
		printf("[ao] filesize: %ld.\n", env->filesize);
		// initial tasks
		del_task(env->tasks);
		env->tasks = Malloc(env->partition * sizeof(task_t));
		off_t block_size = env->filesize / env->partition, start = 0, stop = 0;
		int i;
		for (i = 0; i < env->partition - 1; i++) {
			start = stop;
			stop += block_size;
			//initial_task(&env->tasks[i], start, stop - 1);
			initial_task(&env->tasks[i], start, stop);
		}
		initial_task(&env->tasks[i], stop, env->filesize);
	}
}




void dl_save_status(environ_t *env) {
	FILE *log = Fopen(env->logfile, "wb");
	env->has_log = true;
	fwrite(env, sizeof(environ_t), 1, log);
	fwrite(env->tasks, sizeof(task_t), env->partition, log);
	Fclose(log);
}
