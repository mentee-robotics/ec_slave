################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c \
../Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.c \
../Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.c \
../Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.c 

OBJS += \
./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o \
./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.o \
./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.o \
./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.o 

C_DEPS += \
./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d \
./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.d \
./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.d \
./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.d 


# Each subdirectory must supply rules for building sources it contributes
Platform/tools/SystemView/SEGGER/Syscalls/%.o Platform/tools/SystemView/SEGGER/Syscalls/%.su Platform/tools/SystemView/SEGGER/Syscalls/%.cyclo: ../Platform/tools/SystemView/SEGGER/Syscalls/%.c Platform/tools/SystemView/SEGGER/Syscalls/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG=1 -DUSE_HAL_DRIVER -DSTM32G474xx=1 -DDEBUG -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Platform -I../Platform/drivers/can_iso_tp -I../Platform/drivers/lan9252 -I../Platform/drivers/lwrb -I../Platform/drivers/md80 -I../Platform/drivers/spiDma -I../Platform/drivers/uartDma -I../Platform/drivers/soes -I../Platform/drivers/soes/hal/lan9252 -I../Platform/drivers/soes/include/sys/gcc -I../Platform/drivers/tmc/helpers -I../Platform/drivers/tmc/ic/TMC8462 -I../Platform/drivers/tmc/ramp -I../Platform/modules/canIdle -I../Platform/modules/canM/inc -I../Platform/modules/ethCat -I../appsTest -I../appsTest/CanIdle -I../appsTest/LAN9252_Test -I../appsTest/SPIDMA_Test -I../Platform/tools/SystemView/Config -I../Platform/tools/SystemView/FreeRTOSV10.4 -I../Platform/tools/SystemView/FreeRTOSV10.4/Config/Cortex-M -I../Platform/tools/SystemView/SEGGER -I../Platform/tools/SystemView/SEGGER/Syscalls -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Platform-2f-tools-2f-SystemView-2f-SEGGER-2f-Syscalls

clean-Platform-2f-tools-2f-SystemView-2f-SEGGER-2f-Syscalls:
	-$(RM) ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.cyclo ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.su ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.cyclo ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.d ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.o ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.su ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.cyclo ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.d ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.o ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.su ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.cyclo ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.d ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.o ./Platform/tools/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.su

.PHONY: clean-Platform-2f-tools-2f-SystemView-2f-SEGGER-2f-Syscalls

