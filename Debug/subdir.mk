################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CLHandler.cpp \
../EventHandler.cpp \
../Main.cpp \
../ParticleSystem.cpp \
../Renderer.cpp \
../utils.cpp 

OBJS += \
./CLHandler.o \
./EventHandler.o \
./Main.o \
./ParticleSystem.o \
./Renderer.o \
./utils.o 

CPP_DEPS += \
./CLHandler.d \
./EventHandler.d \
./Main.d \
./ParticleSystem.d \
./Renderer.d \
./utils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


