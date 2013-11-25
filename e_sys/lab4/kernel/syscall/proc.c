/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
 */

#include <sched.h>
#include <config.h>
#include <device.h>
#include <kernel.h>
#include <exports.h>
#include <syscall.h>

#include <bits/errno.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

int task_create(task_t* tasks __attribute__((unused)), size_t num_tasks __attribute__((unused)))
{
    size_t i, j;
    task_t temp;

    // Verify tasks
    if (num_tasks >= OS_AVAIL_TASKS)
        return -EINVAL;
    else if (!valid_addr(tasks, sizeof(task_t) * num_tasks,
                    USR_START_ADDR, USR_END_ADDR))
            return -EFAULT;
    for (i = 0; i < num_tasks; i++)
        if (tasks[i].C > tasks[i].T)
            return -ESCHED;

    // Sort tasks in ascending order
    for (i = 0; i < num_tasks; i++) {
        for (j = i + 1; j < num_tasks; j++) {
            if (tasks[i].T > tasks[j].T) {
                temp = tasks[i];
                tasks[i] = tasks[j];
                tasks[j] = temp;
            }
        }
    }

    allocate_tasks(&tasks, num_tasks);
    return 0;
}

int event_wait(uint32_t dev __attribute__((unused)))
{
    if (dev >= NUM_DEVICES)
        return -EINVAL;

    dev_wait(dev);
    return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(uint32_t call_num __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
