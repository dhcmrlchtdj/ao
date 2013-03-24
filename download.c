#include "ao.h"


// get filename and filesize
// test whether support range
void dl_prepare(env_t *env) {
	task_t *task = init_task(env, true, 0, 1);

	conn_url(task);

	if (env->filename[0] == '\0')
		get_filename_by_path(env, task->url->path);
	printf("[ao] filename: %s\n", env->filename);

	char *val;
	val = get_header(task->response->hf, "Content-Range");
	if (val == NULL) {
		// not support
		env->support_range = false;
		printf("[ao] ! not support 'Range'.\n");
		val = get_header(task->response->hf, "Content-Length");
		if (val == NULL) {
			env->filesize = 0;
			printf("[ao] ! filesize not found.\n");
		} else {
			env->filesize = strtoul(val, NULL, 0);
			printf("[ao] filesize: %lu\n", env->filesize);
		}
	} else {
		env->support_range = true;
		get_filesize_by_range(env, val);
		printf("[ao] filesize: %lu\n", env->filesize);
	}

	free_task(task);
}



void dl_start(env_t *env) {
	printf("[ao] downloading...\n");
	env->fd = open(env->filename, O_WRONLY | O_CREAT, 0644);

	if (env->support_range) {
		dl_multi_thread(env);
	} else {
		dl_single_thread(env);
	}

	close(env->fd);
	printf("[ao] download finished.\n");
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
		printf("%ld ", received);
	}

	free_task(task);
}



void dl_multi_thread(env_t *env) {
	pthread_t tids[env->task_num];
	task_t *task;
	int status, i;
	unsigned long block_size = env->filesize / env->task_num;
	unsigned long start = 0, stop = 0;
	for (i = 0; i < env->task_num - 1; i++) {
		start = stop;
		stop += block_size;
		task = init_task(env, true, start, stop - 1);
		pthread_create(&tids[i], NULL, _dl_thread_routine, task);
	}
	task = init_task(env, true, stop, env->filesize);
	pthread_create(&tids[i], NULL, _dl_thread_routine, task);

	for (i = 0; i < env->task_num; i++)
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
		task->offset += received;
		pwrite(task->env->fd, buff, received, task->offset);
		printf(".");
	}

	free_task(task);
	pthread_exit(NULL);
}
