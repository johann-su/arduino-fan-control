BOARD_TAG = nano
MCU = atmega328p
F_CPU = 16000000L
PORT = /dev/ttyUSB0
USER_LIB_PATH = /home/johann/Arduino/libraries
OBJDIR = build

TARGET = main.cpp
ARDUINO_LIBS = SPI Wire Adafruit_BusIO Adafruit_GFX_Library Adafruit_SSD1306 Adafruit_Unified_Sensor DHT_sensor_library PWM TimerOne

include /usr/share/arduino/Arduino.mk

all: flash monitor

flash: $(OBJDIR)/$(TARGET).hex
	@echo "Flashing..."
	arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano:cpu=atmega328 -i ${OBJDIR}/main.cpp.hex

monitor:
	arduino-cli monitor -p /dev/ttyUSB0 -b arduino:avr:nano:cpu=atmega328 