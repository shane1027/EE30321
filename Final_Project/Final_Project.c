/*
 * File:   Final Project.c
 * Author: Shane
 *
 * Created on April 27, 2016, 1:44 AM
 */


#include "p24EP128GP202.h"
#include "my_config.h"
#include "LCD_Control.h"
#include "DS1631.h"
#include "Buttons.h"

unsigned int current_temp = 28;
unsigned int high_temp = 42;
unsigned int low_temp = 8;
char str[80];

int main(void) {
    
    pin_setup();                // setup pins for buttons and PWM output
    pwm_setup();                // config PWM module to produce 500KHz signal   
    configPins();               // setup inputs / outpus, remapped pins, etc.
    configUART1();              // config UART module
    configI2C1();               // config I2C module
    configDS1631();             // send startup info to temp sensor via I2C
    LCDInit();                  // initialize LCD screen over SPI and clear
    
    startConvertDS1631();       // tell temp sensor to begin conversion
           
    int count = 0;
    
    while(1) {
        current_temp = readTempDS1631();
        sprintf(str, "Temp = %d\r", current_temp);
        puts(str);
        count++;
        if (count > 100) {
            LCDTemp(current_temp, high_temp, low_temp);
            count = 0;
        }
    }
    
    return 0;
}
