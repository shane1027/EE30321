/*
 * File:   Final Project.c
 * Author: Shane Ryan - sryan8@nd.edu
 *
 * Created on April 27, 2016, 1:44 AM
 */

unsigned int current_temp = 0;
unsigned int previous_temp = 0;
unsigned int high_temp = 24;
unsigned int low_temp = 20;

unsigned char mode = 0;
unsigned char condition = 0;

void UpdateTemp(void);
void Initialize(void);

#include "p24EP128GP202.h"
#include "my_config.h"
#include "UART.h"
#include "LCD_Control.h"
#include "DS1631.h"
#include "RGB_LED.h"
#include "Buttons.h"
#include "ISRs.h"

#define NONE 0
#define HEATING 1
#define COOLING 2

int main(void) {

    Initialize();

    while (1) {
        if (!mode) {
            UpdateTemp();
        }
    }

    return 0;
}

void UpdateTemp(void) {
    current_temp = readTempDS1631();
    char temp_buff[6];
    itoa(current_temp, temp_buff);
    
    if (current_temp > high_temp) {
        condition = 2;
        RGB_Color(BLUE);
    }

    else if (current_temp < low_temp) {
        condition = HEATING;
        RGB_Color(RED);
    }
    else {
        condition = NONE;
        RGB_Color(GREEN);
    }

    if (current_temp != previous_temp) {
        my_puts(state_update[12]);
        outChar1(temp_buff[0]);
        outChar1(temp_buff[1]);
        LCDTemp(current_temp, high_temp, low_temp, condition);
        my_puts(state_update[0]);
    }
    
    previous_temp = current_temp;
}

void Initialize(void) {
    pin_setup(); // setup pins for buttons and PWM output
    timer_setup(); // config timer module for button interrupts   
    configPins(); // setup inputs / outpus, remapped pins, etc.
    configUART1(); // config UART module
    configI2C1(); // config I2C module
    configDS1631(); // send startup info to temp sensor via I2C
    startConvertDS1631(); // tell temp sensor to begin conversion
    LCDInit(); // initialize LCD screen over SPI and clear
    RGB_LED_init(); // setup pins for RGB LED output
}
