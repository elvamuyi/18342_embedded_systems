/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * Date:   2013/10/22  11:30am
 */

#include <kernel.h>
#include <exports.h>
#include <bits/types.h>

// U-Boot global data structure
unsigned UBoot_R8;
// SP of kernel before jumping to UserApp
unsigned kStackPtr;

// Install the customized SWI Handler
unsigned* Install_SWI_Handler(unsigned []);
// Customized SWI Handler (C half)
int C_SWI_Handler(unsigned, unsigned *);
// Customized SWI Handler (Assembly half)
extern int SWI_Handler();

// Call user applications (C half)
unsigned* C_Call_UserApp(int, char*[]);
// Call user applications (Assembly half)
extern int Call_UserApp(int, unsigned *);

// Syscalls
extern void exit(int);
extern ssize_t read(int, void *, size_t);
extern ssize_t write(int, const void *, size_t);

int main(int argc, char *argv[])
{
    // Install our new SWI Handler. Save the old address
    unsigned oldSwi_ins[2];
    unsigned *oldSwi = Install_SWI_Handler(oldSwi_ins);

    // Call the user application
    unsigned* uStack = C_Call_UserApp(argc, argv);
    int UserApp_Return = Call_UserApp(argc, uStack);

    // Restore the SWI handler
    *oldSwi = oldSwi_ins[0];
    *(oldSwi + 1) = oldSwi_ins[1];

    return UserApp_Return;
}

/*
 * unsigned* Install_SWI_Handler(unsigned[])
 *  - Install the our customized SWI Handler
 *  - Replace the handler's first 2 instructions and save the original ones
 * Argumnet:
 *  - unsigned odlSwi_ins[]: Original SWI handler's instructions
 * Return:
 *  - unsigned* oldSwi: Original SWI Handler address
 */
unsigned* Install_SWI_Handler(unsigned oldSwi_ins[])
{
    int (*SWI_Handler_Ptr)() = &SWI_Handler;

    // Identify the SWI vector
    unsigned *jmp_table = 0x0;
    unsigned *vector = (unsigned *) SWI_VECTOR;
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
        printf("Unrecognized SWI vector\n");
        exit(0xbadc0de);
    }
    
    // Extract the addr of SWI Handler
    unsigned *oldSwi = (unsigned *) *jmp_table;
    // Store the old SWI Handler's first 2 instructions
    oldSwi_ins[0] = (unsigned) *oldSwi;
    oldSwi_ins[1] = (unsigned) *(oldSwi + 1);

    // Replace the old handler's first 2 instructions
    *oldSwi = 0xe51ff004;  // LDR pc, [pc, #-4]
    *(oldSwi + 1) = (unsigned) SWI_Handler_Ptr;

    return oldSwi;
}

/*
 * int C_SWI_Handler(unsigned, unsigned *)
 *  - SWI Handler (C half), called by SWI Handler (Assembly half)
 *  - Call the corresponding syscall
 * Argumnet:
 *  - unsigned swi_num: SWI number
 *  - unsigned *regs: Register list pointer
 * Return:
 *  - Syscall's return value (except exit that never returns)
 */
int C_SWI_Handler(unsigned swi_num, unsigned *regs)
{
    switch (swi_num) {
        case 0x900001:
            exit((int)*regs);
        case 0x900003:
            return read((int)*regs, (void *)*(regs + 1), (size_t)*(regs + 2));
        case 0x900004:
            return write((int)*regs, (void *)*(regs + 1), (size_t)*(regs + 2));
        default:
            printf("Invalid syscall\n");
            exit(0xbadc0de);
    }
    return 1;
}

/*
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
