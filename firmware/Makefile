# AVR-GCC Makefile
PROJECT=mouse
SOURCES=mouse.c
CC=avr-gcc
OBJCOPY=avr-objcopy
MMCU=attiny13

CFLAGS=-mmcu=$(MMCU)

$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -j .data -O ihex $(PROJECT).out $(PROJECT).hex

$(PROJECT).out: $(SOURCES)
	$(CC) $(CFLAGS) -Os -I./ -o $(PROJECT).out $(SOURCES)

p: $(PROJECT).hex
	avrdude -p t13 -c usbasp  -u -U flash:w:$(PROJECT).hex:i

burn_fuse: $(PROJECT).hex
	avrdude -p t13 -c usbasp -U lfuse:w:0xef:m
	
clean:
	rm -f $(PROJECT).out
	rm -f $(PROJECT).hex
	rm -f $(PROJECT).s
