# Features
## Required
* +3 months battery life with 1500mAh LiPo battery

## Nice to have
* Bluetooth Low Energy (BLE) communication for settings and control
* Bluetooth connection should be established within 5 seconds

# Current consumption
t = 3 months = 90 days = 2160 hours
Q = 1000mAh (just to have some margin)
I = Q / t = 1000mAh / 2160h = 0.46mA

So, the current consumption should be less than 0.46mA.

# MCU Selection
## ESP32-C3
### Sleep modes
#### Option 1: Deep sleep
The device goes to deep sleep and regularly wakes up to advertise BLE.  The problem is that after deep sleep, the device needs to reboot.  Reboot takes a few seconds during which 10s of mA are being drawn.  It's not an option to reboot every 10s, that just consumes too much current.

#### Option 2: Light sleep
The device goes to light sleep and wakes up to advertise BLE.  The device doesn't need to reboot after light sleep.  The current consumption is higher than deep sleep, but it's still low enough for the battery life requirement.

Unfortunately, I haven't found any example code for light sleep with BLE advertising.  To get the most out of it (lowest current consumption), esp-idf instead of Arduino needs to be used.
An [external 32.768kHz crystal is needed for light sleep](https://docs.espressif.com/projects/espressif-esp-faq/en/latest/software-framework/ble-bt.html#how-does-esp32-bluetooth-le-enter-light-sleep-mode).  The [Bluetooth radio can wakeup the CPU from light sleep](https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32/api-reference/system/sleep_modes.html#_CPPv426esp_sleep_enable_bt_wakeupv).

It will be a lot of work for this nice-to-have feature.