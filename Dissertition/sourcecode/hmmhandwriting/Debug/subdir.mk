################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../handwriting.c \
../quantilise.c 

CPP_SRCS += \
../Sample.cpp \
../countAverage.cpp \
../quantilise.cpp \
../testMath.cpp \
../testReadDirecotry.cpp \
../testString.cpp 

OBJS += \
./Sample.o \
./countAverage.o \
./handwriting.o \
./quantilise.o \
./testMath.o \
./testReadDirecotry.o \
./testString.o 

C_DEPS += \
./handwriting.d \
./quantilise.d 

CPP_DEPS += \
./Sample.d \
./countAverage.d \
./quantilise.d \
./testMath.d \
./testReadDirecotry.d \
./testString.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


