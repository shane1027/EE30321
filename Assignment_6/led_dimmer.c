/*
 * File:   led_dimmer.c
 * Author: Shane Ryan
 * Dim an LED, toggle on and off using two push buttons.
 * Save previous brightness state and utilize polling.
 * Created on March 22, 2016, 9:01 PM
 */


#include "p24EP128GP202.h"
#define DELAY 10
#define MAX_BRIGHT 250

void kill_time(int k) {
    int i;
    for (i=0;i<k;i++) {
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
    int state = 0;
    int brightness = MAX_BRIGHT;
    
    while(1) {
        i = PORTAbits.RA0;
        kill_time(100);
        j = PORTAbits.RA1;
        kill_time(100);
               
        if (i & j) {
            if (state & (brightness == 0)) {
                state = ~state;
                brightness = 100;
            }
            state = ~state;
            while (i | j) {
                i = PORTAbits.RA0;
                kill_time(100);
                j = PORTAbits.RA1;
                kill_time(100);
            }
        }
        
        else if (i) {
            if (brightness > (MAX_BRIGHT-2)) {
                brightness = MAX_BRIGHT;
            }
            else {
                brightness++;
            }
        }
        
        else if (j) {
            if (brightness < 2) {
                brightness = 0;
            }
            else {
                brightness--;
            }
        }
        
        if (state) {
            LATBbits.LATB5 = 1;
        }
        
        kill_time(brightness*DELAY);
        
        LATBbits.LATB5 = 0;
        
        kill_time((MAX_BRIGHT-brightness)*DELAY);
    }
}
