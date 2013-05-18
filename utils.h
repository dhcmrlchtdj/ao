//dest_size contains '\0'
//src_size not contains '\0'
void static_copy(char *dest, size_t dest_size, char *src, size_t src_size);
char *dynamic_copy(char *src, size_t src_size);

///////////////////////////////////////////////////////////////////////////////

void set_timer(int timer_fd, long millisecond);
long time_diff(struct timeval *t1, struct timeval *t2);

///////////////////////////////////////////////////////////////////////////////

void get_filename_from_path(char *filename, char *path);
