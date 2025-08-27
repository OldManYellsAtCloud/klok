#include "clock.h"
#include "led.h"
#include "display.h"

int main(void)
{
    display_setup();
    clock_setup();
    led_setup();

    return 0;
}
