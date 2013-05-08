void create_connection(task_t *task);

// return 0, goto next step
// return 1, working
// return 2, something need to do
int wait_connect(task_t *task);
int send_request(task_t *task);
int recv_response(task_t *task);
int save_data(task_t *task);
