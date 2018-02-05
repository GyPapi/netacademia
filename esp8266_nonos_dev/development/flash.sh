sudo chmod 0666 /dev/ttyUSB0 && esptool.py -p /dev/ttyUSB0 -b 460800 write_flash --flash_mode qio --flash_size detect --verify --flash_freq 40m 0x00000 ../bin/eagle.flash.bin 0x10000 ../bin/eagle.irom0text.bin 0xFC000 ../bin/esp_init_data_default.bin 0x7e000 ../bin/blank.bin 0xfe000 ../bin/blank.bin

sudo gtkterm -p /dev/ttyUSB0 -s 115200

