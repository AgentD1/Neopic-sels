/*
 * File:   IR.c
 * Author: Parker
 *
 * Created on January 23, 2022, 4:22 PM
 */


#include "UBMP4.h" // include processor files - each processor file is guarded.  

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "IR.h"

bool receive() {
    LED5 = 0;
    if(H3IN) {
        LED5 = 1;
        return false;
    }
    /*
    for(uint8_t i = 0; i < 8; i++) { // Ommitted in order to capture pulses more reliably
        __delay_ms(1);
        if(H3IN) {
            return false;
        }
    }*/
    while(!H3IN);
    __delay_us(2260);
    if(!H3IN) {
        LED6 = !LED6;
        repeated = true;
        LED5 = 1;
        return true;
    }
    __delay_us(2240);
    
    __delay_us(560);
    LED5 = 1;
    uint8_t dataIndex = 0;
    uint8_t oneNumber = 0;
    for (uint8_t i = 0; i != 255; i++) {
        bool p = H3IN;
        if(p == 0) {
            if(oneNumber == 0) {
                continue;
            }
            dataArray[dataIndex] = oneNumber != 1;
            oneNumber = 0;
            dataIndex++;
            if(dataIndex == 32) {
                break;
            }
            __delay_us(559);
        } else {
            oneNumber++;
            if(oneNumber == 4) { // Illegal in NEC
                return false;
            }
            __delay_us(560);
        }
    }
    
    uint8_t address, notAddress, data, notData;
    
    for(uint8_t i = 0; i < 8; i++) {
        address |= dataArray[i];
        if(i != 7) address <<= 1;
    }
    for(uint8_t i = 0; i < 8; i++) {
        notAddress |= dataArray[i + 8];
        if(i != 7) notAddress <<= 1;
    }
    for(uint8_t i = 0; i < 8; i++) {
        data |= dataArray[i + 16];
        if(i != 7) data <<= 1;
    }
    for(uint8_t i = 0; i < 8; i++) {
        notData |= dataArray[i + 24];
        if(i != 7) notData <<= 1;
    }
    
    
    if(address != 0 || (address ^ notAddress) != 0xFF) {
        LED4 = !LED4;
        return false;
    }
    
//    if(data ^ notData != 0xFF) {
//        //return false;
//    }
    
    decodedData = data;
    
    
    
    return true;
}