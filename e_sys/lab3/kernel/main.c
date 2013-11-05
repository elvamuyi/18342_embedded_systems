/** @file  main.c
 *
 * @brief  Kernel kmain (entry) function
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 15:55
 */

#include <types.h>
#include <kernel.h>
#include <exports.h>
#include <bits/swi.h>

#include <arm/reg.h>
#include <arm/timer.h>
#include <arm/interrupt.h>

// U-Boot global data structure
unsigned global_data;
// SP of kernel before jumping to UserApp
unsigned kStackPtr;

// Install the customized SWI/IRQ Handler
unsigned* Install_Handler(unsigned [], int);
// Customized SWI Handler (Assembly half)
extern int SWI_Handler(void);
// Customized SWI Handler (C half)
void C_SWI_Handler(unsigned, unsigned *);
// Customized IRQ Handler (Assembly half)
extern int IRQ_Handler(void);
// Customized IRQ Handler (C half)
void C_IRQ_Handler(void);

// Call user applications (C half)
unsigned* C_Call_UserApp(int, char*[]);
// Call user applications (Assembly half)
extern int Call_UserApp(int, unsigned *, unsigned);

// Timer driver
extern void initTimer(void);
extern void addTimer(void);

// Syscalls
extern void exit(int);
extern size_t time(void);
extern void sleep(size_t);
extern ssize_t read(int, void *, size_t);
extern ssize_t write(int, const void *, size_t);

int kmain(int argc, char** argv, unsigned table)
{
    app_startup(); /* bss is valid after this point */
    global_data = table;

    // Install our new SWI Handler. Save the old address
    unsigned oldSWI_ins[2];
    unsigned *oldSWI = Install_Handler(oldSWI_ins, 0);
    if (oldSWI == NULL) return 0xbadc0de;

    // Install our new IRQ Handler. Save the old address
    unsigned oldIRQ_ins[2];
    unsigned *oldIRQ = Install_Handler(oldIRQ_ins, 1);
    if (oldIRQ == NULL) return 0xbadc0de;

    initTimer();

    // Call the user application
    unsigned* uStack = C_Call_UserApp(argc, argv);
    int UserApp_Return = Call_UserApp(argc, uStack, IRQ_STACK_ADDR);

    // Restore the SWI and IRQ handler
    *oldSWI = oldSWI_ins[0];
    *(oldSWI + 1) = oldSWI_ins[1];
    *oldIRQ = oldIRQ_ins[0];
    *(oldIRQ + 1) = oldIRQ_ins[1];

    return UserApp_Return;
}

/**
 * unsigned* Install_Handler(unsigned[], int)
 *  - Install the our customized SWI/IRQ Handler
 *  - Replace the handler's first 2 instructions and save the original ones
 * Argumnet:
 *  - unsigned odl_ins[]: Original handler's instructions
 *  - int IRQ_flag: 0 for SWI Handler, others for IRQ Handler
 * Return:
 *  - unsigned* oldPos: Original handler's address
 */
unsigned* Install_Handler(unsigned old_ins[], int IRQ_flag)
{
    // Identify the SWI/IRQ vector
    unsigned *vector = IRQ_flag ? 
        (unsigned *) IRQ_VECTOR : // Install IRQ Handler
        (unsigned *) SWI_VECTOR ; // Install SWI Handler
    unsigned *jmp_table;
    unsigned offset = ((unsigned)(*vector)) & 0x00000FFF;
    unsigned ldr_pc = ((unsigned)(*vector)) & 0xFFFFF000;
    // Positive offset
    if (ldr_pc == 0xe59ff000)
        jmp_table = (unsigned *)((unsigned) vector + 0x8 + offset);
    // Negative offset
    else if (ldr_pc == 0xe51ff000)
        jmp_table = (unsigned *)((unsigned) vector + 0x8 - offset);
    // Unrecognized instuction
    else {
        printf("Unrecognized vector\n");
        return NULL;
    }
    
    // Extract the addr of SWI/IRQ Handler
    unsigned *oldPos = (unsigned *) *jmp_table;
    // Store the old SWI/IRQ Handler's first 2 instructions
    old_ins[0] = (unsigned) *oldPos;
    old_ins[1] = (unsigned) *(oldPos + 1);

    // Replace the old handler's first 2 instructions
    *oldPos = 0xe51ff004;  // LDR pc, [pc, #-4]
    *(oldPos + 1) = IRQ_flag ? 
        (unsigned) (&IRQ_Handler) : (unsigned) (&SWI_Handler);

    return oldPos;
}

/**
 * void C_SWI_Handler(unsigned, unsigned *)
 *  - SWI Handler (C half), called by SWI Handler (Assembly half)
 *  - Call the corresponding syscall
 *  - Syscall's return value saved in $R0 (except exit that never returns)
 * Argumnet:
 *  - unsigned swi_num: SWI number
 *  - unsigned *regs: Register list pointer
 */
void C_SWI_Handler(unsigned swi_num, unsigned *regs)
{
    switch (swi_num) {
        case EXIT_SWI:
            exit((int)*regs);
        case READ_SWI:
            read((int)*regs, (void *)*(regs + 1), (size_t)*(regs + 2));
            break;
        case WRITE_SWI:
            write((int)*regs, (void *)*(regs + 1), (size_t)*(regs + 2));
            break;
        case TIME_SWI:
            time();
            break;
        case SLEEP_SWI:
            sleep((size_t)(*regs));
            break;
        default:
            printf("Invalid syscall\n");
            exit(0xbadc0de);
    }
}

/**
 * void C_IRQ_Handler(void)
 *  - IRQ Handler (C half), called by IRQ Handler (Assembly half)
 *  - Handle OSMR0 timer interrupt only
 *  - Update a timer variable in Timer_Driver
 */
void C_IRQ_Handler(void)
{
    /*
    volatile unsigned* icip = (unsigned *) ICIP_ADDR;
    volatile unsigned* ossr = (unsigned *) OSSR_ADDR;
    volatile unsigned* oscr = (unsigned *) OSCR_ADDR;
    if ((*icip) & 0x4000000) {
        addTimer();
        *ossr = (*ossr) | 0x1;
        *oscr = 0x0;
    }
    */
    if (reg_read(INT_ICIP_ADDR) & 0x4000000) {
        addTimer();
        // Clear the interrupt and reset the OSCR
        reg_set(OSTMR_OSSR_ADDR, 0x1);
        reg_write(OSTMR_OSCR_ADDR, 0x0);
    }
}

/**
 * unsigned* C_Call_UserApp(int, char *[])
 *  - Preparation for calling UserApp (C half)
 *  - Push the argc and argv into user mode stack
 * Argumnet:
 *  - int uArgc: Kernel input argc
 *  - char *uArgv[]: Kernel input argv
 * Return:
 *  - unsigned* uStack: User mode stack pointer
 */
unsigned* C_Call_UserApp(int uArgc, char *uArgv[])
{
    // Push argc and argv on the user mode stack
    int count;
    unsigned *uStack = (unsigned *) (USER_STACK_ADDR - 4);
    // [sp+4(argc)+4] = NULL
    *uStack-- = 0x0;
    // [sp+4(argc)] = argv[argc-1]
    for (count = uArgc; count > 0; count--)
        *uStack-- = (unsigned) (uArgv[count - 1]);
    // [sp] = argc
    *uStack = (unsigned) uArgc;
    // Transfer to User mode, jump to UserApp from assembly
    return uStack;
}
