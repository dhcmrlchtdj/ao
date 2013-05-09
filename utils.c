#include "ao.h"


char *dynamic_copy(char *src, size_t src_size) {
	char *dest = Malloc(sizeof(char) * (src_size + 1));
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


///////////////////////////////////////////////////////////////////////////////


void set_timer(int timer_fd, long millisecond) {
	// {0, 0} means alarm once
	// {0, n} means after n nanoseconds
	struct itimerspec interval = { {0, 0}, {0, millisecond * 1000000} };
	timerfd_settime(timer_fd, 0, &interval, NULL);
}



// return millisecond
long delta_time(struct timeval *t1, struct timeval *t2) {
	long sec = t2->tv_sec - t1->tv_sec;
	long usec = t2->tv_usec - t1->tv_usec;
	return (sec * 1000000 + usec) / 1000;
}


///////////////////////////////////////////////////////////////////////////////


void get_filename_from_path(char *filename, char *path) {
	char *start, *stop;
	start = strrchr(path, '/');
	if (start == NULL || *++start == '\0') {
		// use "default" if not filename
		static_copy(filename, SHORT_STR, "default", strlen("default"));
	} else {
		// FIXME
		stop = strchr(start, '?');
		if (stop) {
			// str between '/' and '?'
			static_copy(filename, SHORT_STR, start, stop - start);
		} else {
			static_copy(filename, SHORT_STR, start, strlen(start));
		}
	}
}
