void start_download(environ_t *env);

void dl_start(environ_t *env);
void dl_prepare(environ_t *env);
void dl_get_response(int epfd, task_t *task);
void dl_check_file(environ_t *env);

void dl_get_info_from_log(environ_t *env);
void dl_get_info_from_task(environ_t *env);
