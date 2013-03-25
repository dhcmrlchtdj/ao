void dl_prepare(env_t *env);
void dl_start(env_t *env);

void dl_single_thread(env_t *env);
void dl_multi_thread(env_t *env);
void *_dl_thread_routine(void *arg);

void check_filename(env_t *env);
