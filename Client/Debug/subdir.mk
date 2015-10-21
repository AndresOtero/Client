################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Client.cpp \
../GameControllerClient.cpp \
../Interprete.cpp \
../MySocket.cpp 

OBJS += \
./Client.o \
./GameControllerClient.o \
./Interprete.o \
./MySocket.o 

CPP_DEPS += \
./Client.d \
./GameControllerClient.d \
./Interprete.d \
./MySocket.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


