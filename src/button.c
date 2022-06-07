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
unsigned char messages[5] = {0x80, 0x80, 0x90, 0x3C, 0x7F};
unsigned char chord;

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    printk("Button pressed at pin %" PRIu32 "\n", pins);

    switch(pins) {
        case 2048:
            chord = 0x3C;
            break;
        case 4096:
            chord = 0x3E;
            break;
        case 8192:
            chord = 0x40;
            break;
        case 16387:
            chord = 0x41;
            break;
        case 32768:
            chord = 0x43;
            break;
        case 16777216:
            chord = 0x45;
            break;
        case 33554432:
            chord = 0x47;
            break;
        default:
            chord = 0x3C;
            break;
    }

    messages[3] = chord;

    for(int i = 0; i < 7; i++) {
        gpio_pin_set_dt(&buttons[i], 0);
    }

    printk("\nchord = 0x%02X\n", (unsigned int)(chord & 0XFF));
    for(int i = 0; i < 5; i++) {
        printk("message[%d] = 0x%02X\n", i, (unsigned int)(messages[i] & 0XFF));
    }
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