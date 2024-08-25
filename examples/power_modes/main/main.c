/*******************************************************************************
* Copyright (c) 2020, STMicroelectronics - All Rights Reserved
*
* This file is part of the VL53L8CX Ultra Lite Driver and is dual licensed,
* either 'STMicroelectronics Proprietary license'
* or 'BSD 3-clause "New" or "Revised" License' , at your option.
*
********************************************************************************
*
* 'STMicroelectronics Proprietary license'
*
********************************************************************************
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms at www.st.com/sla0081
*
* STMicroelectronics confidential
* Reproduction and Communication of this document is strictly prohibited unless
* specifically authorized in writing by STMicroelectronics.
*
*
********************************************************************************
*
* Alternatively, the VL53L8CX Ultra Lite Driver may be distributed under the
* terms of 'BSD 3-clause "New" or "Revised" License', in which case the
* following provisions apply instead of the ones mentioned above :
*
********************************************************************************
*
* License terms: BSD 3-clause "New" or "Revised" License.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. Neither the name of the copyright holder nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*
*******************************************************************************/

/***********************************/
/*    VL53L8CX ULD power mode      */
/***********************************/
/*
* This example shows the possibility of VL53L8CX to change power mode. It
* initializes the VL53L8CX ULD, set a configuration, change the power mode, and
* starts a ranging to capture 10 frames.
*
* In this example, we also suppose that the number of target per zone is
* set to 1 , and all output are enabled (see file platform.h).
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vl53l8cx_api.h"

void app_main(void)
{

    //Define the i2c bus configuration
    i2c_port_t i2c_port = I2C_NUM_1;
    i2c_master_bus_config_t i2c_mst_config = {
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .i2c_port = i2c_port,
            .scl_io_num = 2,
            .sda_io_num = 1,
            .glitch_ignore_cnt = 7,
            .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    //Define the i2c device configuration
    i2c_device_config_t dev_cfg = {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = VL53L8CX_DEFAULT_I2C_ADDRESS >> 1,
            .scl_speed_hz = VL53L8CX_MAX_CLK_SPEED,
    };

    /*********************************/
    /*   VL53L8CX ranging variables  */
    /*********************************/

    uint8_t 				status, loop, isAlive, isReady, i;
    VL53L8CX_Configuration 	Dev;			/* Sensor configuration */
    VL53L8CX_ResultsData 	Results;		/* Results data from VL53L8CX */


    /*********************************/
    /*      Customer platform        */
    /*********************************/

    Dev.platform.bus_config = i2c_mst_config;

    //Register the device
    i2c_master_bus_add_device(bus_handle, &dev_cfg, &Dev.platform.handle);

    /* (Optional) Reset sensor */
    Dev.platform.reset_gpio = GPIO_NUM_5;
    VL53L8CX_Reset_Sensor(&(Dev.platform));

    /* (Optional) Set a new I2C address if the wanted address is different
    * from the default one (filled with 0x20 for this example).
    */
    //status = vl53l8cx_set_i2c_address(&Dev, 0x20);


    /*********************************/
    /*   Power on sensor and init    */
    /*********************************/

    /* (Optional) Check if there is a VL53L8CX sensor connected */
    status = vl53l8cx_is_alive(&Dev, &isAlive);
    if(!isAlive || status)
    {
        printf("VL53L8CX not detected at requested address\n");
        return;
    }

    /* (Mandatory) Init VL53L8CX sensor */
    status = vl53l8cx_init(&Dev);
    if(status)
    {
        printf("VL53L8CX ULD Loading failed\n");
        return;
    }

    printf("VL53L8CX ULD ready ! (Version : %s)\n",
           VL53L8CX_API_REVISION);

    /*********************************/
    /*     Change the power mode     */
    /*********************************/

    /* For the example, we don't want to use the sensor during 10 seconds. In order to reduce
     * the power consumption, the sensor is set to low power mode.
     */
    status = vl53l8cx_set_power_mode(&Dev, VL53L8CX_POWER_MODE_SLEEP);
    if(status)
    {
        printf("vl53l8cx_set_power_mode failed, status %u\n", status);
        return;
    }
    printf("VL53L8CX is now sleeping\n");

    /* We wait 5 seconds, only for the example */
    printf("Waiting 5 seconds for the example...\n");
    VL53L8CX_WaitMs(&(Dev.platform), 5000);

    /* After 5 seconds, the sensor needs to be restarted */
    status = vl53l8cx_set_power_mode(&Dev, VL53L8CX_POWER_MODE_WAKEUP);
    if(status)
    {
        printf("vl53l8cx_set_power_mode failed, status %u\n", status);
        return;
    }
    printf("VL53L8CX is now waking up\n");

    /*********************************/
    /*         Ranging loop          */
    /*********************************/

    status = vl53l8cx_start_ranging(&Dev);

    loop = 0;
    while(loop < 10)
    {
        /* Use polling function to know when a new measurement is ready.
         * Another way can be to wait for HW interrupt raised on PIN A1
         * (INT) when a new measurement is ready */

        status = vl53l8cx_check_data_ready(&Dev, &isReady);

        if(isReady)
        {
            vl53l8cx_get_ranging_data(&Dev, &Results);

            /* As the sensor is set in 4x4 mode by default, we have a total
             * of 16 zones to print. For this example, only the data of first zone are
             * print */
            printf("Print data no : %3u\n", Dev.streamcount);
            for(i = 0; i < 16; i++)
            {
                printf("Zone : %3d, Status : %3u, Distance : %4d mm\n",
                       i,
                       Results.target_status[VL53L8CX_NB_TARGET_PER_ZONE*i],
                       Results.distance_mm[VL53L8CX_NB_TARGET_PER_ZONE*i]);
            }
            printf("\n");
            loop++;
        }

        /* Wait a few ms to avoid too high polling (function in platform
         * file, not in API) */
        WaitMs(&(Dev.platform), 5);
    }

    status = vl53l8cx_stop_ranging(&Dev);
    printf("End of ULD demo\n");
    return;
}
