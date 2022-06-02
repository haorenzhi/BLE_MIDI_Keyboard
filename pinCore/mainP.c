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


#define SLEEP_TIME_MS	1

/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define SW0_NODE0	DT_ALIAS(sw0)
#define SW0_NODE1	DT_ALIAS(sw1)
#define SW0_NODE2	DT_ALIAS(sw2)
#define SW0_NODE3	DT_ALIAS(sw3)
#define SW0_NODE4	DT_ALIAS(sw4)
#define SW0_NODE5	DT_ALIAS(sw5)
#define SW0_NODE6	DT_ALIAS(sw6)
#if !DT_NODE_HAS_STATUS(SW0_NODE0, okay) && !DT_NODE_HAS_STATUS(SW0_NODE1, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE0, gpios,
							      {0});
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(SW0_NODE1, gpios,
							      {1});
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET_OR(SW0_NODE2, gpios,
							      {2});
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET_OR(SW0_NODE3, gpios,
							      {3});
static const struct gpio_dt_spec button4 = GPIO_DT_SPEC_GET_OR(SW0_NODE4, gpios,
							      {4});
static const struct gpio_dt_spec button5 = GPIO_DT_SPEC_GET_OR(SW0_NODE5, gpios,
							      {5});
static const struct gpio_dt_spec button6 = GPIO_DT_SPEC_GET_OR(SW0_NODE6, gpios,
							      {6});
static struct gpio_callback button_cb_data0;
static struct gpio_callback button_cb_data1;
static struct gpio_callback button_cb_data2;
static struct gpio_callback button_cb_data3;
static struct gpio_callback button_cb_data4;
static struct gpio_callback button_cb_data5;
static struct gpio_callback button_cb_data6;

/*
 * The led0 devicetree alias is optional. If present, we'll use it
 * to turn on the LED whenever the button is pressed.
 */
static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
						     {0});

void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

void main(void)
{
	int ret;

	if (!device_is_ready(button0.port)) {
		printk("Error: button device %s is not ready\n",
		       button0.port->name);
		return;
	}
	if (!device_is_ready(button1.port)) {
		printk("Error: button device %s is not ready\n",
		       button1.port->name);
		return;
	}
    if (!device_is_ready(button2.port)) {
		printk("Error: button device %s is not ready\n",
		       button2.port->name);
		return;
	}
    if (!device_is_ready(button3.port)) {
		printk("Error: button device %s is not ready\n",
		       button3.port->name);
		return;
	}
    if (!device_is_ready(button4.port)) {
		printk("Error: button device %s is not ready\n",
		       button4.port->name);
		return;
	}
    if (!device_is_ready(button5.port)) {
		printk("Error: button device %s is not ready\n",
		       button5.port->name);
		return;
	}
    if (!device_is_ready(button6.port)) {
		printk("Error: button device %s is not ready\n",
		       button6.port->name);
		return;
	}

	ret = gpio_pin_configure_dt(&button0, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button0.port->name, button0.pin);
		return;
	}
	ret = gpio_pin_configure_dt(&button1, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button1.port->name, button1.pin);
		return;
	}
    ret = gpio_pin_configure_dt(&button2, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button2.port->name, button2.pin);
		return;
	}
	ret = gpio_pin_configure_dt(&button3, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button3.port->name, button3.pin);
		return;
	}
    ret = gpio_pin_configure_dt(&button4, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button4.port->name, button4.pin);
		return;
	}
	ret = gpio_pin_configure_dt(&button5, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button5.port->name, button5.pin);
		return;
	}
    ret = gpio_pin_configure_dt(&button6, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button6.port->name, button6.pin);
		return;
	}

	ret = gpio_pin_interrupt_configure_dt(&button0,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button0.port->name, button0.pin);
		return;
	}
	ret = gpio_pin_interrupt_configure_dt(&button1,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button1.port->name, button1.pin);
		return;
	}
    ret = gpio_pin_interrupt_configure_dt(&button2,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button2.port->name, button2.pin);
		return;
	}
	ret = gpio_pin_interrupt_configure_dt(&button3,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button3.port->name, button3.pin);
		return;
	}
    ret = gpio_pin_interrupt_configure_dt(&button4,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button4.port->name, button4.pin);
		return;
	}
	ret = gpio_pin_interrupt_configure_dt(&button5,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button5.port->name, button5.pin);
		return;
	}
    ret = gpio_pin_interrupt_configure_dt(&button6,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button6.port->name, button6.pin);
		return;
	}

	// here now
	gpio_init_callback(&button_cb_data0, button_pressed, BIT(button0.pin));
	gpio_init_callback(&button_cb_data1, button_pressed, BIT(button1.pin));
    gpio_init_callback(&button_cb_data2, button_pressed, BIT(button2.pin));
	gpio_init_callback(&button_cb_data3, button_pressed, BIT(button3.pin));
    gpio_init_callback(&button_cb_data4, button_pressed, BIT(button4.pin));
	gpio_init_callback(&button_cb_data5, button_pressed, BIT(button5.pin));
    gpio_init_callback(&button_cb_data6, button_pressed, BIT(button6.pin));
	gpio_add_callback(button0.port, &button_cb_data0);
	gpio_add_callback(button1.port, &button_cb_data1);
    gpio_add_callback(button2.port, &button_cb_data2);
	gpio_add_callback(button3.port, &button_cb_data3);
    gpio_add_callback(button4.port, &button_cb_data4);
	gpio_add_callback(button5.port, &button_cb_data5);
    gpio_add_callback(button6.port, &button_cb_data6);
	// printk("Set up button at %s pin %d\n", button0.port->name, button0.pin);
	// printk("Set up button at %s pin %d\n", button1.port->name, button1.pin);
    // printk("Set up button at %s pin %d\n", button0.port->name, button2.pin);
	// printk("Set up button at %s pin %d\n", button1.port->name, button3.pin);

	if (led.port && !device_is_ready(led.port)) {
		printk("Error %d: LED device %s is not ready; ignoring it\n",
		       ret, led.port->name);
		led.port = NULL;
	}
	if (led.port) {
		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led.port->name, led.pin);
			led.port = NULL;
		} else {
			printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
		}
	}

	printk("Press the button\n");
	if (led.port) {
		while (1) {
			/* If we have an LED, match its state to the button's. */
			int val0 = gpio_pin_get_dt(&button0);
			if (val0 >= 0) {
				gpio_pin_set_dt(&led, val0);
			}

			int val1 = gpio_pin_get_dt(&button1);
			if (val1 >= 0) {
				gpio_pin_set_dt(&led, val1);
			}

            int val2 = gpio_pin_get_dt(&button2);
			if (val2 >= 0) {
				gpio_pin_set_dt(&led, val2);
			}

			int val3 = gpio_pin_get_dt(&button3);
			if (val3 >= 0) {
				gpio_pin_set_dt(&led, val3);
			}

            int val4 = gpio_pin_get_dt(&button4);
			if (val4 >= 0) {
				gpio_pin_set_dt(&led, val4);
			}

			int val5 = gpio_pin_get_dt(&button5);
			if (val5 >= 0) {
				gpio_pin_set_dt(&led, val5);
			}

            int val6 = gpio_pin_get_dt(&button6);
			if (val6 >= 0) {
				gpio_pin_set_dt(&led, val6);
			}

			k_msleep(SLEEP_TIME_MS);
            /* set the val array and button value to zero (unpressed) */
            if (val0 >0){
                val0 = 0;
                gpio_pin_set_dt(&button0,0);
            }
            if (val1 >0){
                val1 = 0;
                gpio_pin_set_dt(&button1,0);
            }
            if (val2 >0){
                val2 = 0;
                gpio_pin_set_dt(&button2,0);
            }
            if (val3 >0){
                val3 = 0;
                gpio_pin_set_dt(&button3,0);
            }
            if (val4 >0){
                val4 = 0;
                gpio_pin_set_dt(&button4,0);
            }
            if (val5 >0){
                val5 = 0;
                gpio_pin_set_dt(&button5,0);
            }
            if (val6 >0){
                val6 = 0;
                gpio_pin_set_dt(&button6,0);
            }
		}
	}
}
