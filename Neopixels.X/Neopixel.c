/*
 * File:   Neopixel.c
 * Author: parkj8494
 *
 * Created on December 7, 2021, 9:03 AM
 */


#include <xc.h>
#include "UBMP4.h"

void neopixel_colour_send(unsigned char colour);

void neopixel_fill_a(unsigned char leds, unsigned char red[], unsigned char green[], unsigned char blue[]) {
    int ledNum = 0;
    for(; ledNum != leds; ledNum++) {
        neopixel_colour_send(green[ledNum]);
        neopixel_colour_send(red[ledNum]);
        neopixel_colour_send(blue[ledNum]);
    }
}

void neopixel_fill(unsigned char leds, unsigned char red, unsigned char green, unsigned char blue) {
    for(; leds != 0; leds --) {
        neopixel_colour_send(green);
        neopixel_colour_send(red);
        neopixel_colour_send(blue);
    }
}

void neopixel_colour_send(unsigned char colour) {
    for(unsigned char bits = 8; bits != 0; bits --) {
        H1OUT = 1;
        if((colour & 0b10000000) == 0) {
            H1OUT = 0;
        }
        NOP();
        H1OUT = 0;
        colour <<= 1;
    }
}
