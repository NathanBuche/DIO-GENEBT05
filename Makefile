# Makefile for watchdog

EXEC = superio
OBJS = superio.o libaaeon.o aaeon_f81866d.o aaeon_superio.o
CC = gcc -g
CFLAGS = -I/usr/src/linux/include/ -O2

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

clean:
	rm -f $(EXEC) *.gdb *.elf *.o
