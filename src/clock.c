#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include "clock.h"
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>

#include "display.h"
#include "characters.h"

LOG_MODULE_REGISTER(clock);

#define STACKSIZE 4096
#define PRIORITY 7

#define COLON_IDX 10
#define NO_COLON_IDX 11

K_THREAD_STACK_DEFINE(clock_stack_area, STACKSIZE);
struct k_thread clock_thread_data;

static void clock_timer_handler(){
    uint32_t uptime;
    uint8_t minutes, seconds;
    while (1){

        uptime = k_uptime_seconds();
        minutes = (uptime / 60) % 100;
        seconds = uptime - (int)(uptime / 60) * 60;

        display_draw_character(0, &chars[minutes / 10]);
        display_draw_character(1, &chars[minutes % 10]);
        display_draw_character(2, &chars[COLON_IDX]);
        display_draw_character(3, &chars[seconds / 10]);
        display_draw_character(4, &chars[seconds % 10]);

        k_msleep(500);

        display_draw_character(2, &chars[NO_COLON_IDX]);
        k_msleep(500);
    }
}


void clock_setup(void){
    k_thread_create(&clock_thread_data, clock_stack_area, K_THREAD_STACK_SIZEOF(clock_stack_area),
                    clock_timer_handler, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
}
