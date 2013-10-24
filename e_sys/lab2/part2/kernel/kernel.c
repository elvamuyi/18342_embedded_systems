/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * Date:   2013/10/22  11:30am
 */

#include <exports.h>
#include <bits/types.h>
#include <bits/errno.h>
#include <bits/fileno.h>

#define SWI_VECTOR 0x08
#define USER_APP_ADDR 0xa2000000

// Customized SWI Handler in assembly
extern int SWI_Handler();
// Customized SWI Handler in C
void C_SWI_Handler(unsigned, unsigned *);
// Install the customized SWI Handler
unsigned * Install_SWI_Handler(unsigned []);

// Syscalls
extern void exit(int);
extern ssize_t read(int, void *, size_t);
extern ssize_t write(int, const void *, size_t);

int main(int argc, char *argv[])
{
    void (*User_App)() = (void *) USER_APP_ADDR;

    // Install our new SWI Handler. Save the old address
    unsigned oldswi_ins[2];
    unsigned *oldswi = Install_SWI_Handler(oldswi_ins);
    
    // Call the user application
    (*User_App)();

    // Restore the SWI handler
    *oldswi = oldswi_ins[0];
    *(oldswi + 1) = oldswi_ins[1];

    /*
    void *buf = malloc(10);
    int rcount = read(STDIN_FILENO, buf, 10);
    printf("Read: %d\nBuffer: %s\n", rcount, buf);

    int wcount = write(STDOUT_FILENO, buf, 10);
    printf("Write: %d\nBuffer: %s\n", wcount, buf);
    */
    return 0;
}

unsigned * Install_SWI_Handler(unsigned oldswi_ins[])
{
    int (*SWI_Handler_Ptr)() = &SWI_Handler;

    // Identify the SWI vector
    unsigned *jmp_table;
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
        return 0x0;
        //exit(0xbadc0de);
    }
    
    // Extract the addr of SWI Handler
    unsigned *oldswi = (unsigned *) *jmp_table;
    // Store the old SWI Handler's first 2 instructions
    oldswi_ins[0] = (unsigned) *oldswi;
    oldswi_ins[1] = (unsigned) *(oldswi + 1);

    // Replace the old handler's first 2 instructions
    *oldswi = 0xe51ff004;  // LDR pc, [pc, #-4]
    *(oldswi + 1) = (unsigned) SWI_Handler_Ptr;

    return oldswi;
}

void C_SWI_Handler(unsigned swi_num, unsigned *regs) {
    switch (swi_num) {
        /*
        case 0x900001:
            exit((int)*regs);
            break;
        */
        case 0x900003:
            read((int)*regs, (void *)*(regs + 1), (size_t)*(regs + 2));
            break;
        case 0x900004:
            write((int)*regs, (void *)*(regs + 1), (size_t)*(regs + 2));
            break;
        default:
            printf("Invalid syscall\n");
            //exit(0xbadc0de);
    }
}
