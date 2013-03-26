#include "ao.h"


// get filename and filesize
// test whether support range
void dl_prepare(void) {
	// connect to url
	task_t *task = init_task(true, 0, 1);
	conn_url(task);

	// not specified filename
	if (env.file_name[0] == '\0') get_filename_by_path(task->url->path);
	// try to get log file
	// if there is no log
	// auto rename if file existed
	_dl_check_file();
	printf("[ao] filename: %s\n", env.file_name);

	if (env.has_log) {
		_dl_read_log();
	} else {
		_dl_get_info(task);
	}

	free_task(task);
}



void dl_start(void) {
	printf("[ao] start download.\n");
	env.fd = open(env.file_name, O_WRONLY | O_CREAT, 0644);

	env.last_size = 0;
	gettimeofday(&env.t1, NULL);

	if (env.support_range) {
		dl_multi_thread();
	} else {
		dl_single_thread();
	}

	close(env.fd);
	printf("\n[ao] download finished.\n");
}




void dl_single_thread(void) {
	env.task_list = init_task(false);
	conn_url(env.task_list);

	char buff[RECV_LEN];
	ssize_t received;
	while (1) {
		received = _recv(env.task_list->sockfd, buff, RECV_LEN);
		if (received == 0) break;
		write(env.fd, buff, received);
		gettimeofday(&env.t2, NULL);
		print_progress_bar();
	}
}



void dl_multi_thread(void) {
	signal(SIGINT, _handle_exit);
	signal(SIGHUP, _handle_exit);
	signal(SIGQUIT, _handle_exit);
	signal(SIGTERM, _handle_exit);

	// create threads
	pthread_t tids[env.task_num];

	int i;
	off_t block_size = env.file_size / env.task_num;
	off_t start = 0, stop = 0;

	task_t *ptr = env.task_list;
	for (i = 0; i < env.task_num; i++) {
		pthread_create(&tids[i], NULL, _dl_thread_routine, ptr);
		ptr = ptr->next;
	}
	for (i = 0; i < env.task_num; i++)
		pthread_join(tids[i], NULL);
}



void *_dl_thread_routine(void *arg) {
	task_t *task = (task_t *)arg;
	conn_url(task);

	char buff[RECV_LEN];
	ssize_t received;
	while (1) {
		received = _recv(task->sockfd, buff, RECV_LEN);
		if (received == 0) break;
		pthread_rwlock_rdlock(&env.rwlock);
		pwrite(env.fd, buff, received, task->range_start);
		task->range_start += received;
		pthread_rwlock_unlock(&env.rwlock);

		if (pthread_mutex_trylock(&env.mutex) == 0) {
			// get mutex
			gettimeofday(&env.t2, NULL);
			print_progress_bar();
			pthread_mutex_unlock(&env.mutex);
		}
	}

	pthread_exit(NULL);
}



void _dl_check_file(void) {
	int fix = 0;
	char *log_format = "%s.log";
	char *file_format = "%s.%d";
	char new_name[SHORT_STR];
	static_copy(new_name, SHORT_STR, env.file_name, strlen(env.file_name));

	while (1) {
		// get log file's name
		snprintf(env.log_name, SHORT_STR, log_format, new_name);
		if (access(new_name, F_OK) != 0) {
			// file not found
			env.has_log = false;
			static_copy(env.file_name, SHORT_STR, new_name, strlen(new_name));
			return;
		} else if (access(env.log_name, F_OK) == 0) {
			// log file found
			env.has_log = true;
			static_copy(env.file_name, SHORT_STR, new_name, strlen(new_name));
			return;
		} else {
			snprintf(new_name, SHORT_STR, file_format, env.file_name, fix++);
		}
	}
}



void _handle_exit(int no) {
	printf("\nreceive signal: %d\n", no);
	if (env.support_range) {
		pthread_rwlock_wrlock(&env.rwlock);
		_dl_save_log();
	}
	close(env.fd);
	destroy_env(&env);
	exit(EXIT_SUCCESS);
}



void _dl_save_log(void) {
	printf("[ao] log saved.\n");
	FILE *fp = fopen(env.log_name, "wb");

	fwrite(&env, sizeof(env_t), 1, fp);
	while (env.task_list) {
		fwrite(env.task_list, sizeof(task_t), 1, fp);
		env.task_list = env.task_list->next;
	}

	fclose(fp);
}



void _dl_read_log(void) {
	FILE *fp = fopen(env.log_name, "wb");
	fread(&env, sizeof(env_t), 1, fp);
	update_log(&env);

	// create task
	env.task_list = malloc(sizeof(task_t));
	task_t **ptr = &env.task_list;

	for (int i = 0; i < env.task_num; i++) {
		fread(ptr, sizeof(task_t), 1, fp);
		(*ptr)->next = malloc(sizeof(task_t));
		ptr = &(*ptr)->next;
	}
	free(*ptr); // the last is not used

	fclose(fp);
}



void _dl_get_info(task_t *task) {
	char *val;
	val = get_header(task->response->hf, "Content-Range");
	if (val == NULL) {
		// not support range
		env.support_range = false;
		printf("[ao] ! not support multithread download.\n");
		val = get_header(task->response->hf, "Content-Length");
		if (val == NULL) {
			env.file_size = 0;
			printf("[ao] ! filesize not found.\n");
		} else {
			env.file_size = atol(val);
			printf("[ao] filesize: %ld\n", env.file_size);
		}
	} else {
		env.support_range = true;
		get_filesize_by_range(val);
		printf("[ao] filesize: %ld\n", env.file_size);

		// create task
		off_t block_size = env.file_size / env.task_num;
		off_t start = 0, stop = 0;
		int i;
		task_t **ptr = &env.task_list;
		for (i = 0; i < env.task_num - 1; i++) {
			start = stop;
			stop += block_size;
			*ptr = init_task(true, start, stop - 1);
			ptr = &(*ptr)->next;
		}
		*ptr = init_task(true, stop, env.file_size);
	}
}
