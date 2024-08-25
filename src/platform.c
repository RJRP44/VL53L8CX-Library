/**
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


#include "platform.h"

//A buffer used to add index for the vl53l8cx data format
uint8_t i2c_buffer[I2C_NUM_MAX][0x8002];


uint8_t VL53L8CX_WrMulti(VL53L8CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t *p_values, uint32_t size) {

    //Select the correct buffer
    i2c_port_num_t i2c_port = p_platform->bus_config.i2c_port;

    //Add index to the data
    i2c_buffer[i2c_port][0] = RegisterAdress >> 8;
    i2c_buffer[i2c_port][1] = RegisterAdress & 0xFF;

    //Add the data
    memcpy(&i2c_buffer[i2c_port][2], p_values, size);

    return i2c_master_transmit(p_platform->handle, i2c_buffer[i2c_port], size + 2, -1);
}

uint8_t VL53L8CX_WrByte(VL53L8CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t value) {

    //Write a single byte
    return VL53L8CX_WrMulti(p_platform, RegisterAdress, &value, 1);
}

uint8_t VL53L8CX_RdMulti(VL53L8CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t *p_values, uint32_t size) {

    //Select the correct buffer
    i2c_port_num_t i2c_port = p_platform->bus_config.i2c_port;

    //Add index to the data
    i2c_buffer[i2c_port][0] = RegisterAdress >> 8;
    i2c_buffer[i2c_port][1] = RegisterAdress & 0xFF;

    return i2c_master_transmit_receive(p_platform->handle, i2c_buffer[i2c_port], 2, p_values, size, -1);
}

uint8_t VL53L8CX_RdByte(VL53L8CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t *p_value) {

    //Read a single byte
    return VL53L8CX_RdMulti(p_platform, RegisterAdress, p_value, 1);
}

uint8_t VL53L8CX_Reset_Sensor(VL53L8CX_Platform* p_platform)
{
    gpio_set_direction(p_platform->reset_gpio, GPIO_MODE_OUTPUT);

    gpio_set_level(p_platform->reset_gpio, 0);
    VL53L8CX_WaitMs(p_platform, 100);

    gpio_set_level(p_platform->reset_gpio, 1);
    VL53L8CX_WaitMs(p_platform, 100);

    return ESP_OK;
}

void VL53L8CX_SwapBuffer(uint8_t *buffer, uint16_t size) {
    uint32_t i;
    uint8_t tmp[4] = {0};

    for (i = 0; i < size; i = i + 4) {

        tmp[0] = buffer[i + 3];
        tmp[1] = buffer[i + 2];
        tmp[2] = buffer[i + 1];
        tmp[3] = buffer[i];

        memcpy(&(buffer[i]), tmp, 4);
    }
}

uint8_t VL53L8CX_WaitMs(VL53L8CX_Platform *p_platform, uint32_t TimeMs) {
    vTaskDelay(TimeMs / portTICK_PERIOD_MS);

    return ESP_OK;
}
