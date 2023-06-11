################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Drivers/soes/ecat_slv.c \
../Application/Drivers/soes/esc.c \
../Application/Drivers/soes/esc_coe.c \
../Application/Drivers/soes/esc_eep.c \
../Application/Drivers/soes/esc_eoe.c \
../Application/Drivers/soes/esc_foe.c 

OBJS += \
./Application/Drivers/soes/ecat_slv.o \
./Application/Drivers/soes/esc.o \
./Application/Drivers/soes/esc_coe.o \
./Application/Drivers/soes/esc_eep.o \
./Application/Drivers/soes/esc_eoe.o \
./Application/Drivers/soes/esc_foe.o 

C_DEPS += \
./Application/Drivers/soes/ecat_slv.d \
./Application/Drivers/soes/esc.d \
./Application/Drivers/soes/esc_coe.d \
./Application/Drivers/soes/esc_eep.d \
./Application/Drivers/soes/esc_eoe.d \
./Application/Drivers/soes/esc_foe.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Drivers/soes/%.o Application/Drivers/soes/%.su Application/Drivers/soes/%.cyclo: ../Application/Drivers/soes/%.c Application/Drivers/soes/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/hal" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/hal/tmc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/include/sys/gcc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/spiDma" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc/helpers" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc/ic/TMC8462" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/hal/lan9252" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/lan9252" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/EthCatSlave" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/CanIDLE" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/CanIDLE/inc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/CanIDLE/src" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/can_iso_tp" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Apps/app_md80" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/AppsTest/LAN9252_Test" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-Drivers-2f-soes

clean-Application-2f-Drivers-2f-soes:
	-$(RM) ./Application/Drivers/soes/ecat_slv.cyclo ./Application/Drivers/soes/ecat_slv.d ./Application/Drivers/soes/ecat_slv.o ./Application/Drivers/soes/ecat_slv.su ./Application/Drivers/soes/esc.cyclo ./Application/Drivers/soes/esc.d ./Application/Drivers/soes/esc.o ./Application/Drivers/soes/esc.su ./Application/Drivers/soes/esc_coe.cyclo ./Application/Drivers/soes/esc_coe.d ./Application/Drivers/soes/esc_coe.o ./Application/Drivers/soes/esc_coe.su ./Application/Drivers/soes/esc_eep.cyclo ./Application/Drivers/soes/esc_eep.d ./Application/Drivers/soes/esc_eep.o ./Application/Drivers/soes/esc_eep.su ./Application/Drivers/soes/esc_eoe.cyclo ./Application/Drivers/soes/esc_eoe.d ./Application/Drivers/soes/esc_eoe.o ./Application/Drivers/soes/esc_eoe.su ./Application/Drivers/soes/esc_foe.cyclo ./Application/Drivers/soes/esc_foe.d ./Application/Drivers/soes/esc_foe.o ./Application/Drivers/soes/esc_foe.su

.PHONY: clean-Application-2f-Drivers-2f-soes

