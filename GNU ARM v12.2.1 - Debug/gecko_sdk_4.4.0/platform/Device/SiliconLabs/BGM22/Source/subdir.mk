################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/aatir/gecko_sdk/platform/Device/SiliconLabs/BGM22/Source/startup_bgm22.c \
/home/aatir/gecko_sdk/platform/Device/SiliconLabs/BGM22/Source/system_bgm22.c 

OBJS += \
./gecko_sdk_4.4.0/platform/Device/SiliconLabs/BGM22/Source/startup_bgm22.o \
./gecko_sdk_4.4.0/platform/Device/SiliconLabs/BGM22/Source/system_bgm22.o 

C_DEPS += \
./gecko_sdk_4.4.0/platform/Device/SiliconLabs/BGM22/Source/startup_bgm22.d \
./gecko_sdk_4.4.0/platform/Device/SiliconLabs/BGM22/Source/system_bgm22.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_4.4.0/platform/Device/SiliconLabs/BGM22/Source/startup_bgm22.o: /home/aatir/gecko_sdk/platform/Device/SiliconLabs/BGM22/Source/startup_bgm22.c gecko_sdk_4.4.0/platform/Device/SiliconLabs/BGM22/Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DDEBUG=1' '-DDEBUG_EFM=1' '-DBGM220PC22HNA=1' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DSL_BOARD_NAME="BRD4314A"' '-DSL_BOARD_REV="A02"' '-DSL_COMPONENT_CATALOG_PRESENT=1' -I"/home/aatir/SimplicityStudio/v5_workspace/lola/config" -I"/home/aatir/gecko_sdk//platform/service/device_init/inc" -I"/home/aatir/gecko_sdk//platform/emdrv/common/inc" -I"/home/aatir/gecko_sdk//hardware/kit/BGM22_BRD4310A/config" -I"/home/aatir/gecko_sdk//hardware/kit/common/bsp" -I"/home/aatir/SimplicityStudio/v5_workspace/lola/autogen" -I"/home/aatir/SimplicityStudio/v5_workspace/lola" -I"/home/aatir/gecko_sdk//platform/Device/SiliconLabs/BGM22/Include" -I"/home/aatir/gecko_sdk//hardware/board/inc" -I"/home/aatir/gecko_sdk//platform/CMSIS/Core/Include" -I"/home/aatir/gecko_sdk//platform/service/device_init/inc" -I"/home/aatir/gecko_sdk//platform/emlib/inc" -I"/home/aatir/gecko_sdk//platform/emlib/src" -I"/home/aatir/gecko_sdk//platform/common/inc" -I"/home/aatir/gecko_sdk//platform/common/toolchain/inc" -I"/home/aatir/gecko_sdk//platform/service/system/inc" -Os -Wall -Wextra -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.4.0/platform/Device/SiliconLabs/BGM22/Source/startup_bgm22.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gecko_sdk_4.4.0/platform/Device/SiliconLabs/BGM22/Source/system_bgm22.o: /home/aatir/gecko_sdk/platform/Device/SiliconLabs/BGM22/Source/system_bgm22.c gecko_sdk_4.4.0/platform/Device/SiliconLabs/BGM22/Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DDEBUG=1' '-DDEBUG_EFM=1' '-DBGM220PC22HNA=1' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DSL_BOARD_NAME="BRD4314A"' '-DSL_BOARD_REV="A02"' '-DSL_COMPONENT_CATALOG_PRESENT=1' -I"/home/aatir/SimplicityStudio/v5_workspace/lola/config" -I"/home/aatir/gecko_sdk//platform/service/device_init/inc" -I"/home/aatir/gecko_sdk//platform/emdrv/common/inc" -I"/home/aatir/gecko_sdk//hardware/kit/BGM22_BRD4310A/config" -I"/home/aatir/gecko_sdk//hardware/kit/common/bsp" -I"/home/aatir/SimplicityStudio/v5_workspace/lola/autogen" -I"/home/aatir/SimplicityStudio/v5_workspace/lola" -I"/home/aatir/gecko_sdk//platform/Device/SiliconLabs/BGM22/Include" -I"/home/aatir/gecko_sdk//hardware/board/inc" -I"/home/aatir/gecko_sdk//platform/CMSIS/Core/Include" -I"/home/aatir/gecko_sdk//platform/service/device_init/inc" -I"/home/aatir/gecko_sdk//platform/emlib/inc" -I"/home/aatir/gecko_sdk//platform/emlib/src" -I"/home/aatir/gecko_sdk//platform/common/inc" -I"/home/aatir/gecko_sdk//platform/common/toolchain/inc" -I"/home/aatir/gecko_sdk//platform/service/system/inc" -Os -Wall -Wextra -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.4.0/platform/Device/SiliconLabs/BGM22/Source/system_bgm22.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


