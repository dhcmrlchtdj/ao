void dl_prepare(env_t *env);
void dl_start(env_t *env);

void dl_single_thread(env_t *env);
void dl_multi_thread(env_t *env);
void *_dl_thread_routine(void *arg);

void handle_alarm(int signo);
static sigjmp_buf _dl_jmpbuf;
static volatile sig_atomic_t _dl_canjmp;

void check_filename(env_t *env);
