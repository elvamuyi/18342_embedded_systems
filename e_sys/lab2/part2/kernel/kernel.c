/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * Date:   2013/10/22  11:30am
 */

#include <exports.h>
#include <bits/swi.h>
#include <bits/fileno.h>

#define SWI_VECTOR 0x08
#define USER_APP_ADDR 0xa2000000

// Customized SWI Handler
extern int S_Handler();

// Syscall
void C_SWI_Handler(unsigned swi_num, unsigned *regs) {
    switch (swi_num) {
        /*
        case 1:
            break;
        case 3:
            break;
        case 4:
            break;
        */
        default:
            printf("Invalid syscall\n");
            //exit(0xbadc0de);
    }
}

int main(int argc, char *argv[]) {
	int (*S_Handler_Ptr)() = &S_Handler;
	void (*User_App)() = (void *) USER_APP_ADDR;

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
        return 1;
        //exit(0xbadc0de);
    }
    
    // Extract the addr of SWI Handler
    unsigned *oldswi = (unsigned *) *jmp_table;

    // Store the old SWI Handler's first 2 instructions
    unsigned oldswi_ins[2];
    oldswi_ins[0] = (unsigned) *oldswi;
    oldswi_ins[1] = (unsigned) *(oldswi + 1);

    // Install the new SWI Handler
    *oldswi = 0xe51ff004;  // LDR pc, [pc, #-4]
    *(oldswi + 1) = (unsigned) S_Handler_Ptr;

    // Call the user application
    (*User_App)();

    // Restore the SWI handler
    *oldswi = oldswi_ins[0];
    *(oldswi + 1) = oldswi_ins[1];

    return 0;
}
