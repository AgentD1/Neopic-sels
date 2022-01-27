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
#include    <stdlib.h>

#include    "UBMP4.h"           // Include UBMP4 constants and functions

#include    "Neopixel.h"
#include    "IR.h"


#define LED_NUM 10

void rainbowCycle(void);
void solidColour(void);
void pulsingSolidColour(void);
void debug(void);
void randomLightup(void);
void potentiometerReading(void);
void receiveDebug(void);


void hsvtorgb(uint8_t* r, uint8_t *g, uint8_t *b, uint8_t h, uint8_t s, uint8_t v);
void setCode(uint8_t number, uint8_t *r1, uint8_t *g1, uint8_t *b1, uint8_t *r2, uint8_t *g2, uint8_t *b2, uint8_t *r3, uint8_t *g3);

uint8_t numFunctions = 4;
uint8_t functionIndex = 0;
void (*colorFunctions[])(void) = { rainbowCycle, solidColour, pulsingSolidColour, randomLightup };

int tick = 1;
int ticks_left = 0;

uint8_t reds[LED_NUM];
uint8_t greens[LED_NUM];
uint8_t blues[LED_NUM];

const uint8_t gamma[256] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 2,
    2, 2, 2, 2, 3, 3, 3, 3,
    4, 4, 4, 4, 5, 5, 5, 6,
    6, 6, 7, 7, 8, 8, 8, 9,
    9, 10, 10, 10, 11, 11, 12, 12,
    13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 21,
    21, 22, 22, 23, 24, 24, 25, 26,
    26, 27, 28, 28, 29, 30, 30, 31,
    32, 32, 33, 34, 35, 35, 36, 37,
    38, 38, 39, 40, 41, 41, 42, 43,
    44, 45, 46, 46, 47, 48, 49, 50,
    51, 52, 53, 53, 54, 55, 56, 57,
    58, 59, 60, 61, 62, 63, 64, 65,
    66, 67, 68, 69, 70, 71, 72, 73,
    74, 75, 76, 77, 78, 79, 80, 81,
    82, 83, 84, 86, 87, 88, 89, 90,
    91, 92, 93, 95, 96, 97, 98, 99,
    100, 102, 103, 104, 105, 107, 108, 109,
    110, 111, 113, 114, 115, 116, 118, 119,
    120, 122, 123, 124, 126, 127, 128, 129,
    131, 132, 134, 135, 136, 138, 139, 140,
    142, 143, 145, 146, 147, 149, 150, 152,
    153, 154, 156, 157, 159, 160, 162, 163,
    165, 166, 168, 169, 171, 172, 174, 175,
    177, 178, 180, 181, 183, 184, 186, 188,
    189, 191, 192, 194, 195, 197, 199, 200,
    202, 204, 205, 207, 208, 210, 212, 213,
    215, 217, 218, 220, 222, 224, 225, 227,
    229, 230, 232, 234, 236, 237, 239, 241,
    243, 244, 246, 248, 250, 251, 253, 255
};


bool on = true;


int main(void) {
    // Configure oscillator and I/O ports. These functions run once at start-up.
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
    ADC_config();
    
    ADC_select_channel(ANH2);
    
    
    TRISC &= 0b11011111;
    
    LED1 = 0;
    LED3 = 1;
            
    while(1) {
        received = false;
        repeated = false;
        //LED4 = !LED4;
        
        LED1 = !LED1;
        LED3 = !LED3;
        
        received = receive();
        
        
        if(received && !repeated && decodedData == IR_POWER) {
            on = !on;
            if(!on) {
                for(uint8_t i = 0; i < LED_NUM; i++) {
                    reds[i] = 0;
                    greens[i] = 0;
                    blues[i] = 0;
                }
            }
        }
        
        for(uint8_t i = 0; i < LED_NUM; i++) {
            reds[i] = gamma[reds[i]];
            greens[i] = gamma[greens[i]];
            blues[i] = gamma[blues[i]];
        }
        
        neopixel_fill_a(LED_NUM, reds, greens, blues);
        
        if(on) {
            if((SW1 == 0 && ticks_left == 0) || (received && !repeated && decodedData == IR_FUNC_STOP)) {
                functionIndex++;
                functionIndex %= numFunctions;

                for(uint8_t i = 0; i < LED_NUM; i++) {
                    reds[i] = 0;
                    greens[i] = 0;
                    blues[i] = 0;
                }

                ticks_left = 100;
            }

            (*colorFunctions[functionIndex])();

            if(ticks_left != 0) ticks_left--;

            tick++;
        }
        
        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0 && SW2 == 0) {
            RESET();
        }
        
    }
}



bool reversed = false;
uint8_t left_shift = 0;

uint8_t speed = 1;

uint8_t brightness = 255;

uint16_t rainbowCycleTick = 0;

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
    
    if(received) {
        if(decodedData == IR_VOLUMEDOWN) {
            brightness += 5;
        } else if(decodedData == IR_EQ) {
            brightness -= 5;
        } else if(!repeated && decodedData == IR_DOWNARROW) {
            speed--;
            if(speed == 0) {
                speed = 9;
            }
        } else if(!repeated && decodedData == IR_ST_REPT) {
            speed++;
            if(speed == 9) {
                speed = 0;
            }
        }
    }
    
    for(uint8_t i = 0; i < LED_NUM; i++) {
        if(reversed) {
            hsvtorgb(&reds[i], &greens[i], &blues[i], (uint8_t)rainbowCycleTick + (i * 1), 255, brightness);
        } else {
            hsvtorgb(&reds[i], &greens[i], &blues[i], (uint8_t)rainbowCycleTick + (i * 1), 255, brightness);
        }
        reds[i] >>= left_shift;
        greens[i] >>= left_shift;
        blues[i] >>= left_shift;
    }
    
    if(tick % speed == 0) {
        rainbowCycleTick++;
    }
}

uint8_t hue = 1;

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
    
    if(received) {
        if(decodedData == IR_VOLUMEDOWN) {
            brightness += 5;
        } else if(decodedData == IR_EQ) {
            brightness -= 5;
        } else if(decodedData == IR_DOWNARROW) {
            hue += 5;
        } else if(decodedData == IR_ST_REPT) {
            hue -= 5;
        }
    }

    __delay_ms(4);

    for(uint8_t i = 0; i < LED_NUM; i++) {
        hsvtorgb(&reds[i], &greens[i], &blues[i], hue, 255, brightness);
        
        reds[i] >>= left_shift;
        greens[i] >>= left_shift;
        blues[i] >>= left_shift;
    }
}

bool pulsingReversed = false;
uint8_t pulsingTick = 0;

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
    
    
    if(received) {
        if(decodedData == IR_VOLUMEDOWN) {
            brightness += 5;
        } else if(decodedData == IR_EQ) {
            brightness -= 5;
        } else if(decodedData == IR_DOWNARROW) {
            hue += 5;
        } else if(decodedData == IR_ST_REPT) {
            hue -= 5;
        }
    }

    __delay_ms(4);
    
    
    if(!pulsingReversed) {
        pulsingTick++;
        if(pulsingTick == 255) {
            pulsingReversed = true;
        }
    } else {
        pulsingTick--;
        if(pulsingTick == 0) {
            pulsingReversed = false;
        }
    }

    for(uint8_t i = 0; i < LED_NUM; i++) {
        hsvtorgb(&reds[i], &greens[i], &blues[i], hue, 255, pulsingTick);
        
        reds[i] >>= left_shift;
        greens[i] >>= left_shift;
        blues[i] >>= left_shift;
    }
}


float redsf[LED_NUM], greensf[LED_NUM], bluesf[LED_NUM];
float reductionMultiple = 0.95;

void randomLightup() {
    if(ticks_left == 0) {
        if(SW2 == 0 && left_shift != 8) {
            left_shift++;
            ticks_left = 20;
        }
        if(SW3 == 0 && left_shift != 0) {
            left_shift--;
            ticks_left = 20;
        }
    }

    __delay_ms(4);
    
    uint8_t reduction = 2;
    
    if(received) {
        if(decodedData == IR_VOLUMEDOWN) {
            reductionMultiple += 0.01;
            if(reductionMultiple > 0.99) reductionMultiple = 0.99;
        } else if(decodedData == IR_EQ) {
            reductionMultiple -= 0.01;
            if(reductionMultiple < 0.9) reductionMultiple = 0.9;
        } else if(decodedData == IR_DOWNARROW) {
            speed--;
            if(speed == 0) {
                speed = 9;
            }
        } else if(!repeated && decodedData == IR_ST_REPT) {
            speed++;
            if(speed == 9) {
                speed = 0;
            }
        }
    }

    for(uint8_t i = 0; i < LED_NUM; i++) {
        redsf[i] *= reductionMultiple;
        greensf[i] *= reductionMultiple;
        bluesf[i] *= reductionMultiple;
        
        reds[i] = redsf[i];
        greens[i] = greensf[i];
        blues[i] = bluesf[i];
        if(greens[i] == 0 && greensf[i] > 0.2) {
            reds[i] = 0;
            redsf[i] = 0;
            greens[i] = 0;
            greensf[i] = 0;
            blues[i] = 0;
            bluesf[i] = 0;
        }
        if(reds[i] == 0 && redsf[i] > 0.2) {
            reds[i] = 0;
            redsf[i] = 0;
            greens[i] = 0;
            greensf[i] = 0;
            blues[i] = 0;
            bluesf[i] = 0;
        }
        if(blues[i] == 0 && bluesf[i] > 0.2) {
            reds[i] = 0;
            redsf[i] = 0;
            greens[i] = 0;
            greensf[i] = 0;
            blues[i] = 0;
            bluesf[i] = 0;
        }
        
        if((uint8_t)(rand()) == 255) {
            hsvtorgb(&reds[i], &greens[i], &blues[i], hue, 255, 255);
            redsf[i] = reds[i];
            greensf[i] = greens[i];
            bluesf[i] = blues[i];
        }
    }
    
    if(tick % speed == 0) {
        hue++;
    }
}

/*
void setCode(uint8_t number, uint8_t *r1, uint8_t *g1, uint8_t *b1, uint8_t *r2, uint8_t *g2, uint8_t *b2, uint8_t *r3, uint8_t *g3) {
    *r1 = ((number & 0b00000001) != 0) * 32;
    *g1 = ((number & 0b00000010) != 0) * 32;
    *b1 = ((number & 0b00000100) != 0) * 32;
    *r2 = ((number & 0b00001000) != 0) * 32;
    *g2 = ((number & 0b00010000) != 0) * 32;
    *b2 = ((number & 0b00100000) != 0) * 32;
    *r3 = ((number & 0b01000000) != 0) * 32;
    *g3 = ((number & 0b10000000) != 0) * 32;
}*/

// https://stackoverflow.com/a/22120275
void hsvtorgb(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t h1, uint8_t s1, uint8_t v1) {
    uint8_t region, p, q, t;
    uint16_t h, s, v, remainder;

    if (s1 == 0)
    {
        *r = v1;
        *g = v1;
        *b = v1;
    }

    // converting to 16 bit to prevent overflow
    h = h1;
    s = s1;
    v = v1;

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            *r = v;
            *g = t;
            *b = p;
            break;
        case 1:
            *r = q;
            *g = v;
            *b = p;
            break;
        case 2:
            *r = p;
            *g = v;
            *b = t;
            break;
        case 3:
            *r = p;
            *g = q;
            *b = v;
            break;
        case 4:
            *r = t;
            *g = p;
            *b = v;
            break;
        default:
            *r = v;
            *g = p;
            *b = q;
            break;
    }
}

/*
void receiveDebug() {
    for(uint8_t i = 0; i < 8; i++) {
        reds[i] = ((decodedData << i) & 0b10000000) == 0 ? 0 : 128;
        greens[i] = 128 - reds[i];
        blues[i] = 0;
    }
    
    blues[8] = blues[9] = 0;
    
    if(received && decodedData == IR_VOLUMEUP) {
        blues[8] = 128;
    } else if(received && decodedData == IR_VOLUMEDOWN) {
        blues[9] = 128;
    }
    
    __delay_ms(5);
}

void potentiometerReading() {
    setCode(ADC_read(), &reds[0], &greens[0], &blues[0], &reds[1], &greens[1], &blues[1], &reds[2], &greens[2]);
    
    __delay_ms(10);
}
void debug() {
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
            hue += 1;
            ticks_left = 20;
        }
        if(SW5 == 0) {
            hue -= 1;
            ticks_left = 20;
        }
    }

    __delay_ms(4);

    for(uint8_t i = 0; i < LED_NUM; i++) {
        hsvtorgb(&reds[i], &greens[i], &blues[i], hue, 255, 255);
        
        reds[i] >>= left_shift;
        greens[i] >>= left_shift;
        blues[i] >>= left_shift;
    }
    
    setCode(hue, &reds[0], &greens[0], &blues[0], &reds[1], &greens[1], &blues[1], &reds[2], &greens[2]);
    blues[2] = 0;
}

*/