#ifndef __DTOS_H__
#define __DTOS_H__

#define TASK_QUEUE_EMPTY 0b00000001
#define TASK_QUEUE_FULL  0b00000010

typedef void (*taskptr_t)(void);

void os_set_task(taskptr_t task);
void os_init(void);
void os_task_manager(void);
void os_timer_service(void);
void os_set_timer_task(taskptr_t Task, uint16_t Time);
void os_clear_timer_task(taskptr_t Task);

#endif
