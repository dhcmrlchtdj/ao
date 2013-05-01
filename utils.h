//dest_size contains '\0'
//src_size not contains '\0'
char *dynamic_copy(char *src, size_t src_size);
void static_copy(char *dest, size_t dest_size, char *src, size_t src_size);

// return ms
// t2 must greater than t1
long delta_time(struct timeval *t1, struct timeval *t2);
