/** @file main.c
 *
 * @brief kernel main
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
 */
 
#include <kernel.h>

uint32_t global_data;

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
    int user_return = enter_user_mode();

    /*
     * Real-time kernel never reaches below 
     */
    destroy_timer();
    destroy_swi();
    destroy_interrupt();

    return user_return;
}
