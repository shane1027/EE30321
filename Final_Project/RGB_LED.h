/*
 * File:   RGB_LED.h
 * Author: Shane Ryan - sryan8@nd.edu
 * Purpose:  Header file to setup and manipulate an RGB LED simply.
 *
 * Created on April 28, 2016, 2:36 AM
 */

// Pins for LED Control Signals
#define LED_RED LATBbits.LATB13
#define LED_RED_PIN TRISBbits.TRISB13
#define LED_RED_PULLUP CNPUBbits.CNPUB13

#define LED_GREEN LATBbits.LATB14
#define LED_GREEN_PIN TRISBbits.TRISB14
#define LED_GREEN_PULLUP CNPUBbits.CNPUB14

#define LED_BLUE LATBbits.LATB15
#define LED_BLUE_PIN TRISBbits.TRISB15
#define LED_BLUE_PULLUP CNPUBbits.CNPUB15
  
#define IN 1
#define OUT 0

#define ON 1
#define OFF 0

#define RED 0
#define GREEN 1
#define BLUE 2


void RGB_LED_init(void) {
    LED_RED_PIN = OUT;
    LED_RED_PULLUP = ON;
    LED_RED = OFF;
    
    LED_GREEN_PIN = OUT;
    LED_GREEN_PULLUP = ON;
    LED_GREEN = OFF;
    
    LED_BLUE_PIN = OUT;
    LED_BLUE_PULLUP = ON;
    LED_BLUE = OFF;
}

void RGB_Color(unsigned char color) {
    
    if (color == RED) {
        LED_RED = ON;
        LED_GREEN = OFF;
        LED_BLUE = OFF;
    }
    if (color == GREEN) {
        LED_RED = OFF;
        LED_GREEN = ON;
        LED_BLUE = OFF;
    }
    if (color == BLUE) {
        LED_RED = OFF;
        LED_GREEN = OFF;
        LED_BLUE = ON;
    }
    
}
