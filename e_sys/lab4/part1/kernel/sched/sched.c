/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
 */

#include <types.h>

#include <config.h>
#include "sched_i.h"

#include <arm/psr.h>
#include <arm/exception.h>

tcb_t system_tcb[OS_MAX_TASKS]; /* Allocate memory for system TCBs */

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
static void __attribute__((unused)) idle(void)
{
    enable_interrupts();
    while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks __attribute__((unused)), size_t num_tasks __attribute__((unused)))
{
    runqueue_init();

    // Reserve priority 0
    uint8_t prio;
    size_t i_task;
    for (i_task = 0, prio = 1; i_task < num_tasks; i_task++, prio++) {
        system_tcb[prio].native_prio = prio;
        system_tcb[prio].cur_prio = prio;
        system_tcb[prio].context.r4 = (uint32_t) ((*tasks)[i_task].lambda);
        system_tcb[prio].context.r5 = (uint32_t) ((*tasks)[i_task].data);
        system_tcb[prio].context.r6 = (uint32_t) ((*tasks)[i_task].stack_pos);
        system_tcb[prio].context.lr = launch_task;
        system_tcb[prio].context.sp = system_tcb[prio].kstack_high;
        system_tcb[prio].holds_lock = 0;
        system_tcb[prio].sleep_queue = NULL;
        runqueue_add(&system_tcb[prio], prio);
    }

    sched_init(NULL);
}

void sched_init(task_t* main_task __attribute__((unused)))
{
    // Idle task initialization
    system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].cur_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].context.r4 = (uint32_t) idle;
    system_tcb[IDLE_PRIO].context.r5 = 0x0;
    system_tcb[IDLE_PRIO].context.r6 = USR_END_ADDR;
    system_tcb[IDLE_PRIO].context.lr = launch_task;
    system_tcb[IDLE_PRIO].context.sp = system_tcb[IDLE_PRIO].kstack_high;
    system_tcb[IDLE_PRIO].holds_lock = 0;
    system_tcb[IDLE_PRIO].sleep_queue = NULL;
    runqueue_add(&system_tcb[IDLE_PRIO], IDLE_PRIO);

    dispatch_init(&system_tcb[IDLE_PRIO]);
}
