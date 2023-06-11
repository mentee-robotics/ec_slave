################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/CanIDLE/src/canIdle.c \
../Application/CanIDLE/src/canIdle_Config.c \
../Application/CanIDLE/src/canIdle_Irq.c 

OBJS += \
./Application/CanIDLE/src/canIdle.o \
./Application/CanIDLE/src/canIdle_Config.o \
./Application/CanIDLE/src/canIdle_Irq.o 

C_DEPS += \
./Application/CanIDLE/src/canIdle.d \
./Application/CanIDLE/src/canIdle_Config.d \
./Application/CanIDLE/src/canIdle_Irq.d 


# Each subdirectory must supply rules for building sources it contributes
Application/CanIDLE/src/%.o Application/CanIDLE/src/%.su Application/CanIDLE/src/%.cyclo: ../Application/CanIDLE/src/%.c Application/CanIDLE/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/hal" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/hal/tmc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/include/sys/gcc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/spiDma" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc/helpers" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc/ic/TMC8462" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/hal/lan9252" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/lan9252" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/EthCatSlave" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/CanIDLE" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/CanIDLE/inc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/CanIDLE/src" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/can_iso_tp" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Apps/app_md80" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/AppsTest/LAN9252_Test" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-CanIDLE-2f-src

clean-Application-2f-CanIDLE-2f-src:
	-$(RM) ./Application/CanIDLE/src/canIdle.cyclo ./Application/CanIDLE/src/canIdle.d ./Application/CanIDLE/src/canIdle.o ./Application/CanIDLE/src/canIdle.su ./Application/CanIDLE/src/canIdle_Config.cyclo ./Application/CanIDLE/src/canIdle_Config.d ./Application/CanIDLE/src/canIdle_Config.o ./Application/CanIDLE/src/canIdle_Config.su ./Application/CanIDLE/src/canIdle_Irq.cyclo ./Application/CanIDLE/src/canIdle_Irq.d ./Application/CanIDLE/src/canIdle_Irq.o ./Application/CanIDLE/src/canIdle_Irq.su

.PHONY: clean-Application-2f-CanIDLE-2f-src

