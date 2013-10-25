CC      = gcc
OBJCOPY = objcopy
ASFLAGS = -nostdinc -I include
LDFLAGS = -nostdlib -Wl,-e,-N,-Ttext,$(LOAD_ADDR) 

LOAD_ADDR = 0xa2000000

objects = hello.o

default: hello.bin

.PHONY: default clean clobber

hello.bin: hello
	$(OBJCOPY) -O binary $< $@

hello: $(objects)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $<

clean:
	rm -f hello $(objects)

clobber: clean
	rm -f hello.bin
