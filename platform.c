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

uint8_t RdByte(VL53L8CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t *p_value) {
    esp_err_t ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    //St25 Read sequence :
    i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, RegisterAdress >> 8, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, RegisterAdress & 0xFF, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_READ, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, p_value, NACK_VAL);

    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(p_platform->port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

uint8_t WrByte(VL53L8CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t value) {
    esp_err_t ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    //Write byte sequence :
    i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, RegisterAdress >> 8, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, RegisterAdress & 0xFF, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, value, ACK_CHECK_EN);

    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(p_platform->port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    //ESP_ERROR_CHECK(ret);
    return ret;
}

uint8_t WrMulti(VL53L8CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t *p_values, uint32_t size) {
    esp_err_t ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    //St25 Write sequence :
    i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, RegisterAdress >> 8, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, RegisterAdress & 0xFF, ACK_CHECK_EN);
    i2c_master_write(cmd, p_values, size, ACK_CHECK_EN);

    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(p_platform->port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

uint8_t RdMulti(VL53L8CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t *p_values, uint32_t size) {
    esp_err_t ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    //St25 Read sequence :
    i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, RegisterAdress >> 8, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, RegisterAdress & 0xFF, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (p_platform->address) | I2C_MASTER_READ, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, p_values, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, p_values + size - 1, NACK_VAL);

    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

uint8_t Reset_Sensor(VL53L8CX_Platform* p_platform)
{
    gpio_set_direction(p_platform->reset_gpio, GPIO_MODE_OUTPUT);

    gpio_set_level(p_platform->reset_gpio, 0);
    WaitMs(p_platform, 100);

    gpio_set_level(p_platform->reset_gpio, 1);
    WaitMs(p_platform, 100);

    return ESP_OK;
}

void SwapBuffer(uint8_t *buffer, uint16_t size) {
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

uint8_t WaitMs(VL53L8CX_Platform *p_platform, uint32_t TimeMs) {
    vTaskDelay(TimeMs / portTICK_PERIOD_MS);

    return ESP_OK;
}
