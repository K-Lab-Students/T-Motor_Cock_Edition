################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/freertos-can-driver-master/freertos-can-driver.c 

OBJS += \
./Middlewares/freertos-can-driver-master/freertos-can-driver.o 

C_DEPS += \
./Middlewares/freertos-can-driver-master/freertos-can-driver.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/freertos-can-driver-master/%.o Middlewares/freertos-can-driver-master/%.su Middlewares/freertos-can-driver-master/%.cyclo: ../Middlewares/freertos-can-driver-master/%.c Middlewares/freertos-can-driver-master/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/NakedSnake/Documents/FreeRTOSina_CAN/Middlewares/freertos-can-driver-master" -I"C:/Users/NakedSnake/Documents/FreeRTOSina_CAN/Middlewares/myactuator-rmd-can-develop" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-freertos-2d-can-2d-driver-2d-master

clean-Middlewares-2f-freertos-2d-can-2d-driver-2d-master:
	-$(RM) ./Middlewares/freertos-can-driver-master/freertos-can-driver.cyclo ./Middlewares/freertos-can-driver-master/freertos-can-driver.d ./Middlewares/freertos-can-driver-master/freertos-can-driver.o ./Middlewares/freertos-can-driver-master/freertos-can-driver.su

.PHONY: clean-Middlewares-2f-freertos-2d-can-2d-driver-2d-master

