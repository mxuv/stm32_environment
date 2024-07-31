#include <stdint.h>

#include "os.h"
#include "os_config.h"
#include "mcu.h"

static taskptr_t task_queue[N_TASKS];

static uint8_t q_index_first;
static uint8_t q_index_last;
static uint8_t os_state;

static struct
{
  taskptr_t calling_task;
  uint16_t time;
} os_timers[N_TIMERS];

void os_init(void)
{
  q_index_first = 0;
  q_index_last = 0;
  os_state = TASK_QUEUE_EMPTY;
  for (uint8_t i = 0; i < N_TIMERS; i++)
  {
    os_timers[i].calling_task = 0;
    os_timers[i].time = 0;
  }
}

void os_set_task(taskptr_t Task)
{
  DISABLE_INTERRUPTS();
  if (os_state == TASK_QUEUE_FULL)
  {
    ENABLE_INTERRUPTS();
    return;
  }
  os_state = 0;
  task_queue[q_index_first] = Task;
  q_index_first++;
  if (q_index_first == (N_TASKS))
    q_index_first = 0;
  if (q_index_first == q_index_last)
    os_state = TASK_QUEUE_FULL;
  ENABLE_INTERRUPTS();
}

void os_set_timer_task(taskptr_t Task, uint16_t Time)
{
  uint8_t n = 0;

  DISABLE_INTERRUPTS();
  for (uint8_t i = 0; i < N_TIMERS; i++)
  {
    if (os_timers[i].calling_task == Task)
    {
      os_timers[i].time = Time;
      ENABLE_INTERRUPTS();
      return;
    }
    if (os_timers[i].calling_task == 0)
      n = i;
  }
  os_timers[n].calling_task = Task;
  os_timers[n].time = Time;
  ENABLE_INTERRUPTS();
}

void os_clear_timer_task(taskptr_t Task)
{
  DISABLE_INTERRUPTS();
  for (uint8_t i = 0; i < N_TIMERS; i++)
  {
    if (os_timers[i].calling_task == Task)
    {
      os_timers[i].calling_task = 0;
      os_timers[i].time = 0;
    }
  }
  ENABLE_INTERRUPTS();
}

void os_task_manager(void)
{
  taskptr_t CallTask;

  DISABLE_INTERRUPTS();
  if (os_state == TASK_QUEUE_EMPTY)
  {
    ENABLE_INTERRUPTS();
    return;
  }
  os_state = 0;
  CallTask = task_queue[q_index_last];
  q_index_last++;
  if (q_index_last == (N_TASKS))
    q_index_last = 0;
  if (q_index_last == q_index_first)
    os_state = TASK_QUEUE_EMPTY;
  ENABLE_INTERRUPTS();
  CallTask();
}

void os_timer_service(void)
{
  for (uint8_t i = 0; i < N_TIMERS; i++)
  {
    if (os_timers[i].calling_task != 0)
    {
      os_timers[i].time--;
      if (os_timers[i].time == 0)
      {
        os_set_task(os_timers[i].calling_task);
        os_timers[i].calling_task = 0;
      }
    }
  }
}
