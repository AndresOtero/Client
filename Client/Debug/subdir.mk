################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Client.cpp \
../Interprete.cpp \
../MySocket.cpp \
../Yaml.cpp \
../mensaje.cpp 

OBJS += \
./Client.o \
./Interprete.o \
./MySocket.o \
./Yaml.o \
./mensaje.o 

CPP_DEPS += \
./Client.d \
./Interprete.d \
./MySocket.d \
./Yaml.d \
./mensaje.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/andres/git/AgeOfEmpiresTaller/AgeOfEmpiresTaller/libs/plog-master/include" -I"/home/andres/git/AgeOfEmpiresTaller/AgeOfEmpiresTaller" -I"/home/andres/git/AgeOfEmpiresTaller/AgeOfEmpiresTaller/Debug" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


