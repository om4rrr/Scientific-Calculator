################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/Seven_Segment/Seven_Segment_TimeLinking/Seven_Segmen_config.c \
../HAL/Seven_Segment/Seven_Segment_TimeLinking/Seven_Segment_prog.c 

OBJS += \
./HAL/Seven_Segment/Seven_Segment_TimeLinking/Seven_Segmen_config.o \
./HAL/Seven_Segment/Seven_Segment_TimeLinking/Seven_Segment_prog.o 

C_DEPS += \
./HAL/Seven_Segment/Seven_Segment_TimeLinking/Seven_Segmen_config.d \
./HAL/Seven_Segment/Seven_Segment_TimeLinking/Seven_Segment_prog.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/Seven_Segment/Seven_Segment_TimeLinking/%.o: ../HAL/Seven_Segment/Seven_Segment_TimeLinking/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


