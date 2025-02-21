# Arduino Fan Controller

### Compile

```bash
avr-gcc -mmcu=atmega328p -DF_CPU=16000000 -o main.elf main.c
avr-objcopy -O ihex -R .eeprom -j .text -j .data main.elf main.hex
```

### Environent

- [AVR Toolchain setup wsl2](https://www.tonymitchell.ca/posts/setup-avr-toolchain-on-wsl/#option-1-using-usbip)
- [Attach usb device to wsl2](https://learn.microsoft.com/windows/wsl/connect-usb#attach-a-usb-device)
- Install `sudo apt install arduino arduino-core-avr arduino-mk avrdude`

### Start Serial Monitor

```bash
sudo screen /dev/ttyUSB0 115200
```

### Flashing

```bash
usbipd list
usbipd attach --wsl --busid <id>
```

```bash
# cd build-nano
# sudo avrdude -c arduino -p m328p -U flash:w:main.cpp.hex:i -P /dev/ttyUSB0 -b 115200
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano:cpu=atmega328 -i build-nano/main.cpp.hex
arduino-cli monitor -p /dev/ttyUSB0 -b arduino:avr:nano:cpu=atmega328
```
