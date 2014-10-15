################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DataBaseUpdateThread.cpp \
../ExtractedImage.cpp \
../ExtractionManager.cpp \
../ExtractionManagerStats.cpp \
../ExtractionParms.cpp \
../FrameExtractorThread.cpp \
../FrameProcessorThread.cpp \
../ImageExtractionThread.cpp \
../LogicalFrame.cpp \
../LogicalFrameBlob.cpp \
../LogicalFrameQueue.cpp \
../SipperImage.cpp \
../SipperThread.cpp 

OBJS += \
./DataBaseUpdateThread.o \
./ExtractedImage.o \
./ExtractionManager.o \
./ExtractionManagerStats.o \
./ExtractionParms.o \
./FrameExtractorThread.o \
./FrameProcessorThread.o \
./ImageExtractionThread.o \
./LogicalFrame.o \
./LogicalFrameBlob.o \
./LogicalFrameQueue.o \
./SipperImage.o \
./SipperThread.o 

CPP_DEPS += \
./DataBaseUpdateThread.d \
./ExtractedImage.d \
./ExtractionManager.d \
./ExtractionManagerStats.d \
./ExtractionParms.d \
./FrameExtractorThread.d \
./FrameProcessorThread.d \
./ImageExtractionThread.d \
./LogicalFrame.d \
./LogicalFrameBlob.d \
./LogicalFrameQueue.d \
./SipperImage.d \
./SipperThread.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/media/Data/Users/kkramer/IEC/SipperSoftware/BaseLibrary" -I"/media/Data/Users/kkramer/IEC/SipperSoftware/SipperInstruments" -I"/media/Data/Users/kkramer/IEC/SipperSoftware/PicesLibrary" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


