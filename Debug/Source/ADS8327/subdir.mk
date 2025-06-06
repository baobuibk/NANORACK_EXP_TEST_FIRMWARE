################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/ADS8327/ads8327.c 

OBJS += \
./Source/ADS8327/ads8327.o 

C_DEPS += \
./Source/ADS8327/ads8327.d 


# Each subdirectory must supply rules for building sources it contributes
Source/ADS8327/%.o Source/ADS8327/%.su Source/ADS8327/%.cyclo: ../Source/ADS8327/%.c Source/ADS8327/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/ADG1414" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/ADS8327" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/MCP4902" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/ADC_DMA_Device" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/scheduler" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/UART" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/CMDLine" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/FRAM" -I"D:/WorkSpace/STM32 PRJ/EXP_test/Source/NTC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Source-2f-ADS8327

clean-Source-2f-ADS8327:
	-$(RM) ./Source/ADS8327/ads8327.cyclo ./Source/ADS8327/ads8327.d ./Source/ADS8327/ads8327.o ./Source/ADS8327/ads8327.su

.PHONY: clean-Source-2f-ADS8327

