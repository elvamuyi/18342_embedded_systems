/** @file swi.c
 *
 * @brief SWI handler (C half)
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
 */

#include <lock.h>
#include <types.h>
#include <assert.h>
#include <syscall.h>
#include <bits/swi.h>
#include <arm/psr.h>
#include <arm/swi.h>
#include <arm/exception.h>

static uint32_t* old_swi;        // Old SWI handler start position
static uint32_t old_swi_ins[2];  // Old SWI handler first two instructions

void init_swi() {
    old_swi = install_handler(old_swi_ins, 0, swi_wrapper);
    if (old_swi == NULL) panic("Unrecognized SWI Vector\n");
}

void destroy_swi() {
    *old_swi = old_swi_ins[0];
    *(old_swi + 1) = old_swi_ins[1];
}

/**
 * void swi_handler(unsigned, unsigned *)
 *  - SWI Handler (C half), called by swi_wrapper (Assembly half)
 *  - Call the corresponding syscall
 *  - Syscall's return value saved in $R0 (except those that never returns)
 * Argumnet:
 *  - unsigned swi_num: SWI number
 *  - unsigned *regs: Register list pointer
 */
void swi_handler(uint32_t swi_num, uint32_t *regs)
{
    switch (swi_num) {
        case READ_SWI:
            enable_interrupts();
            read_syscall((int)*regs, (void *)*(regs + 1), (size_t)*(regs + 2));
            disable_interrupts();
            break;
        case WRITE_SWI:
            enable_interrupts();
            write_syscall((int)*regs, (void *)*(regs + 1), (size_t)*(regs + 2));
            disable_interrupts();
            break;
        case TIME_SWI:
            time_syscall();
            break;
        case SLEEP_SWI:
            enable_interrupts();
            sleep_syscall((size_t)*regs);
            disable_interrupts();
            break;
        case CREATE_SWI:
            task_create((task_t *)*regs, (size_t)*(regs + 1));
            break;
        case MUTEX_CREATE:
            mutex_create();
            break;
        case MUTEX_LOCK:
            mutex_lock((int)*regs);
            break;
        case MUTEX_UNLOCK:
            mutex_unlock((int)*regs);
            break;
        case EVENT_WAIT:
            event_wait((unsigned)*regs);
            break;
        default:
            invalid_syscall((uint32_t)*regs);
    }
}
