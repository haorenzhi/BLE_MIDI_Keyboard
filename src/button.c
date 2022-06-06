/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>
#include <string.h>
#include "button.h"

// define callback handler named button_pressed
void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

void button_check(void){
	int ret[7];
        
    /* button available check */
    for(int i = 0; i < 7; i++) {
        if (!device_is_ready(buttons[i].port)) {
            printk("Error: button device %s is not ready\n",
                   buttons[i].port->name);
            return;
        }
    }
        
    /* adding extra flags : GPIO_INPUT */
    for(int i = 0; i < 7; i++) {
        ret[i] = gpio_pin_configure_dt(&buttons[i], GPIO_INPUT);

        if (ret[i] != 0) {
            printk("Error %d: failed to configure %s pin %d\n",
                   ret[i], buttons[i].port->name, buttons[i].pin);
            return;
        }
    }
        
    /* adding extra flags : GPIO_INT_EDGE_TO_ACTIVE */
    /* button port and pin available check */
    for(int i = 0; i < 7; i++) {
        ret[i] = gpio_pin_interrupt_configure_dt(&buttons[i], GPIO_INT_EDGE_TO_ACTIVE);

        if (ret[i] != 0) {
            printk("Error %d: failed to configure interrupt on %s pin %d\n",
                   ret[i], buttons[i].port->name, buttons[i].pin);
            return;
        }
    }
        
    /* assign callback handler to gpio_callback */
    for(int i = 0; i < 7; i++) {
        gpio_init_callback(&button_cb_data[i], button_pressed, BIT(buttons[i].pin));
    }

    /* add callback to port*/
    for(int i = 0; i < 7; i++) {
        gpio_add_callback(buttons[i].port, &button_cb_data[i]);
        printk("Set up button at %s pin %d\n", buttons[i].port->name, buttons[i].pin);
    }

	printk("Press the button\n");
}

unsigned char[] button_loop(void){
 /* If we have an LED, match its state to the button's. */

        int[] val = new int[7];
        int count = 0;
        for(int i = 0; i < 7; i++) {
            val[i] = gpio_pin_get_dt(&buttons[i]);
            if(val[i] == 1) {
                count++;
            }
        }

        k_msleep(SLEEP_TIME_MS); // sleep 3s for reading the input data

        int num;
        if(count == 1) {
            num = 3 + 2 * count;
        }else {
            num = 3 + 2 * count + 2;
        }
        unsigned char[] message = new unsigned char[num];

        // header
        message[0] = 0x80;
        // timestamp
        message[1] = 0x80;
        // status
        message[2] = 0x90;
        if(count > 1) {
            // second header
            message[5] = 0x80;
            // second timestamp
            message[6] = 0x90;
        }

        int idx;
        for(idx = 0; idx < val.length; idx++) {
            unsigned char chord;
            if(val[idx] != 0) {
                switch (idx) {
                    case 0:
                        chord = 0x3C;
                        break;
                    case 1:
                        chord = 0x3E;
                        break;
                    case 2:
                        chord = 0x40;
                        break;
                    case 3:
                        chord = 0x41;
                        break;
                    case 4:
                        chord = 0x43;
                        break;
                    case 5:
                        chord = 0x45;
                        break;
                    case 6:
                        chord = 0x47;
                        break;
                }

                message[3] = chord;
                message[4] = 0x7F;
                break;
            }
        }

        int j = 7;
        for(idx; idx < val.length; idx++) {
            unsigned char chord;
            if(val[idx] != 0) {
                switch (idx) {
                    case 0:
                        chord = 0x3C;
                        break;
                    case 1:
                        chord = 0x3E;
                        break;
                    case 2:
                        chord = 0x40;
                        break;
                    case 3:
                        chord = 0x41;
                        break;
                    case 4:
                        chord = 0x43;
                        break;
                    case 5:
                        chord = 0x45;
                        break;
                    case 6:
                        chord = 0x47;
                        break;
                }

                message[j] = chord;
                message[j+1] = 0x7F;
                j += 2;
            }
        }

        /* do something here
         * wrap up val array into MIDI data
         * sending the MIDI data using BLE
         * */

        /* set the val array and button value to zero (unpressed) */
        for(int i = 0; i < 7; i++) {
            if(val[i] > 0) {
                val[i] = 0;
                gpio_pin_set_dt(&buttons[i], 0);
            }
        }

        return message;
}