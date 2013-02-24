.PHONY: build

build:
	@echo "Uploading ./build/bt_read_write.hex..."
	"avrdude" "-CC:/arduino-1.0.3/hardware/tools/avr/etc/avrdude.conf" -v -v -v -v -V -patmega328p -carduino -PCOM4 -b57600 -D "-Uflash:w:./build/bt_read_write.hex:i"
	
