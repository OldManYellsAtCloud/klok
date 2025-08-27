cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(rpi_pico_st7789_blinky)

target_sources(app PRIVATE src/main.c
                           src/clock.h
                           src/clock.c
                           src/led.h
                           src/led.c
                           src/display.h
                           src/display.c
                           src/char_utils.h
                           )