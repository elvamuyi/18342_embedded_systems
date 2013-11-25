/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
    cur_tcb = idle;
    dispatch_nosave();
}

/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task. The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
    // Remove current task from run_queue
    uint8_t cur_prio = get_cur_prio();
    tcb_t* task_to_switch = runqueue_remove(cur_prio);

    // Find next runnable task & add current task back to run_queue
    uint8_t next_task_prio = highest_prio();
    runqueue_add(task_to_switch, cur_prio);

    // Context switch
    cur_tcb = &system_tcb[next_task_prio];
    ctx_switch_full(&(cur_tcb->context), &(task_to_switch->context));
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
    // Find next runnable task
    uint8_t next_task_prio = highest_prio();
    cur_tcb = &system_tcb[next_task_prio];

    ctx_switch_half(&(cur_tcb->context));
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	tcb_t* task_to_switch = get_cur_tcb();

    // Find next runnable task
    uint8_t next_task_prio = highest_prio();
    cur_tcb = &system_tcb[next_task_prio];

    ctx_switch_full(&(cur_tcb->context), &(task_to_switch->context));
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
    return cur_tcb->cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
    return cur_tcb;
}
