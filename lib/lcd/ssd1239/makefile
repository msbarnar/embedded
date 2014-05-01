# Name: Makefile
# Author: <insert your name here>
# Copyright: <insert your copyright message here>
# License: <insert your license reference here>

OBJECTS   = $(SOURCES:%.c=$(BIN_DIR)/%.o)

CROSSPACK = /usr/local/CrossPack-AVR/bin
AVRDUDE = $(CROSSPACK)/avrdude -c $(PROGRAMMER) -p $(DEVICE)
COMPILE = $(CROSSPACK)/avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
OBJCOPY = $(CROSSPACK)/avr-objcopy
AVRSIZE = $(CROSSPACK)/avr-size

# symbolic targets:
all:	main.hex

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(COMPILE) -I ./include/ -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $(BIN_DIR)/$@

flash:	all
	$(AVRDUDE) -U flash:w:$(BIN_DIR)/main.hex:i

fuse:
	$(AVRDUDE) -U lfuse:w:$(FUSE_L):m -U hfuse:w:$(FUSE_H):m

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean: ;\
  mkdir -p $(BIN_DIR).bak ;\
  mv $(BIN_DIR)/* $(BIN_DIR).bak/ ;\
	rm -f $(BIN_DIR)/*

# file targets:
main.elf: $(OBJECTS) ;\
	$(COMPILE) -o $(BIN_DIR)/main.elf $(OBJECTS)

main.hex: main.elf ;\
	$(OBJCOPY) -j .text -j .data -O ihex $(BIN_DIR)/main.elf $(BIN_DIR)/main.hex
	$(AVRSIZE) --format=avr --mcu=$(DEVICE) $(BIN_DIR)/main.elf