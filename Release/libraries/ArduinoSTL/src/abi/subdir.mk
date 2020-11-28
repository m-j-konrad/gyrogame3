################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
D:\devel\Sloeber\arduinoPlugin\libraries\ArduinoSTL\1.1.0\src\abi\abi.cpp 

LINK_OBJ += \
.\libraries\ArduinoSTL\src\abi\abi.cpp.o 

CPP_DEPS += \
.\libraries\ArduinoSTL\src\abi\abi.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\ArduinoSTL\src\abi\abi.cpp.o: D:\devel\Sloeber\arduinoPlugin\libraries\ArduinoSTL\1.1.0\src\abi\abi.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\devel\Sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"D:\devel\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.3\cores\arduino" -I"D:\devel\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.3\variants\mega" -I"C:\Users\mjk\Documents\Arduino\libraries\Adafruit_GFX_Library" -I"C:\Users\mjk\Documents\Arduino\libraries\SD\src" -I"D:\devel\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.3\libraries\SPI\src" -I"C:\Users\mjk\Documents\Arduino\libraries\Waveshare_4_Inch_Tft_Touchscreen\src" -I"C:\Users\mjk\Documents\Arduino\libraries\Adafruit_BusIO" -I"D:\devel\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.3\libraries\Wire\src" -I"D:\devel\Sloeber\arduinoPlugin\libraries\ArduinoSTL\1.1.0\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"   -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


