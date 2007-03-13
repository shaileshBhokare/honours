################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../backup/20070228/quantilise.c 

OBJS += \
./backup/20070228/quantilise.o 

C_DEPS += \
./backup/20070228/quantilise.d 


# Each subdirectory must supply rules for building sources it contributes
backup/20070228/%.o: ../backup/20070228/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


