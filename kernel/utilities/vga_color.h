
#include "kernel/inc/types.h"

#ifndef VGA_COLOR_H

#define VGA_COLOR_H

#define DEFAULT_COLOR 0x0f  // White text on black background

typedef u8 vga_color;

enum vga_colors {
    BLACK_TEXT = 0x00,
    BLUE_TEXT = 0x01,
    GREEN_TEXT = 0x02,
    CYAN_TEXT = 0x03,
    RED_TEXT = 0x04,
    MAGENTA_TEXT = 0x05,
    BROWN_TEXT = 0x06,
    LIGHT_GREY_TEXT = 0x07,
    DARK_GREY_TEXT = 0x08,
    LIGHT_BLUE_TEXT = 0x09,
    LIGHT_GREEN_TEXT = 0x0A,
    LIGHT_CYAN_TEXT = 0x0B,
    LIGHT_RED_TEXT = 0x0C,
    LIGHT_MAGENTA_TEXT = 0x0D,
    LIGHT_BROWN_TEXT = 0x0E,
    WHITE_TEXT = 0x0F,

    BLACK_BACKGROUND = 0x00,
    BLUE_BACKGROUND = 0x10,
    GREEN_BACKGROUND = 0x20,
    CYAN_BACKGROUND = 0x30,
    RED_BACKGROUND = 0x40,
    MAGENTA_BACKGROUND = 0x50,
    BROWN_BACKGROUND = 0x60,
    LIGHT_GREY_BACKGROUND = 0x70,
    DARK_GREY_BACKGROUND = 0x80,
    LIGHT_BLUE_BACKGROUND = 0x90,
    LIGHT_GREEN_BACKGROUND = 0xA0,
    LIGHT_CYAN_BACKGROUND = 0xB0,
    LIGHT_RED_BACKGROUND = 0xC0,
    LIGHT_MAGENTA_BACKGROUND = 0xD0,
    LIGHT_BROWN_BACKGROUND = 0xE0,
    WHITE_BACKGROUND = 0xF0
};


/**
 * @brief Create a vga color object
 * 
 * @param foreground vga_color The text / foreground color
 * @param background vga_color The background color
 * @return u8 
 */
vga_color create_vga_color(vga_color foreground, vga_color background) {
    return foreground | background;
}

#endif