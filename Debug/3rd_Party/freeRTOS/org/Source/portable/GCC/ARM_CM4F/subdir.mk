################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../3rd_Party/freeRTOS/org/Source/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./3rd_Party/freeRTOS/org/Source/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./3rd_Party/freeRTOS/org/Source/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
3rd_Party/freeRTOS/org/Source/portable/GCC/ARM_CM4F/%.o: ../3rd_Party/freeRTOS/org/Source/portable/GCC/ARM_CM4F/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -DSTM32F446xx -DUSE_STDPERIPH_DRIVER -I"Z:/RTOS_Workspace/STM32_TaskDelete/StdPeriph_Driver/inc" -I"Z:/RTOS_Workspace/STM32_TaskDelete/3rd_Party/freeRTOS/org/Source/include" -I"Z:/RTOS_Workspace/STM32_TaskDelete/3rd_Party/freeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"Z:/RTOS_Workspace/STM32_TaskDelete/inc" -I"Z:/RTOS_Workspace/STM32_TaskDelete/StdPeriph_Driver/inc" -I"Z:/RTOS_Workspace/STM32_TaskDelete/inc" -I"Z:/RTOS_Workspace/STM32_TaskDelete/CMSIS/device" -I"Z:/RTOS_Workspace/STM32_TaskDelete/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


