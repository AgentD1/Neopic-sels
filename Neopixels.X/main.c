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

void rainbowCycle(void);
void solidColour(void);
void pulsingSolidColour(void);
void debug(void);
void randomLightup(void);

void init_genrand(unsigned long s);
unsigned char genrand_char(void);

void hsvtorgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v);
void setCode(unsigned char number, unsigned char *r1, unsigned char *g1, unsigned char *b1, unsigned char *r2, unsigned char *g2, unsigned char *b2, unsigned char *r3, unsigned char *g3);

unsigned char numFunctions = 5;
unsigned char functionIndex = 0;
void (*colorFunctions[])(void) = { rainbowCycle, solidColour, pulsingSolidColour, debug, randomLightup };

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
            
            for(unsigned char i = 0; i < 45; i++) {
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

    for(unsigned char i = 0; i < 45; i++) {
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

    for(unsigned char i = 0; i < 45; i++) {
        hsvtorgb(&reds[i], &greens[i], &blues[i], hue, 255, 255);
        
        reds[i] >>= left_shift;
        greens[i] >>= left_shift;
        blues[i] >>= left_shift;
    }
    
    setCode(hue, &reds[0], &greens[0], &blues[0], &reds[1], &greens[1], &blues[1], &reds[2], &greens[2]);
    blues[2] = 0;
}

float redsf[45], greensf[45], bluesf[45];

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

    for(unsigned char i = 0; i < 45; i++) {
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
        
        if(genrand_char() == 255) {
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

// http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/MT2002/CODES/MTARCOK/mt19937ar-cok.c
#define N 18
#define M 12
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UMASK 0x80000000UL /* most significant w-r bits */
#define LMASK 0x7fffffffUL /* least significant r bits */
#define MIXBITS(u,v) ( ((u) & UMASK) | ((v) & LMASK) )
#define TWIST(u,v) ((MIXBITS(u,v) >> 1) ^ ((v)&1UL ? MATRIX_A : 0UL))

static unsigned long state[N]; /* the array for the state vector  */
static int left = 1;
static int initf = 0;
static unsigned long *next;

/* initializes state[N] with a seed */
void init_genrand(unsigned long s) {
    int j;
    state[0]= s & 0xffffffffUL;
    for (j=1; j<N; j++) {
        state[j] = (1812433253UL * (state[j-1] ^ (state[j-1] >> 30)) + j); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array state[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        state[j] &= 0xffffffffUL;  /* for >32 bit machines */
    }
    left = 1; initf = 1;
}

static void next_state(void) {
    unsigned long *p=state;
    int j;

    /* if init_genrand() has not been called, */
    /* a default initial seed is used         */
    if (initf==0) init_genrand(5489UL);

    left = N;
    next = state;
    
    for (j=N-M+1; --j; p++) 
        *p = p[M] ^ TWIST(p[0], p[1]);

    for (j=M; --j; p++) 
        *p = p[M-N] ^ TWIST(p[0], p[1]);

    *p = p[M-N] ^ TWIST(p[0], state[0]);
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned char genrand_char(void)
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return (unsigned char)y;
}

