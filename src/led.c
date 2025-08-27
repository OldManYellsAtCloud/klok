#include "led.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(led);

#define SLEEP_TIME_MS 1000
#define LED0_NODE DT_ALIAS(led0)

/* size of stack area used by each thread */
#define STACKSIZE 4096

/* scheduling priority used by each thread */
#define PRIORITY 7

K_THREAD_STACK_DEFINE(led_stack_area, STACKSIZE);
struct k_thread led_thread_data;

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

void led_thread(void){
    int ret;
    while (1) {
        ret = gpio_pin_toggle_dt(&led);
        if (ret < 0) {
            LOG_ERR("Failed to toggle LED: %d", ret);
            return;
        }

        if (ret < 0) {
            LOG_ERR("Failed to toggle backlight: %d", ret);
            return;
        }

        /* Sleep for specified time */
        k_msleep(SLEEP_TIME_MS);
    }
}

void led_setup(void){
    int ret;
    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED device %s is not ready", led.port->name);
        return;
    }

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure LED pin: %d", ret);
        return;
    }

    k_thread_create(&led_thread_data, led_stack_area, K_THREAD_STACK_SIZEOF(led_stack_area),
                led_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
}