# cat-repeller
Detect cat with PIR sensor and scare them off using ultrasound.  Settings and control through Web Bluetooth

## Operation
* Solar panel used as light sensor to detect day/night.  
* PIR sensor detects motion.  

### LEDs
* During daylight, the white LEDs will be disabled because they have little effect and consume power.
* The red LED will signal the device is active using short blinks.

## Electronics
### CATPIR
PCB holding PIR-sensor and piezo buzzer.

* [CATPIR 1.0.0 : PCB source](https://365.altium.com/files/FE5699FE-DB18-49EB-B4F2-E8AD16447050)

## Firmware 
### Piezo control
* Down-chirp signal to scare off cats.  Due to tolerances not every piezo will resonate at the same frequency.  The down-chirp will cover a range of frequencies.  The down-chirp is used in alarm systems as well.  My personal opinion is that a down-chirp relates more to danger than an up-chirp or a constant tone.


https://randomnerdtutorials.com/esp32-web-bluetooth/
https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/


https://microcontrollerslab.com/esp32-ble-server-gatt-service-battery-level-indication/
https://circuitdigest.com/microcontroller-projects/esp32-based-bluetooth-ibeacon


