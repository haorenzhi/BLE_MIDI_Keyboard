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

#define SLEEP_TIME_MS 3
/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 * !!!!!! reconfig the official dts files using .overlay file
 * here we define 7 buttons for simulation
 */
#define SW0_NODE	DT_ALIAS(sw0)
#define SW1_NODE	DT_ALIAS(sw1)
#define SW2_NODE	DT_ALIAS(sw2)
#define SW3_NODE	DT_ALIAS(sw3)
#define SW4_NODE	DT_ALIAS(sw4)
#define SW5_NODE	DT_ALIAS(sw5)
#define SW6_NODE	DT_ALIAS(sw6)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay) || !DT_NODE_HAS_STATUS(SW1_NODE, okay) // skip other node if not necessary
#error "Unsupported board: devicetree alias is not defined"
#endif

/* 
gpios = <&gpio0 11 (GPIO_PULL_UP | GPIO_ACTIVE_LOW)>; 
struct gpio_dt_spec {
	const struct device *port;
	gpio_pin_t pin;
	gpio_dt_flags_t dt_flags;
};
*/
static const struct gpio_dt_spec buttons[7] = [
    GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0}),
    GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios, {1}),
    GPIO_DT_SPEC_GET_OR(SW2_NODE, gpios, {2}),
    GPIO_DT_SPEC_GET_OR(SW3_NODE, gpios, {3}),
    GPIO_DT_SPEC_GET_OR(SW4_NODE, gpios, {4}),
    GPIO_DT_SPEC_GET_OR(SW5_NODE, gpios, {5}),
    GPIO_DT_SPEC_GET_OR(SW6_NODE, gpios, {6}),
    ]

/*
struct gpio_callback {
	sys_snode_t node;
	gpio_callback_handler_t handler;
	gpio_port_pins_t pin_mask;
};
*/
static struct gpio_callback button_cb_data[7];

// define callback handler named button_pressed
void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

void main(void)
{
	int[] ret = new int[7];
        
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

	/* main function here */
    while (1) {
        /* If we have an LED, match its state to the button's. */

        int[] val = new int[7];
        for(int i = 0; i < 7; i++) {
            val[i] = gpio_pin_get_dt(&buttons[i]);
        }

        k_msleep(SLEEP_TIME_MS); // sleep 3s for reading the input data

        /* do something here
         * wrap up val array into MIDI data
         * sending the MIDI data using BLE
         * */
        char str[80];
        for(int i = 0; i < 7; i++) {
            if(val[i] == 1) {
                if(i == 0) {
                    strcpy(str, "8080903C7F");
                    strcat(str, "8090")
                }else {
                    strcat(str, "40") // 40 represents E notes above 3C( modify it according to the index)
                    strcat(str, "7F"); // 7F represents the MIDI_VELOCITY_MAX
                }
            }
        }

        /* set the val array and button value to zero (unpressed) */
        for(int i = 0; i < 7; i++) {
            if(val[i] > 0) {
                val[i] = 0;
                gpio_pin_set_dt(&buttons[i], 0);
            }
        }
    }

}
