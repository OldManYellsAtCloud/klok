#!/bin/bash
source ~/zephyrproject/venv/bin/activate
source ~/zephyrproject/zephyr/zephyr-env.sh
west build -b stm32f429i_disc1 -p auto
west flash --runner openocd
# -- -DDTC_OVERLAY_FILE=./boards/stm32f429i_disc1.overlay
#west build -b stm32f746g_disco -p auto -- -DDTC_OVERLAY_FILE=./boards/stm32f746g_disco.overlay
