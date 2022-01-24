/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef IR_H
#define	IR_H

unsigned char decodedData;
bool received = false;
bool repeated = false;

bool dataArray[32];


bool receive(void);


#define IR_POWER            0b10100010
#define IR_VOLUMEUP         0b01100010
#define IR_FUNC_STOP        0b11100010
#define IR_FASTBACKWARD     0b00100010
#define IR_PLAYPAUSE        0b00000010
#define IR_FASTFORWARD      0b11000010
#define IR_DOWNARROW        0b11100000
#define IR_VOLUMEDOWN       0b10101000
#define IR_UPARROW          0b10010000
#define IR_ZERO             0b01101000
#define IR_EQ               0b10011000
#define IR_ST_REPT          0b10110000
#define IR_ONE              0b00110000
#define IR_TWO              0b00011000
#define IR_THREE			0b01111010
#define IR_FOUR             0b00010000
#define IR_FIVE             0b00111000
#define IR_SIX              0b01011010
#define IR_SEVEN			0b01000010
#define IR_EIGHT			0b01001010
#define IR_NINE             0b01010010



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

