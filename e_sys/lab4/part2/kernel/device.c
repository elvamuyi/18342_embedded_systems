/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
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
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
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
 * event is signaled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned dev __attribute__((unused)))
{
	// Remove from run queue
    tcb_t* task_to_sleep = get_cur_tcb();

    // Add to sleep queue
    task_to_sleep->sleep_queue = devices[dev].sleep_queue;
    devices[dev].sleep_queue = task_to_sleep;

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
    int cur_dev;
    tcb_t *sq_ptr1, *sq_ptr2;
    bool_e update_flag = FALSE;
    for (cur_dev = 0; cur_dev < NUM_DEVICES; cur_dev++)
    {
        // Not meet the time yet
        if (devices[cur_dev].next_match > millis)
            continue;

        // Update the device timer
        devices[cur_dev].next_match = millis + dev_freq[cur_dev];

        // Remove from sleep_queue & add to run_queue
        sq_ptr1 = devices[cur_dev].sleep_queue;
        if (sq_ptr1 != NULL) {
            update_flag = TRUE;
            do {
                // Get next sleep task
                sq_ptr2 = sq_ptr1;
                sq_ptr1 = sq_ptr1->sleep_queue;
                // Remove from sleep_queue
                sq_ptr2->sleep_queue = NULL;
                // Add to the run_queue
                runqueue_add(sq_ptr2, sq_ptr2->cur_prio);
            } while(sq_ptr1 != NULL);
            devices[cur_dev].sleep_queue = NULL;
        }
    }
    // If any task is activated, call dispatch_save
    if (update_flag) dispatch_save();
}
