/*
 * File:   hardware_test.c
 * Author: Shane Ryan
 * Setup a quick program to ensure proper hardware
 * functionality (buttons, LED blink).
 * Created on March 22, 2016, 9:01 PM
 */


#include "p24EP128GP202.h"

void kill_time(void) {
    int i;
    for (i=0;i<30000;i++) {
    }
}

int main(void) {
    TRISBbits.TRISB5 = 0;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 0;
    
    int i = 0;
    int j = 0;
    
    while(1) {
        i = PORTAbits.RA0;
        j = PORTAbits.RA1;
        
        if (i | j) {
            LATBbits.LATB5 = 1;
        }
        kill_time();
        
        LATBbits.LATB5 = 0;
        
        kill_time();
    }
}
