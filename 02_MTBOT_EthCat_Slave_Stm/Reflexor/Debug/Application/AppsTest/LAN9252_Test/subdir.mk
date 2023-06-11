################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/AppsTest/LAN9252_Test/appTest_lan9252.c 

OBJS += \
./Application/AppsTest/LAN9252_Test/appTest_lan9252.o 

C_DEPS += \
./Application/AppsTest/LAN9252_Test/appTest_lan9252.d 


# Each subdirectory must supply rules for building sources it contributes
Application/AppsTest/LAN9252_Test/%.o Application/AppsTest/LAN9252_Test/%.su Application/AppsTest/LAN9252_Test/%.cyclo: ../Application/AppsTest/LAN9252_Test/%.c Application/AppsTest/LAN9252_Test/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/hal" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/hal/tmc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/include/sys/gcc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/spiDma" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc/helpers" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc/ic/TMC8462" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/hal/lan9252" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/lan9252" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/EthCatSlave" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/CanIDLE" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/CanIDLE/inc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/CanIDLE/src" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/can_iso_tp" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Apps/app_md80" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/AppsTest/LAN9252_Test" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-AppsTest-2f-LAN9252_Test

clean-Application-2f-AppsTest-2f-LAN9252_Test:
	-$(RM) ./Application/AppsTest/LAN9252_Test/appTest_lan9252.cyclo ./Application/AppsTest/LAN9252_Test/appTest_lan9252.d ./Application/AppsTest/LAN9252_Test/appTest_lan9252.o ./Application/AppsTest/LAN9252_Test/appTest_lan9252.su

.PHONY: clean-Application-2f-AppsTest-2f-LAN9252_Test

