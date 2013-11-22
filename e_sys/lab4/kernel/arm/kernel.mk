ARM_OBJS := reg.o psr.o timer.o swi_handler.o irq_handler.o
ARM_OBJS := $(ARM_OBJS:%=$(KDIR)/arm/%)

KOBJS += $(ARM_OBJS)
