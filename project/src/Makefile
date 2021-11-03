# makfile configuration

COMMON_OBJECTS  =

CPU             = msp430g2553

CFLAGS          = -mmcu=${CPU} -I../h -L/opt/ti/msp430_gcc/include

LDFLAGS		= -L/opt/ti/msp430_gcc/include




#switch the compiler (for the internal make rules)

CC              = msp430-elf-gcc

AS              = msp430-elf-as



all: buzzer.elf



#additional rules for files

buzzer.elf: playSongTwo-c.o buzzer.o main.o ../lib/libTimer.a

	${CC} ${CFLAGS} -o $@ $^



load: buzzer.elf

	msp430loader.sh  $^

clean: $^

	rm -f *.o *.elf

playSongTwo-c.o: buzzer.h
