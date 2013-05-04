#include "ao.h"



char *dynamic_copy(char *src, size_t src_size) {
	char *dest = malloc(sizeof(char) * (src_size + 1));
	assert(dest != NULL);
	memcpy(dest, src, src_size);
	dest[src_size] = '\0';
	return dest;
}




void static_copy(char *dest, size_t dest_size, char *src, size_t src_size) {
	if (dest_size > src_size) {
		memcpy(dest, src, src_size);
		dest[src_size] = '\0';
	} else {
		memcpy(dest, src, dest_size - 1);
		dest[dest_size] = '\0';
	}
}




long delta_time(struct timeval *t1, struct timeval *t2) {
	long sec = t2->tv_sec - t1->tv_sec;
	long usec = t2->tv_usec - t1->tv_usec;
	return ((sec * 1000000 + usec) / 1000);
}



void environ_init(environ_t* env) {
	env->partition = 0;
	env->filename[0] = '\0';
}



void environ_update(environ_t* env) {
	env->epfd = epoll_create1(0);
	assert(env->epfd > 0);

	if (env->partition == 0)
		env->partition = 6;
}



void p(environ_t *e) {
	printf("epfd: %d\n", e->epfd);
	printf("partition: %d\n", e->partition);
	printf("filename: '%s'\n", e->filename);
}
