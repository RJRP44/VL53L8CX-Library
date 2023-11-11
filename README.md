<h1 style="text-align: center;"> VL53L8CX library for ESP32  </h1>
<p style="text-align: center;">
<img src="https://seeklogo.com/images/E/espressif-systems-logo-1350B9E771-seeklogo.com.png" alt="Espressif logo" width="40">
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
<img src="https://upload.wikimedia.org/wikipedia/fr/e/e0/STMicroelectronics_%28logo-2020%29.svg" alt="ST logo" width="160">
</p>

A vl53l5cx library for esp32 using the **esp-idf framework**. This library is based
on [ST's  Ultra Lite Driver (ULD) for VL53L8CX](https://www.st.com/content/st_com/en/products/embedded-software/imaging-software/stsw-img040.html)
. This library is just an adaptation of the ST's library for esp-32.

> **Warning**
> This Library is **not** compatible with Arduino framework

## Contents 📌

* [Installation](#installation-)
  * [Wiring for the SATEL-VL53L8](#wiring-for-the-satel-VL53L8)
  * [Library Installation](#library-installation)
* [Examples](#examples-)
* [Usual Errors](#usual-errors-)
  * [Stack overflow error](#stack-overflow-error-)

---

## Installation 📥
### Wiring for the SATEL-VL53L8

For the examples this is the used wiring, but feel free to modify it and adapt it for your needs.

| SATEL-VL53L8 Pins | ESP32S3 Pins                                  |           
|-------------------|-----------------------------------------------|
| 5v                | 5v                                            |
| 1v8               | NC                                            |
| 3v3               | NC                                            |
| IOVDD             | NC                                            |
| PWREN             | 5V                                            |
| MCLK_SCL          | GPIO2, 2.2 kΩ pullup resistor required to 3v3 |
| MOSI_SDA          | GPIO1, 2.2 kΩ pullup resistor required to 3v3 |
| MISO              | NC                                            |
| NCS               | 3v3                                           |       
| SPI_I2C_N         | GND                                           |
| GPIO1             | NC                                            |
| GPIO2             | NC                                            |
| GND               | GND                                           |


### Library Installation

You can now use the [IDF Component Manager](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-component-manager.html) to easily import this library into your project ! If you have not already done so, create a file named `idf_component.yml` in your `📁 ./main` folder. Now just add the library to it :
```yaml
dependencies:
  
  #V53L8CX-Library
  vl53l8cx:
    git: https://github.com/RJRP44/V53L8CX-Library.git

```

You can now reload your cmake project, and you should see it in the cmake log and in the `📁 ./managed_components` folder.

```log
Processing 2 dependencies:
[1/2] idf (5.0.0)
[2/2] vl53l8cx
```

## Examples 📄

You can find in `📁 ./examples` ST's examples adapted for ESP32 with I²C.

## Usual errors 🐛
### Stack overflow error 💽

If by running an example you get a stack overflow error, this means that your main stack size is too small.
```log
***ERROR*** A stack overflow in task main has been detected.
```
**Increase the main stack size :**

Run `idf.py menuconfig`. Go to Component Config -> ESP System settings and increase the Main task stack size.

## 📝 License

Copyright © 2023 [RJRP](https://www.github.com/RJRP44).

This project is [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause/)  licensed.

## ✨ Show your support

Give a ⭐️ if this project helped you!

## 👤 Authors

- [@RJRP](https://www.github.com/RJRP44)
