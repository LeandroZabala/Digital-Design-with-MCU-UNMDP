################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/Components/lis302dl/lis302dl.c 

OBJS += \
./Utilities/Components/lis302dl/lis302dl.o 

C_DEPS += \
./Utilities/Components/lis302dl/lis302dl.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Components/lis302dl/lis302dl.o: ../Utilities/Components/lis302dl/lis302dl.c Utilities/Components/lis302dl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F407xx -DUSE_HAL_DRIVER -DDEBUG -c -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/ili9325" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/s25fl512s" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/cs43l22" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/ili9341" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/ampire480272" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/n25q512a" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/s5k5cag" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/mfxstm32l152" -I"C:/DDCM/stm32f407g-disc1_hal_lib/CMSIS/device" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/n25q128a" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/ts3510" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/st7735" -I"C:/DDCM/stm32f407g-disc1_hal_lib/HAL_Driver/Inc/Legacy" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/lis302dl" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/otm8009a" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/stmpe1600" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/Common" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/ov2640" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/l3gd20" -I"C:/DDCM/stm32f407g-disc1_hal_lib/HAL_Driver/Inc" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/stmpe811" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/lis3dsh" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/wm8994" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/n25q256a" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/ls016b8uy" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/ft6x06" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/STM32F4-Discovery" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/exc7200" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/st7789h2" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/ampire640480" -I"C:/DDCM/stm32f407g-disc1_hal_lib/Utilities/Components/lsm303dlhc" -I"C:/DDCM/stm32f407g-disc1_hal_lib/CMSIS/core" -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"Utilities/Components/lis302dl/lis302dl.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

