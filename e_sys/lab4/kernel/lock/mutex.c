/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 * 
 * @date  
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];
static int mutexExist = 0;    // Number of existing mutex

void mutex_init(void)
{
    int i;
    mutexExist = 0;
    for (i = 0; i < OS_NUM_MUTEX; i++) {
        gtMutex[i].bAvailable = 1;
        gtMutex[i].pHolding_Tcb = NULL;
        gtMutex[i].bLock = 0;
        gtMutex[i].pSleep_queue = NULL;
    }
}

int mutex_create(void)
{
    if (mutexExist < OS_NUM_MUTEX) {
        gtMutex[mutexExist].bAvailable = 0;
        gtMutex[mutexExist].pHolding_Tcb = NULL;
        gtMutex[mutexExist].bLock = 0;
        gtMutex[mutexExist].pSleep_queue = NULL;
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
            || (gtMutex[mutex].bAvailable == 0))
        return -EINVAL;

    // Check deadlock
    if (gtMutex[mutex].pHolding_Tcb == cur_tcb)
        return -EDEADLOCK;

    // Lock
    if (gtMutex[mutex].bLock == 0) {
        cur_tcb->holds_lock = mutex;
        gtMutex[mutex].bLock = 1;
        gtMutex[mutex].pHolding_Tcb = cur_tcb;
        return 0;
    } else {
        // Add to sleep_queue
        if (gtMutex[mutex].pSleep_queue == NULL)
            gtMutex[mutex].pSleep_queue = cur_tcb;
        else {
            tcb_t* sqIndex = gtMutex[mutex].pSleep_queue;
            while (sqIndex->sleep_queue != NULL)
                sqIndex = sqIndex->sleep_queue;
            sqIndex->sleep_queue = cur_tcb;
        }

        enable_interrupts();
        dispatch_sleep();
        return 0;
    }
}

int mutex_unlock(int mutex __attribute__((unused)))
{
    // Check mutex out of range || use unavailable mutex
    if (!(mutex > 0 && mutex < OS_NUM_MUTEX) 
            || (gtMutex[mutex].bAvailable == 0))
        return -EINVAL;

    // Unlock
    if (gtMutex[mutex].pSleep_queue == NULL) {
        gtMutex[mutex].bLock = 0;
        gtMutex[mutex].pHolding_Tcb = NULL;
    } else {
        gtMutex[mutex].bLock = 1;
        gtMutex[mutex].pHolding_Tcb = gtMutex[mutex].pSleep_queue;
        gtMutex[mutex].pSleep_queue = gtMutex[mutex].pHolding_Tcb->sleep_queue;

        // Add to run_queue
        runqueue_add(gtMutex[mutex].pHolding_Tcb, gtMutex[mutex].pHolding_Tcb->cur_prio);
        gtMutex[mutex].pHolding_Tcb->sleep_queue = NULL;
    }

    return 1;
}
