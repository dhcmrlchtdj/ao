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
		dest[dest_size] = '\0';
	}
}



void set_alarm(int timer_fd, long nsec) {
	// {0, 0} means alarm once
	// {0, nsec} means after nsec nanoseconds
	struct itimerspec interval = { {0, 0}, {0, nsec} };
	timerfd_settime(timer_fd, 0, &interval, NULL);
}



long delta_time(struct timeval *t1, struct timeval *t2) {
	long sec = t2->tv_sec - t1->tv_sec;
	long usec = t2->tv_usec - t1->tv_usec;
	return ((sec * 1000000 + usec) / 1000);
}



/*

void get_filename_by_path(char *path) {
	char *start, *stop;
	start = strrchr(path, '/');
	if (start == NULL || *++start == '\0') {
		// use "default" if not filename
		static_copy(env.file_name, SHORT_STR, "default", 7);
	} else {
		// FIXME
		stop = strchr(start, '?');
		if (stop) {
			// str between '/' and '?'
			static_copy(env.file_name, SHORT_STR, start, stop - start);
		} else {
			static_copy(env.file_name, SHORT_STR, start, strlen(start));
		}
	}
}



void get_filesize_by_range(char *range) {
	char *pos = strchr(range, '/');
	env.file_size = atol(++pos); // move to filesize
}

*/
