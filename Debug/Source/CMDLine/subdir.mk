################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/CMDLine/cmdline.c \
../Source/CMDLine/command.c 

OBJS += \
./Source/CMDLine/cmdline.o \
./Source/CMDLine/command.o 

C_DEPS += \
./Source/CMDLine/cmdline.d \
./Source/CMDLine/command.d 


# Each subdirectory must supply rules for building sources it contributes
Source/CMDLine/%.o Source/CMDLine/%.su Source/CMDLine/%.cyclo: ../Source/CMDLine/%.c Source/CMDLine/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/ADG1414" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/MCP4902" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/ADC_DMA_Device" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/scheduler" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/UART" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/CMDLine" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Source-2f-CMDLine

clean-Source-2f-CMDLine:
	-$(RM) ./Source/CMDLine/cmdline.cyclo ./Source/CMDLine/cmdline.d ./Source/CMDLine/cmdline.o ./Source/CMDLine/cmdline.su ./Source/CMDLine/command.cyclo ./Source/CMDLine/command.d ./Source/CMDLine/command.o ./Source/CMDLine/command.su

.PHONY: clean-Source-2f-CMDLine

