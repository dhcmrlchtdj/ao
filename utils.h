//dest_size contains '\0'
//src_size not contains '\0'
void static_copy(char *dest, size_t dest_size, char *src, size_t src_size);
char *dynamic_copy(char *src, size_t src_size);

//////////////////////////////

void set_alarm(int timer_fd, long nsec);
