#include "ao.h"


// get filename and filesize
// test whether support range
void dl_prepare(env_t *env) {
	char *val; //response header value
	task_t *task = init_task(env, true, 0, 1);

	conn_url(task);

	if (env->filename[0] == '\0') {
		// not specified filename
		get_filename_by_path(env, task->url->path);
		// rename if file is existed
		check_filename(env);
	}
	printf("[ao] filename: %s\n", env->filename);

	val = get_header(task->response->hf, "Content-Range");
	if (val == NULL) {
		// not support range
		env->support_range = false;
		printf("[ao] ! not support multithread download.\n");
		val = get_header(task->response->hf, "Content-Length");
		if (val == NULL) {
			env->filesize = 0;
			printf("[ao] ! filesize not found.\n");
		} else {
			env->filesize = atol(val);
			printf("[ao] filesize: %ld\n", env->filesize);
		}
	} else {
		env->support_range = true;
		get_filesize_by_range(env, val);
		printf("[ao] filesize: %ld\n", env->filesize);
	}

	free_task(task);
}



void dl_start(env_t *env) {
	printf("[ao] start download.\n");
	env->fd = open(env->filename, O_WRONLY | O_CREAT, 0644);

	if (env->support_range) {
		dl_multi_thread(env);
	} else {
		dl_single_thread(env);
	}

	close(env->fd);
	printf("\n[ao] download finished.\n");
}




void dl_single_thread(env_t *env) {
	task_t *task = init_task(env, false);
	conn_url(task);

	char buff[RECV_LEN];
	ssize_t received;
	while (1) {
		received = _recv(task->sockfd, buff, RECV_LEN);
		if (received == 0) break;
		write(env->fd, buff, received);
		print_progress_bar(env);
	}

	free_task(task);
}



void dl_multi_thread(env_t *env) {
	// create threads
	pthread_t tids[env->task_num];

	task_t *task;
	int i;
	off_t block_size = env->filesize / env->task_num;
	off_t start = 0, stop = 0;
	for (i = 0; i < env->task_num - 1; i++) {
		start = stop;
		stop += block_size;
		task = init_task(env, true, start, stop - 1);
		pthread_create(&tids[i], NULL, _dl_thread_routine, task);
	}
	task = init_task(env, true, stop, env->filesize);
	pthread_create(&tids[i], NULL, _dl_thread_routine, task);

	for (i = 0; i < env->task_num; i++) {
		pthread_join(tids[i], NULL);
	}
}



void *_dl_thread_routine(void *arg) {
	task_t *task = (task_t *)arg;
	conn_url(task);

	char buff[RECV_LEN];
	ssize_t received;
	while (1) {
		received = _recv(task->sockfd, buff, RECV_LEN);
		if (received == 0) break;
		pwrite(task->env->fd, buff, received, task->range_start);
		task->range_start += received;
		if (pthread_mutex_trylock(&task->env->mutex) == 0) {
			print_progress_bar(task->env);
			pthread_mutex_unlock(&task->env->mutex);
		}
	}

	free_task(task);
	pthread_exit(NULL);
}



void check_filename(env_t *env) {
	if (access(env->filename, F_OK) == 0) {
		// file existed
		int fix = 0;
		char *format = "%s.%d";
		char new_name[SHORT_STR];

		while (fix < 100) {
			snprintf(new_name, SHORT_STR, format, env->filename, fix++);
			if (access(new_name, F_OK) != 0) {
				static_copy(env->filename, SHORT_STR,
						new_name, strlen(new_name));
				return;
			}
		}

		printf("please specify a filename\n");
		exit(EXIT_FAILURE);
	}
}
