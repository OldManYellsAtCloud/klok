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

K_THREAD_STACK_DEFINE(clock_stack_area, STACKSIZE);
struct k_thread clock_thread_data;

static void clock_timer_handler(){
    while (1){

        display_draw_character(0, &chars[0]);
        display_draw_character(1, &chars[1]);
        display_draw_character(2, &chars[10]);
        display_draw_character(3, &chars[2]);
        display_draw_character(4, &chars[3]);

        k_msleep(500);

        display_draw_character(0, &chars[4]);
        display_draw_character(1, &chars[5]);
        display_draw_character(2, &chars[11]);
        display_draw_character(3, &chars[8]);
        display_draw_character(4, &chars[9]);

        k_msleep(500);
    }
}


void clock_setup(void){
    k_thread_create(&clock_thread_data, clock_stack_area, K_THREAD_STACK_SIZEOF(clock_stack_area),
                    clock_timer_handler, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
}
