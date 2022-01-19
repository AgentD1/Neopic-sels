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
#include <stdlib.h>

#include    "UBMP4.h"           // Include UBMP4 constants and functions

#include    "Neopixel.h"


#define LED_NUM 45

void rainbowCycle(void);
void solidColour(void);
void pulsingSolidColour(void);
void debug(void);
void randomLightup(void);
void potentiometerReading(void);

void hsvtorgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v);
void setCode(unsigned char number, unsigned char *r1, unsigned char *g1, unsigned char *b1, unsigned char *r2, unsigned char *g2, unsigned char *b2, unsigned char *r3, unsigned char *g3);

unsigned char numFunctions = 6;
unsigned char functionIndex = 0;
void (*colorFunctions[])(void) = { rainbowCycle, solidColour, pulsingSolidColour, debug, randomLightup, potentiometerReading };

int tick = 1;
int ticks_left = 0;

unsigned char reds[LED_NUM];
unsigned char greens[LED_NUM];
unsigned char blues[LED_NUM];

unsigned char decodedData;

int main(void) {
    // Configure oscillator and I/O ports. These functions run once at start-up.
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
    ADC_config();
    
    ADC_select_channel(ANH2);
    
    
    
    TRISC &= 0b11011111;
            
    while(1) {
        LED4 = H3IN;
    }
    
    /*
    void doAlittleTrolling() {
        
        while (cap == true) {
            
            Legromp++;
            if (legromp > cap) {
                
                eat.exe;
                        
                heal.player.sheesh;
                Get drake;
                playerGun = ODIN;
                
                Sage = GOD TIER;
                
                Bucky.update = buff;
                If (Bucky.update = true) {
                    
                    Meikai = quaduple.ace
                }
                Sheesh
            }
            
        }
        
    }
     * */
    /*
    // Code in this while loop runs repeatedly.
    while(1) {
        neopixel_fill_a(LED_NUM, reds, greens, blues);
        
        if(SW1 == 0 && ticks_left == 0) {
            functionIndex++;
            functionIndex %= numFunctions;
            
            for(unsigned char i = 0; i < LED_NUM; i++) {
                reds[i] = 0;
                greens[i] = 0;
                blues[i] = 0;
            }
            
            ticks_left = 20;
        }
        
        (*colorFunctions[functionIndex])();
        
        if(ticks_left != 0) ticks_left--;
        
        tick++;
        
        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0 && SW2 == 0) {
            RESET();
        }
    }*/
}

bool receive() {
    while(H3IN);
    __delay_ms(40);
    __delay_us(550);
    unsigned char signal = 0;
    for(unsigned char i = 0; i < 8; i++) {
        if(!H3IN) {
            signal |= 1;
            __delay_us(1120);
        }
        signal <<= 1;
        __delay_us(1120);
    }
    
    unsigned char antisignal = 0;
    for(unsigned char i = 0; i < 8; i++) {
        if(!H3IN) {
            antisignal |= 1;
            __delay_us(1120);
        }
        antisignal <<= 1;
        __delay_us(1120);
    }
    
    if(~antisignal != signal) {
        decodedData = 0;
        return false;
    }
    
    decodedData = signal;
    
    return true;
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

    unsigned char pot = ADC_read();
    
    for(unsigned char i = 0; i < LED_NUM; i++) {
        if(reversed) {
            hsvtorgb(&reds[i], &greens[i], &blues[i], (unsigned char)(-tick) + (i * 2), 255, pot);
        } else {
            hsvtorgb(&reds[i], &greens[i], &blues[i], (unsigned char)(tick) + (i * 2), 255, pot);
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

    unsigned char pot = ADC_read();
    
    for(unsigned char i = 0; i < LED_NUM; i++) {
        hsvtorgb(&reds[i], &greens[i], &blues[i], hue, 255, pot);
        
        reds[i] >>= left_shift;
        greens[i] >>= left_shift;
        blues[i] >>= left_shift;
    }
}

bool pulsingReversed = false;
unsigned char pulsingTick = 0;

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

    for(unsigned char i = 0; i < LED_NUM; i++) {
        hsvtorgb(&reds[i], &greens[i], &blues[i], hue, 255, pulsingTick);
        
        reds[i] >>= left_shift;
        greens[i] >>= left_shift;
        blues[i] >>= left_shift;
    }
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

    for(unsigned char i = 0; i < LED_NUM; i++) {
        hsvtorgb(&reds[i], &greens[i], &blues[i], hue, 255, 255);
        
        reds[i] >>= left_shift;
        greens[i] >>= left_shift;
        blues[i] >>= left_shift;
    }
    
    setCode(hue, &reds[0], &greens[0], &blues[0], &reds[1], &greens[1], &blues[1], &reds[2], &greens[2]);
    blues[2] = 0;
}

float redsf[LED_NUM], greensf[LED_NUM], bluesf[LED_NUM];

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
    
    unsigned char reduction = 2;

    for(unsigned char i = 0; i < LED_NUM; i++) {
//        if(reds[i] > reduction) {
//            reds[i] -= reduction;
//        } else if(reds[i] > 0) {
//            reds[i] = 0;
//        }
//        if(greens[i] > reduction) {
//            greens[i] -= reduction;
//        } else if(greens[i] > 0) {
//            greens[i] = 0;
//        }
//        if(blues[i] > reduction) {
//            blues[i] -= reduction;
//        } else if(blues[i] > 0) {
//            blues[i] = 0;
//        }
        
        redsf[i] *= 0.95;
        greensf[i] *= 0.95;
        bluesf[i] *= 0.95;
        
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
        
        if((char)(rand()) == 255) {
            hsvtorgb(&reds[i], &greens[i], &blues[i], hue, 255, 255);
            redsf[i] = reds[i];
            greensf[i] = greens[i];
            bluesf[i] = blues[i];
        }
    }
        
    if(tick % 8 == 0) {
        hue++;
    }
}

void potentiometerReading() {
    setCode(ADC_read(), &reds[0], &greens[0], &blues[0], &reds[1], &greens[1], &blues[1], &reds[2], &greens[2]);
    
    __delay_ms(10);
}

void setCode(unsigned char number, unsigned char *r1, unsigned char *g1, unsigned char *b1, unsigned char *r2, unsigned char *g2, unsigned char *b2, unsigned char *r3, unsigned char *g3) {
    *r1 = ((number & 0b00000001) != 0) * 32;
    *g1 = ((number & 0b00000010) != 0) * 32;
    *b1 = ((number & 0b00000100) != 0) * 32;
    *r2 = ((number & 0b00001000) != 0) * 32;
    *g2 = ((number & 0b00010000) != 0) * 32;
    *b2 = ((number & 0b00100000) != 0) * 32;
    *r3 = ((number & 0b01000000) != 0) * 32;
    *g3 = ((number & 0b10000000) != 0) * 32;
}

// https://stackoverflow.com/a/22120275
void hsvtorgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h1, unsigned char s1, unsigned char v1) {
    unsigned char region, p, q, t;
    unsigned int h, s, v, remainder;

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
unsigned long randState = 6666;

unsigned long rand() {
    const unsigned long A = 48271;
    
    unsigned long low = (randState & 0x7fff) * A;
    unsigned long high = (randState >> 15) * A;
    
    unsigned long x = low + ((high & 0xffff) << 15) + (high >> 16);
    
    x = (x & 0x7fffffff) + (x >> 31);
    
    randState = x;
    
    return x;
}*/