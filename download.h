void dl_prepare(void);
void dl_start(void);

void dl_single_thread(void);
void dl_multi_thread(void);
void *_dl_thread_routine(void *arg);

void _dl_check_file(void);

void _handle_exit(int no);

void _dl_save_log(void);
void _dl_read_log(void);

void _dl_get_info(task_t *task);
