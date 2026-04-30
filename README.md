# VL53L8CX library for ESP32

[![Component Registry](https://components.espressif.com/components/rjrp44/vl53l8cx/badge.svg)](https://components.espressif.com/components/rjrp44/vl53l8cx)
[![Uld Version](https://img.shields.io/badge/uld_version-2.0.0-white)](https://www.st.com/content/st_com/en/products/embedded-software/imaging-software/stsw-img040.html)

A vl53l5cx library for esp32 using the **esp-idf framework**. This library is based
on [ST's  Ultra Lite Driver (ULD) for VL53L8CX](https://www.st.com/content/st_com/en/products/embedded-software/imaging-software/stsw-img040.html)
v.2.0.0. This library is just an adaptation of the ST's library for esp-32.

> **Warning**
> This Library is **not** compatible with Arduino framework

## Contents 📌

* [Getting started](#Getting-started)
  * [Requirements](#requirements)
  * [Library Installation](#library-installation-)
  * [Wiring for the SATEL-VL53L8](#wiring-for-the-satel-VL53L8)
* [Examples](#examples-)
* [Usual Errors](#usual-errors-)
  * [Stack overflow error](#stack-overflow-error-)
* [More parameters](#more-parameters-)
---

## Getting started

### Requirements

This version of the library uses the esp-idf v6 for performance improvements.

### Library Installation 📥

The stable version of this library is available at https://components.espressif.com/components/rjrp44/vl53l8cx.

So, you can use the [IDF Component Manager](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-component-manager.html) to easily import this library into your project.
To add this component to your project, run:

```log
idf.py add-dependency "rjrp44/vl53l8cx" 
```

### Wiring for the SATEL-VL53L8

For the examples this is the used wiring, but feel free to modify it and adapt it for your needs.

| SATEL-VL53L8 Pins | ESP32S3 Pins                                                                                                                     |           
|-------------------|----------------------------------------------------------------------------------------------------------------------------------|
| 5v                | 5v                                                                                                                               |
| 1v8               | NC                                                                                                                               |
| 3v3               | NC                                                                                                                               |
| IOVDD             | NC                                                                                                                               |
| PWREN             | GPIO5, 10 kΩ pullup resistor required to 3v3 if you want to use the `Reset_Sensor()` function, otherwise connect it to 3v3 or 5v |
| MCLK_SCL          | GPIO2, 2.2 kΩ pullup resistor required to 3v3                                                                                    |
| MOSI_SDA          | GPIO1, 2.2 kΩ pullup resistor required to 3v3                                                                                    |
| MISO              | NC                                                                                                                               |
| NCS               | 3v3                                                                                                                              |       
| SPI_I2C_N         | GND                                                                                                                              |
| GPIO1             | NC                                                                                                                               |
| GPIO2             | NC                                                                                                                               |
| GND               | GND                                                                                                                              |

## Examples 📄

You can find in `📁 ./examples` ST's examples adapted for ESP32 with I²C. For more information about the available features read [UM3109](https://www.st.com/resource/en/user_manual/um3109-a-guide-for-using-the-vl53l8cx-lowpower-highperformance-timeofflight-multizone-ranging-sensor-stmicroelectronics.pdf).

## Usual errors 🐛
### Stack overflow error 💽

If by running an example you get a stack overflow error, this means that your main stack size is too small.
```log
***ERROR*** A stack overflow in task main has been detected.
```
**Increase the main stack size :**

Run `idf.py menuconfig`. Go to Component `Component config -> ESP System settings` and increase the Main task stack size to at least `7168`.


## More parameters ⚙
In order to get more control over your sensor configuration, run `idf.py menuconfig` and go to `Component Config -> Vl53l8cx`.

In those settings you can change the number of target per zone, the timeout value, and you can disable selected output, in order to reduce I2C access.
## 📝 License

Copyright © 2026 [RJRP](https://www.github.com/RJRP44).

This project is [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause/)  licensed.

## ✨ Show your support

Give a ⭐️ if this project helped you!

## 👤 Authors

- [@RJRP](https://www.github.com/RJRP44)
