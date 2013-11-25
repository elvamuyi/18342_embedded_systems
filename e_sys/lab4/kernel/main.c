/** @file main.c
 *
 * @brief kernel main
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
 */
 
#include <task.h>
#include <lock.h>
#include <sched.h>
#include <kernel.h>
#include <device.h>
#include <assert.h>

uint32_t global_data;
// SP of kernel before jumping to UserApp
unsigned kStackPtr;

int kmain(int argc __attribute__((unused)), char** argv __attribute__((unused)), uint32_t table)
{
    app_startup();
    global_data = table;

    // Install our new SWI/IRQ Handler
    init_swi();
    init_interrupt();
    
    // Initilalize system hardware
    dev_init();
    mutex_init();
    init_timer();

    // Call user tasks
    int user_return = c_enter_user_mode(argc, argv);

    /*
     * Real-time kernel never reaches below 
     */

    // Restore the timer, SWI and IRQ handler
    destroy_timer();
    destroy_swi();
    destroy_interrupt();

    return user_return;
}

/**
 * int C_Call_UserApp(int, char *[])
 *  - Preparation for calling UserApp (C half)
 *  - Push the argc and argv into user mode stack
 * Argumnet:
 *  - int uArgc: Kernel input argc
 *  - char *uArgv[]: Kernel input argv
 * Return:
 *  - int Call_UserApp: User application return value
 */
int c_enter_user_mode(int uArgc, char *uArgv[])
{
    // Push argc and argv on the user mode stack
    int count;
    unsigned *uStack = (unsigned *) (USR_STACK - 4);
    // [sp+4(argc)+4] = NULL
    *uStack-- = 0x0;
    // [sp+4(argc)] = argv[argc-1]
    for (count = uArgc; count > 0; count--)
        *uStack-- = (unsigned) (uArgv[count - 1]);
    // [sp] = argc
    *uStack = (unsigned) uArgc;
    // Transfer to User mode, jump to UserApp from assembly
    return enter_user_mode(uArgc, uStack, IRQ_STACK_ADDR);
}
