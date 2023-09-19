################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include appsTest/TMC_Test/subdir.mk
-include appsTest/SPIDMA_Test/subdir.mk
-include appsTest/LAN9252_Test/subdir.mk
-include appsTest/EthCat/subdir.mk
-include appsTest/CanIdle/subdir.mk
-include appsTest/subdir.mk
-include USB_Device/Target/subdir.mk
-include USB_Device/App/subdir.mk
-include Platform/tools/SystemView/SEGGER/Syscalls/subdir.mk
-include Platform/tools/SystemView/SEGGER/subdir.mk
-include Platform/tools/SystemView/FreeRTOSV10.4/Config/Cortex-M/subdir.mk
-include Platform/tools/SystemView/FreeRTOSV10.4/subdir.mk
-include Platform/modules/ethCat/subdir.mk
-include Platform/modules/canM/src/subdir.mk
-include Platform/modules/canIdle/subdir.mk
-include Platform/drivers/uartDma/subdir.mk
-include Platform/drivers/tmc/ramp/subdir.mk
-include Platform/drivers/tmc/ic/TMC8462/subdir.mk
-include Platform/drivers/tmc/helpers/subdir.mk
-include Platform/drivers/spiDma/subdir.mk
-include Platform/drivers/soes/hal/lan9252/subdir.mk
-include Platform/drivers/soes/subdir.mk
-include Platform/drivers/md80/subdir.mk
-include Platform/drivers/lwrb/subdir.mk
-include Platform/drivers/lan9252/subdir.mk
-include Platform/drivers/can_iso_tp/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Reflexor
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Reflexor.elf \

MAP_FILES += \
Reflexor.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Reflexor.list \

OBJCOPY_HEX += \
Reflexor.hex \

OBJCOPY_BIN += \
Reflexor.bin \


# All Target
all: main-build

# Main-build Target
main-build: Reflexor.elf secondary-outputs

# Tool invocations
Reflexor.elf Reflexor.map: $(OBJS) $(USER_OBJS) /home/mentee/dev/ec_slave/02_MTBOT_EthCat_Slave_Stm/Reflexor/STM32G474RBTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Reflexor.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/mentee/dev/ec_slave/02_MTBOT_EthCat_Slave_Stm/Reflexor/STM32G474RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Reflexor.map" -Wl,--cref -Wl,--gc-sections -static -L"/home/mentee/dev/ec_slave/02_MTBOT_EthCat_Slave_Stm/Reflexor/USB_Device/App" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Reflexor.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Reflexor.list"
	@echo 'Finished building: $@'
	@echo ' '

Reflexor.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Reflexor.hex"
	@echo 'Finished building: $@'
	@echo ' '

Reflexor.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Reflexor.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Reflexor.bin Reflexor.elf Reflexor.hex Reflexor.list Reflexor.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
