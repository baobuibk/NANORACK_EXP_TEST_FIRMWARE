################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ADG1414/adg1414.c 

OBJS += \
./ADG1414/adg1414.o 

C_DEPS += \
./ADG1414/adg1414.d 


# Each subdirectory must supply rules for building sources it contributes
ADG1414/%.o ADG1414/%.su ADG1414/%.cyclo: ../ADG1414/%.c ADG1414/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/WorkSpace/STM32 PRJ/EXP_test/ADG1414" -I"D:/WorkSpace/STM32 PRJ/EXP_test/MCP4902" -I"D:/WorkSpace/STM32 PRJ/EXP_test/ADS8327" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ADG1414

clean-ADG1414:
	-$(RM) ./ADG1414/adg1414.cyclo ./ADG1414/adg1414.d ./ADG1414/adg1414.o ./ADG1414/adg1414.su

.PHONY: clean-ADG1414

