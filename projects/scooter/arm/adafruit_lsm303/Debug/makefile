################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: adafruit_lsm303.axf

# Tool invocations
adafruit_lsm303.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/Users/matthew/code/embedded/scooter/arm/CMSIS_CORE_LPC17xx/Debug" -Xlinker -Map="adafruit_lsm303.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "adafruit_lsm303_Debug.ld" -o "adafruit_lsm303.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) adafruit_lsm303.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "adafruit_lsm303.axf"; # arm-none-eabi-objcopy -O binary "adafruit_lsm303.axf" "adafruit_lsm303.bin" ; checksum -p LPC1769 -d "adafruit_lsm303.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
