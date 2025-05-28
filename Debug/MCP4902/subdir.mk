################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCP4902/mcp4902.c 

OBJS += \
./MCP4902/mcp4902.o 

C_DEPS += \
./MCP4902/mcp4902.d 


# Each subdirectory must supply rules for building sources it contributes
MCP4902/%.o MCP4902/%.su MCP4902/%.cyclo: ../MCP4902/%.c MCP4902/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/WorkSpace/STM32 PRJ/EXP_test/ADG1414" -I"D:/WorkSpace/STM32 PRJ/EXP_test/MCP4902" -I"D:/WorkSpace/STM32 PRJ/EXP_test/ADS8327" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MCP4902

clean-MCP4902:
	-$(RM) ./MCP4902/mcp4902.cyclo ./MCP4902/mcp4902.d ./MCP4902/mcp4902.o ./MCP4902/mcp4902.su

.PHONY: clean-MCP4902

