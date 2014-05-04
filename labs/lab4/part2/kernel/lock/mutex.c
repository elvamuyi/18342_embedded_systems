/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
 */

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

mutex_t gtMutex[OS_NUM_MUTEX];
static int mutexExist = 0;    // Number of existing mutex

void mutex_init(void)
{
    int i;
    mutexExist = 0;
    for (i = 0; i < OS_NUM_MUTEX; i++) {
        gtMutex[i].bAvailable = FALSE;
        gtMutex[i].pHolding_Tcb = NULL;
        gtMutex[i].bLock = FALSE;
        gtMutex[i].pSleep_queue = NULL;
    }
}

int mutex_create(void)
{
    if (mutexExist < OS_NUM_MUTEX) {
        gtMutex[mutexExist].bAvailable = TRUE;
        mutexExist++;
        return mutexExist - 1;
    }
    else return -ENOMEM;
}

int mutex_lock(int mutex __attribute__((unused)))
{
    tcb_t* cur_tcb = get_cur_tcb();

    // Check if mutex out of range || use unavailable mutex
    if (!(mutex > 0 && mutex < OS_NUM_MUTEX) 
            || (gtMutex[mutex].bAvailable == FALSE))
        return -EINVAL;

    // Check deadlock
    else if (gtMutex[mutex].pHolding_Tcb == cur_tcb)
        return -EDEADLOCK;

    // Lock when mutex is free
    else if (gtMutex[mutex].bLock == FALSE) {
        cur_tcb->holds_lock++;
        cur_tcb->cur_prio = TOP_PRIO;
        gtMutex[mutex].bLock = TRUE;
        gtMutex[mutex].pHolding_Tcb = cur_tcb;
    } 
    
    // Mutex being used. Add current task to mutext sleep_queue
    else {
        tcb_t* sqIndex = gtMutex[mutex].pSleep_queue;
        if (sqIndex != NULL) {
            while (sqIndex->sleep_queue != NULL)
                sqIndex = sqIndex->sleep_queue;
            sqIndex->sleep_queue = cur_tcb;
        }
        else
            gtMutex[mutex].pSleep_queue = cur_tcb;

        // Context switch the next available task
        dispatch_sleep();
    }

    return 0;
}

int mutex_unlock(int mutex __attribute__((unused)))
{
    tcb_t* cur_tcb = get_cur_tcb();

    // Check mutex out of range || use unavailable mutex
    if (!(mutex > 0 && mutex < OS_NUM_MUTEX) 
            || (gtMutex[mutex].bAvailable == FALSE))
        return -EINVAL;

    // Check if current task holds the mutex
    else if (gtMutex[mutex].pHolding_Tcb != cur_tcb)
        return -EPERM;

    // Unlock when no waiting tasks
    else if (gtMutex[mutex].pSleep_queue == NULL) {
        gtMutex[mutex].bLock = FALSE;
        gtMutex[mutex].pHolding_Tcb = NULL;
    }
    
    // Unlock when there are waiting tasks
    else {
        // Release one task from the mutex sleep_queue
        gtMutex[mutex].bLock = TRUE;
        gtMutex[mutex].pHolding_Tcb = gtMutex[mutex].pSleep_queue;
        gtMutex[mutex].pSleep_queue = gtMutex[mutex].pHolding_Tcb->sleep_queue;
        gtMutex[mutex].pHolding_Tcb->sleep_queue = NULL;

        // Add the new released task to run_queue
        runqueue_add(gtMutex[mutex].pHolding_Tcb, gtMutex[mutex].pHolding_Tcb->cur_prio);
    }

    // Adjust the current task priority
    cur_tcb->holds_lock--;
    cur_tcb->cur_prio = cur_tcb->holds_lock ? TOP_PRIO : cur_tcb->native_prio;
    return 0;
}
