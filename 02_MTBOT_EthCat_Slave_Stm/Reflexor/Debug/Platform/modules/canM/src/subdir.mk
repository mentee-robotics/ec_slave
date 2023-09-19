################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Platform/modules/canM/src/canM.c \
../Platform/modules/canM/src/canM_Cfg.c \
../Platform/modules/canM/src/canM_Irq.c 

OBJS += \
./Platform/modules/canM/src/canM.o \
./Platform/modules/canM/src/canM_Cfg.o \
./Platform/modules/canM/src/canM_Irq.o 

C_DEPS += \
./Platform/modules/canM/src/canM.d \
./Platform/modules/canM/src/canM_Cfg.d \
./Platform/modules/canM/src/canM_Irq.d 


# Each subdirectory must supply rules for building sources it contributes
Platform/modules/canM/src/%.o Platform/modules/canM/src/%.su Platform/modules/canM/src/%.cyclo: ../Platform/modules/canM/src/%.c Platform/modules/canM/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG=1 -DUSE_HAL_DRIVER -DSTM32G474xx=1 -DDEBUG -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Platform -I../Platform/drivers/can_iso_tp -I../Platform/drivers/lan9252 -I../Platform/drivers/lwrb -I../Platform/drivers/md80 -I../Platform/drivers/spiDma -I../Platform/drivers/uartDma -I../Platform/drivers/soes -I../Platform/drivers/soes/hal/lan9252 -I../Platform/drivers/soes/include/sys/gcc -I../Platform/drivers/tmc/helpers -I../Platform/drivers/tmc/ic/TMC8462 -I../Platform/drivers/tmc/ramp -I../Platform/modules/canIdle -I../Platform/modules/canM/inc -I../Platform/modules/ethCat -I../appsTest -I../appsTest/CanIdle -I../appsTest/LAN9252_Test -I../appsTest/SPIDMA_Test -I../Platform/tools/SystemView/Config -I../Platform/tools/SystemView/FreeRTOSV10.4 -I../Platform/tools/SystemView/FreeRTOSV10.4/Config/Cortex-M -I../Platform/tools/SystemView/SEGGER -I../Platform/tools/SystemView/SEGGER/Syscalls -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Platform-2f-modules-2f-canM-2f-src

clean-Platform-2f-modules-2f-canM-2f-src:
	-$(RM) ./Platform/modules/canM/src/canM.cyclo ./Platform/modules/canM/src/canM.d ./Platform/modules/canM/src/canM.o ./Platform/modules/canM/src/canM.su ./Platform/modules/canM/src/canM_Cfg.cyclo ./Platform/modules/canM/src/canM_Cfg.d ./Platform/modules/canM/src/canM_Cfg.o ./Platform/modules/canM/src/canM_Cfg.su ./Platform/modules/canM/src/canM_Irq.cyclo ./Platform/modules/canM/src/canM_Irq.d ./Platform/modules/canM/src/canM_Irq.o ./Platform/modules/canM/src/canM_Irq.su

.PHONY: clean-Platform-2f-modules-2f-canM-2f-src

