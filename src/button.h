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
extern unsigned char messages[5];

static const struct gpio_dt_spec buttons[7] = {
    GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0}),
    GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios, {0}),
    GPIO_DT_SPEC_GET_OR(SW2_NODE, gpios, {0}),
    GPIO_DT_SPEC_GET_OR(SW3_NODE, gpios, {0}),
    GPIO_DT_SPEC_GET_OR(SW4_NODE, gpios, {0}),
    GPIO_DT_SPEC_GET_OR(SW5_NODE, gpios, {0}),
    GPIO_DT_SPEC_GET_OR(SW6_NODE, gpios, {0}),
};

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
		    uint32_t pins);
void button_check(void);
void button_loop(void);