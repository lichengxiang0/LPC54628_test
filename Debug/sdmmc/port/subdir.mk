################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sdmmc/port/fsl_sdmmc_event.c \
../sdmmc/port/fsl_sdmmc_host.c 

OBJS += \
./sdmmc/port/fsl_sdmmc_event.o \
./sdmmc/port/fsl_sdmmc_host.o 

C_DEPS += \
./sdmmc/port/fsl_sdmmc_event.d \
./sdmmc/port/fsl_sdmmc_host.d 


# Each subdirectory must supply rules for building sources it contributes
sdmmc/port/%.o: ../sdmmc/port/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DCPU_LPC54628J512ET180 -DCPU_LPC54628J512ET180_cm4 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I../board -I../source -I../ -I../drivers -I../device -I../CMSIS -I../component/serial_manager -I../component/uart -I../utilities -I../sdmmc/port -I../sdmmc/inc -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


