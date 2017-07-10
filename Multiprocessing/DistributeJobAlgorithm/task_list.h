typedef struct {
	int creating_processor;
	int start_time;
	int required_CPU_time;
} task_t;



typedef struct task_list_t {
	task_t* task;
	struct task_list_t* next;
} task_list_t;

task_list_t* tl_new(task_t* task, task_list_t* next);

task_list_t* tl_add(task_list_t* tl, task_t* task);

task_list_t* tl_pop(task_list_t* tl);

task_t* tl_get_task(task_list_t* tl);

void tl_free(task_list_t* tl);