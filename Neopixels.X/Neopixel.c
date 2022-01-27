/*
 * File:   Neopixel.c
 * Author: parkj8494
 *
 * Created on December 7, 2021, 9:03 AM
 */


#include <xc.h>
#include "UBMP4.h"
#include "stdint.h"

void neopixel_colour_send(uint8_t colour);

void neopixel_fill_a(uint8_t leds, uint8_t red[], uint8_t green[], uint8_t blue[]) {
    uint8_t ledNum = 0;
    for(; ledNum != leds; ledNum++) {
        neopixel_colour_send(green[ledNum]);
        neopixel_colour_send(red[ledNum]);
        neopixel_colour_send(blue[ledNum]);
    }
}

void neopixel_fill(uint8_t leds, uint8_t red, uint8_t green, uint8_t blue) {
    for(; leds != 0; leds --) {
        neopixel_colour_send(green);
        neopixel_colour_send(red);
        neopixel_colour_send(blue);
    }
}

void neopixel_colour_send(uint8_t colour) {
    for(uint8_t bits = 8; bits != 0; bits --) {
        H1OUT = 1;
        if((colour & 0b10000000) == 0) {
            H1OUT = 0;
        }
        NOP();
        H1OUT = 0;
        colour <<= 1;
    }
}
