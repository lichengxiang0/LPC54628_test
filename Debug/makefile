################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include sdmmc/src/subdir.mk
-include sdmmc/port/subdir.mk
-include fatfs/fatfs_source/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/serial_manager/subdir.mk
-include board/subdir.mk
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
all: LPC54628J512_Project.axf

# Tool invocations
LPC54628J512_Project.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\^_^\Documents\MCUXpressoIDE_10.3.1_2233\workspace\LPC54628J512_Project\libs" -Xlinker -Map="LPC54628J512_Project.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T LPC54628J512_Project_Debug.ld -o "LPC54628J512_Project.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) LPC54628J512_Project.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "LPC54628J512_Project.axf"; # arm-none-eabi-objcopy -v -O binary "LPC54628J512_Project.axf" "LPC54628J512_Project.bin" ; # checksum -p LPC54628J512 -d "LPC54628J512_Project.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
