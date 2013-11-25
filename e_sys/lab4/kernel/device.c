/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <types.h>
#include <assert.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	size_t next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const size_t dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
    int i;
    for (i = 0; i < NUM_DEVICES; i++) {
        devices[i].sleep_queue = NULL;
        devices[i].next_match = dev_freq[i];
    }
}

/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned dev __attribute__((unused)))
{
	// Remove from run queue
    tcb_t* task_to_sleep = runqueue_remove(get_cur_prio());

    // Add to sleep queue
    tcb_t* top = devices[dev].sleep_queue;
    devices[dev].sleep_queue = task_to_sleep;
    task_to_sleep->sleep_queue = top;

    // Context switch
    dispatch_sleep();
}

/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(size_t millis __attribute__((unused)))
{
	// Find out current device & update next_match
    int cur_dev;
    tcb_t *sq_ptr1, *sq_ptr2;
    for (cur_dev = 0; cur_dev < NUM_DEVICES; cur_dev++) {
        if (devices[cur_dev].next_match != millis)
            continue;

        // Update the device timer
        devices[cur_dev].next_match += dev_freq[cur_dev];

        // Remove from sleep_queue & add to run_queue
        sq_ptr1 = devices[cur_dev].sleep_queue;
        while (sq_ptr1 != NULL) {
            // Get next sleep task
            sq_ptr2 = sq_ptr1;
            sq_ptr1 = sq_ptr1->sleep_queue;
            // Remove from sleep_queue
            sq_ptr2->sleep_queue = NULL;
            // Add to the run_queue
            runqueue_add(sq_ptr2, sq_ptr2->cur_prio);
        }
        devices[cur_dev].sleep_queue = NULL;
    }
}
