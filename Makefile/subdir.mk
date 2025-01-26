################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../button.c \
../buzzer.c \
../gpio.c \
../led.c \
../seven_segment_display.c \
../stopWatch_timer_project.c \
../timer.c 

OBJS += \
./button.o \
./buzzer.o \
./gpio.o \
./led.o \
./seven_segment_display.o \
./stopWatch_timer_project.o \
./timer.o 

C_DEPS += \
./button.d \
./buzzer.d \
./gpio.d \
./led.d \
./seven_segment_display.d \
./stopWatch_timer_project.d \
./timer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


