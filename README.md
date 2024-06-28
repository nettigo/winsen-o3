# Winsen O3 sensor

This is simple library for reading data from ZE25a and MZ27 O3 sensors from Winsen.

* https://nettigo.eu/products/ozon-o3-sensor-winsen-ze25a


This is first MVP version:

* keep in mind it works only in active mode (reads stream of incoming frames from sensor)
* each frame is read and reading is stored as sum (unsigned long) Make sure that measurement interval is not too long and 
 overflow does not happen

``
#include "winsen-o3.h

WinsenO3 sensor;

void setup() {
  sensor.begin(RX_PIN, TX_PIN);
}
``

In `loop` call `sensor.process()` it will read and parse data. Then You can check if any data is available (`sensor.available()`) and 
get average of readings via `sensor.currentValue()` and to know how many frames were 
successfully parsed via `sensor.sampleCount()`. Use `sensor.restart()` to clear counter and start new samples.
