################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:\devel\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.3\libraries\Wire\src\utility\twi.c 

C_DEPS += \
.\libraries\Wire\src\utility\twi.c.d 

LINK_OBJ += \
.\libraries\Wire\src\utility\twi.c.o 


# Each subdirectory must supply rules for building sources it contributes
libraries\Wire\src\utility\twi.c.o: D:\devel\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.3\libraries\Wire\src\utility\twi.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"D:\devel\Sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7/bin/avr-gcc" -c -g -Os -Wall -Wextra -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"D:\devel\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.3\cores\arduino" -I"D:\devel\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.3\variants\mega" -I"C:\Users\mjk\Documents\Arduino\libraries\Adafruit_GFX_Library" -I"C:\Users\mjk\Documents\Arduino\libraries\SD\src" -I"D:\devel\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.3\libraries\SPI\src" -I"C:\Users\mjk\Documents\Arduino\libraries\Waveshare_4_Inch_Tft_Touchscreen\src" -I"C:\Users\mjk\Documents\Arduino\libraries\Adafruit_BusIO" -I"D:\devel\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.3\libraries\Wire\src" -I"D:\devel\Sloeber\arduinoPlugin\libraries\ArduinoSTL\1.1.0\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 "$<"   -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


