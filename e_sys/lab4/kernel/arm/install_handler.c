/** @file install_handler.c
 *
 * @brief Install SWI or IRQ handler
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
 */

#include <types.h>
#include <assert.h>
#include <arm/physmem.h>

/**
 * uint32_t* install_handler(uint32_t[], int, void*)
 *  - Install the our customized SWI/IRQ Handler
 *  - Replace the handler's first 2 instructions and save the original ones
 * Argumnet:
 *  - uint32_t odl_ins[]: Original handler's instructions
 *  - int IRQ_flag: 0 for SWI Handler, others for IRQ Handler
 * Return:
 *  - uint32_t* oldPos: Original handler's address
 */
uint32_t* install_handler(uint32_t old_ins[], int IRQ_flag, void* handler)
{
    // Identify the SWI/IRQ vector
    uint32_t *vector = IRQ_flag ? 
        (uint32_t *) IRQ_VECTOR : // Install IRQ Handler
        (uint32_t *) SWI_VECTOR ; // Install SWI Handler
    uint32_t *jmp_table;
    uint32_t offset = ((uint32_t)(*vector)) & 0x00000FFF;
    uint32_t ldr_pc = ((uint32_t)(*vector)) & 0xFFFFF000;
    // Positive offset
    if (ldr_pc == 0xe59ff000)
        jmp_table = (uint32_t *)((uint32_t) vector + 0x8 + offset);
    // Negative offset
    else if (ldr_pc == 0xe51ff000)
        jmp_table = (uint32_t *)((uint32_t) vector + 0x8 - offset);
    // Unrecognized instuction
    else {
        panic("Unrecognized vector\n");
        return NULL;
    }
    
    // Extract the addr of SWI/IRQ Handler
    uint32_t *oldPos = (uint32_t *) *jmp_table;
    // Store the old SWI/IRQ Handler's first 2 instructions
    old_ins[0] = (uint32_t) *oldPos;
    old_ins[1] = (uint32_t) *(oldPos + 1);

    // Replace the old handler's first 2 instructions
    *oldPos = 0xe51ff004;  // LDR pc, [pc, #-4]
    *(oldPos + 1) = (uint32_t) handler;

    return oldPos;
}
