# Name: Makefile
# Author: <insert your name here>
# Copyright: <insert your copyright message here>
# License: <insert your license reference here>

# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected. We recommend that you leave it undefined and
#                add settings like this to your ~/.avrduderc file:
#                   default_programmer = "stk500v2"
#                   default_serial = "avrdoper"
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

OBJECTS   = $(SOURCES:.c=.o)

# For computing fuse byte values for other devices and options see
# the fuse bit calculator at http://www.engbedded.com/fusecalc/


# Tune the lines below only if you know what you are doing:

CROSSPACK = /usr/local/CrossPack-AVR/bin
AVRDUDE = $(CROSSPACK)/avrdude -c $(PROGRAMMER) -p $(DEVICE)
COMPILE = $(CROSSPACK)/avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
OBJCOPY = $(CROSSPACK)/avr-objcopy
AVRSIZE = $(CROSSPACK)/avr-size

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $(OUTPUT)/$@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $(OUTPUT)/$@

flash:	all
	$(AVRDUDE) -U flash:w:$(OUTPUT)/main.hex:i

fuse:
	$(AVRDUDE) -U lfuse:w:$(FUSE_L):m -U hfuse:w:$(FUSE_H):m

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean: ;\
  mkdir -p $(OUTPUT).bak ;\
  mv $(OUTPUT)/* $(OUTPUT).bak/ ;\
	rm -f $(OUTPUT)/*

# file targets:
main.elf: $(OBJECTS) ;\
	$(COMPILE) -o $(OUTPUT)/main.elf $(OUTPUT)/$(OBJECTS)

main.hex: main.elf ;\
	$(OBJCOPY) -j .text -j .data -O ihex $(OUTPUT)/main.elf $(OUTPUT)/main.hex
	$(AVRSIZE) --format=avr --mcu=$(DEVICE) $(OUTPUT)/main.elf