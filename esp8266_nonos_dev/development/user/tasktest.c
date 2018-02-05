/*
 * tacktest.c
 *
 *  Created on: Jan 27, 2018
 *      Author: nolex
 *       This tests investigates how can be used tasks with priorization. As we can see, there is a background sobroutine which saves context of
 *       the previous task (function)
 */


#include "osapi.h"
#include "user_interface.h"

#define MY_TASK_PRIORITY 0
#define MY_QUEUE_SIZE    1
static os_event_t g_my_queue[MY_QUEUE_SIZE];
static os_event_t g_my_queue2[MY_QUEUE_SIZE];
volatile uint32_t g_value = 0;
static os_timer_t task_start_timer;

os_timer_t blink_timer3;

// this is the timer callback function
void run_task() {
   system_os_post(MY_TASK_PRIORITY, 0, 0);
   os_printf("init loller\n\r");
}

void task1()
{
	uint8_t x = 10;
	while(x--)
	{
		os_printf("x: %d\n\r", x);
		if(x == 6) //immediately start another task with higher priorization
			system_os_post(2, 0, 0);
	}
}

void task2()
{
	os_printf("hoppp!\n\r");
}


void my_task(os_event_t *ev) {
    ++g_value;
    // start the task again in 5 sec
    os_printf("loller\n\r");
    os_timer_arm(&task_start_timer, 5000 /*ms*/, 0 /*once*/);
}



void task_demonstrate()
{
	//preparing task
	system_os_task(task1,1, g_my_queue, MY_QUEUE_SIZE);
	//again
	system_os_task(task2,2, g_my_queue2, MY_QUEUE_SIZE);
	//starting task
	system_os_post(1, 0, 0);
	os_timer_setfn(&blink_timer3, (os_timer_func_t *)task1, (void *)0);
	os_timer_arm(&blink_timer3, 200, 1);
}
