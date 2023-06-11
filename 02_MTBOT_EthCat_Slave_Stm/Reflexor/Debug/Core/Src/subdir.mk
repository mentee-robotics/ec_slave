################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/app_freertos.c \
../Core/Src/dma.c \
../Core/Src/fdcan.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/main.c \
../Core/Src/spi.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_hal_timebase_tim.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c \
../Core/Src/usart.c \
../Core/Src/usb.c 

OBJS += \
./Core/Src/app_freertos.o \
./Core/Src/dma.o \
./Core/Src/fdcan.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/spi.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_hal_timebase_tim.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o \
./Core/Src/usart.o \
./Core/Src/usb.o 

C_DEPS += \
./Core/Src/app_freertos.d \
./Core/Src/dma.d \
./Core/Src/fdcan.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/main.d \
./Core/Src/spi.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_hal_timebase_tim.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d \
./Core/Src/usart.d \
./Core/Src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/hal" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/hal/tmc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/soes/include/sys/gcc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/spiDma" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc/helpers" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/tmc/ic/TMC8462" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/Drivers/md80" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/EthCatSlave" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/CanIDLE" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/CanIDLE/inc" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/AppsTest" -I"/home/administrator/Project/MTBOT/02_MTBOT_EthCat_Slave_Stm/Reflexor/Application/AppsTest/TMC_Test" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/app_freertos.cyclo ./Core/Src/app_freertos.d ./Core/Src/app_freertos.o ./Core/Src/app_freertos.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/fdcan.cyclo ./Core/Src/fdcan.d ./Core/Src/fdcan.o ./Core/Src/fdcan.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32g4xx_hal_msp.cyclo ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_hal_timebase_tim.cyclo ./Core/Src/stm32g4xx_hal_timebase_tim.d ./Core/Src/stm32g4xx_hal_timebase_tim.o ./Core/Src/stm32g4xx_hal_timebase_tim.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/usb.cyclo ./Core/Src/usb.d ./Core/Src/usb.o ./Core/Src/usb.su

.PHONY: clean-Core-2f-Src

