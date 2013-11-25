ARM_OBJS := reg.o psr.o timer.o swi.o swi_asm.o \
	irq_handler.o int.o install_handler.o
ARM_OBJS := $(ARM_OBJS:%=$(KDIR)/arm/%)

KOBJS += $(ARM_OBJS)
