#include "display.h"
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(display);

#define BUFFER_WIDTH 100
#define BUFFER_HEIGHT 10

const struct device *display_dev;
struct display_buffer_descriptor *buf_desc;

static int CHAR_POSITIONS[] = {5, 80, 155, 170, 245};

static void *buf;

static void display_blank_whole_screen(){
    memset(buf, 0, BUFFER_HEIGHT * BUFFER_WIDTH * sizeof(uint16_t));
    for (size_t row = 0; row < 320; row += BUFFER_HEIGHT) {
        for (size_t col = 0; col < 240; col += BUFFER_WIDTH) {
            display_write(display_dev, col, row, buf_desc, buf);
        }
    }
}

void display_setup(){
    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev)) {
        LOG_ERR("Display is not ready, hmmm");
        return;
    }

    buf_desc = k_calloc(1, sizeof(struct display_buffer_descriptor));

    buf_desc->height = BUFFER_HEIGHT;
    buf_desc->width = BUFFER_WIDTH;
    buf_desc->frame_incomplete = false;
    buf_desc->pitch = BUFFER_WIDTH;
    buf_desc->buf_size = BUFFER_HEIGHT * BUFFER_WIDTH * sizeof(uint16_t);

    buf = k_calloc(BUFFER_WIDTH * BUFFER_HEIGHT, sizeof(uint16_t));
    if (!buf){
        LOG_ERR("Not enough memory!");
        return;
    }

    display_blank_whole_screen();

    display_blanking_off(display_dev);
}


/**
 * Calculate the color that should be displayed at a position in
 * the framebuffer. It scales the buffer to character size,
 * and depending on the current slice, it returns the corresponding 
 * character color value.
 */
static inline uint16_t display_color_at_position(uint8_t char_slice, int buf_col, int buf_row, struct char_with_width *character){
    uint16_t color;
    uint8_t scaled_col, scaled_row;
    // switch col and row - the framebuffer is portrait, but
    // the screen is in landscape.
    scaled_row = (char_slice * 10 + buf_col) / 10;
    // the screen is upside down, so render it also upside down
    // bottom row at the top
    scaled_col = (9 - buf_row / 10) * character->width;
    color = character->data[scaled_col + scaled_row];
    return color;
}

/**
 * Fills the buffer with the scaled color values of the slice of the character.
 * A slice is a scaled column of pixels.
 */
static inline void display_fill_buffer(uint8_t char_slice, struct char_with_width *character){
    uint16_t color;

    for (int buf_row = 0; buf_row < BUFFER_WIDTH; ++buf_row){
        for (int buf_col = 0; buf_col < BUFFER_HEIGHT; ++buf_col){
            color = display_color_at_position(char_slice, buf_col, buf_row, character);
            memset((uint16_t*)buf + (buf_col * 100 + buf_row), color, sizeof(uint16_t));
        }
    }
}

/**
 * Draw a character bitmap on the screen at a given position.
 * The third, middle position is narrow, only 10 pixels wide.
 * Positions 1, 2, 4, and 5 are 70 pixels wide.
 * 
 * For now display text only in the middle of the screen.
 */
void display_draw_character(uint8_t position, struct char_with_width *character){
    for (int char_slice = 0; char_slice < character->width; ++char_slice){
        display_fill_buffer(char_slice, character);
        display_write(display_dev, 80, CHAR_POSITIONS[position] + char_slice * 10, buf_desc, buf);
    }
}