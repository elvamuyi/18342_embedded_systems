/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
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
#include <arm/physmem.h>
#include <arm/exception.h>

int task_create(task_t* tasks __attribute__((unused)), size_t num_tasks __attribute__((unused)))
{
    size_t i, j;
    task_t* temp; 
    task_t* ordered_tasks[num_tasks];
    for (i = 0; i < num_tasks; i++)
        ordered_tasks[i] = &tasks[i];

    // TODO: Filter non-schedulable user tasks

    // Sort tasks in ascending order
    for (i = 0; i < num_tasks; i++) {
        for (j = i + 1; j < num_tasks; j++) {
            if ((*ordered_tasks[i]).T > (*ordered_tasks[j]).T) {
                temp = ordered_tasks[i];
                ordered_tasks[i] = ordered_tasks[j];
                ordered_tasks[j] = temp;
            }
        }
    }

    allocate_tasks(ordered_tasks, num_tasks);
    return 1;
}

int event_wait(uint32_t dev __attribute__((unused)))
{
    dev_wait(dev);
    return 1;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(uint32_t call_num __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
