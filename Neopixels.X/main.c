/*==============================================================================
 Project: Intro-1-Input-Output
 Date:    May 16, 2021
 
 This example UBMP4 input and output program demonstrates pushbutton input, LED
 (bit) output, port latch (byte) output, time delay functions, and simple 'if'
 condition structures.
 
 Additional program analysis and programming activities demonstrate byte output,
 logical condition operators AND and OR, using delay functions to create sound,
 and simulated start-stop button functionality.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP4.h"           // Include UBMP4 constants and functions

#include    "Neopixel.h"

void rainbowCycle();
void solidColour();
void pulsingSolidColour();

void hsvtorgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v);

unsigned char numFunctions = 3;
unsigned char functionIndex = 0;
void (*colorFunctions[])(void) = { rainbowCycle, solidColour, pulsingSolidColour };

int tick = 1;
int ticks_left = 0;

unsigned char reds[45];
unsigned char greens[45];
unsigned char blues[45];

int main(void) {
    // Configure oscillator and I/O ports. These functions run once at start-up.
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
    
    int red = 0xFF;
    int green = 0;
    int blue = 0;
    
    // Code in this while loop runs repeatedly.
    while(1) {
        neopixel_fill_a(45, reds, greens, blues);
        
        if(SW1 == 0 && ticks_left == 0) {
            functionIndex++;
            functionIndex %= numFunctions;
            
            ticks_left = 20;
        }
        
        (*colorFunctions[functionIndex])();
        
        if(ticks_left != 0) ticks_left--;
        
        tick++;
        
        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0 && SW2 == 0) {
            RESET();
        }
    }
}

bool reversed = false;
unsigned char left_shift = 0;

void rainbowCycle() {
    if(ticks_left == 0) {
        if(SW2 == 0 && left_shift != 8) {
            left_shift++;
            ticks_left = 20;
        }
        if(SW3 == 0 && left_shift != 0) {
            left_shift--;
            ticks_left = 20;
        }
        if(SW4 == 0) {
            reversed = !reversed;
            ticks_left = 20;
        }
    }

    __delay_ms(4);

    for(unsigned char i = 0; i < 45; i++) {
        if(reversed) {
            hsvtorgb(&reds[i], &greens[i], &blues[i], (unsigned char)(-tick) + (i * 2), 255, 255);
        } else {
            hsvtorgb(&reds[i], &greens[i], &blues[i], (unsigned char)(tick) + (i * 2), 255, 255);
        }
        reds[i] >>= left_shift;
        greens[i] >>= left_shift;
        blues[i] >>= left_shift;
    }
}

unsigned char hue = 1;

void solidColour() {
    if(ticks_left == 0) {
        if(SW2 == 0 && left_shift != 8) {
            left_shift++;
            ticks_left = 20;
        }
        if(SW3 == 0 && left_shift != 0) {
            left_shift--;
            ticks_left = 20;
        }
        if(SW4 == 0) {
            hue += 4;
            ticks_left = 20;
        }
        if(SW5 == 0) {
            hue -= 4;
            ticks_left = 20;
        }
    }

    __delay_ms(4);

    for(unsigned char i = 0; i < 45; i++) {
        hsvtorgb(&reds[i], &greens[i], &blues[i], hue, 255, 255);
        
        reds[i] >>= left_shift;
        greens[i] >>= left_shift;
        blues[i] >>= left_shift;
    }
}

bool pulsingReversed = false;

void pulsingSolidColour() {
    if(ticks_left == 0) {
        if(SW2 == 0 && left_shift != 8) {
            left_shift++;
            ticks_left = 20;
        }
        if(SW3 == 0 && left_shift != 0) {
            left_shift--;
            ticks_left = 20;
        }
        if(SW4 == 0) {
            hue += 4;
            ticks_left = 20;
        }
        if(SW5 == 0) {
            hue -= 4;
            ticks_left = 20;
        }
    }

    __delay_ms(4);

    for(unsigned char i = 0; i < 45; i++) {
        hsvtorgb(&reds[i], &greens[i], &blues[i], hue, 255, tick);
        
        reds[i] >>= left_shift;
        greens[i] >>= left_shift;
        blues[i] >>= left_shift;
    }
}


// http://web.mit.edu/storborg/Public/hsvtorgb.c
//void hsvtorgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v) {
//    unsigned char region, fpart, p, q, t;
//    
//    if(s == 0) {
//        /* color is grayscale */
//        *r = *g = *b = v;
//        return;
//    }
//    
//    /* make hue 0-5 */
//    region = h / 43;
//    /* find remainder part, make it from 0-255 */
//    fpart = (h - (region * 43)) * 6;
//    
//    /* calculate temp vars, doing integer multiplication */
//    p = (v * (255 - s)) >> 8;
//    q = (v * (255 - ((s * fpart) >> 8))) >> 8;
//    t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;
//        
//    /* assign temp vars based on color cone region */
//    switch(region) {
//        case 0:
//            *r = v; *g = t; *b = p; break;
//        case 1:
//            *r = q; *g = v; *b = p; break;
//        case 2:
//            *r = p; *g = v; *b = t; break;
//        case 3:
//            *r = p; *g = q; *b = v; break;
//        case 4:
//            *r = t; *g = p; *b = v; break;
//        default:
//            *r = v; *g = p; *b = q; break;
//    }
//    
//    return;
//}
