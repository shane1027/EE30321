
#include "xc.h"

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

void kill1s(){
    unsigned int i,j;
    
    for(i=0;i<5;i++) {
        for(j=0;j<60000;j++){
            Nop();
        }
    }
}
int main(void) {
    LED_RED_PIN = OUT;
    LED_RED_PULLUP = ON;
    LED_RED = OFF;
    
    LED_GREEN_PIN = OUT;
    LED_GREEN_PULLUP = ON;
    LED_GREEN = OFF;
    
    LED_BLUE_PIN = OUT;
    LED_BLUE_PULLUP = ON;
    LED_BLUE = OFF;
    

    while(1) {
        LED_RED = ON;
        LED_GREEN = OFF;
        LED_BLUE = OFF;
        kill1s();
        LED_RED = OFF;
        LED_GREEN = ON;
        LED_BLUE = OFF;
        kill1s();
        LED_RED = OFF;
        LED_GREEN = OFF;
        LED_BLUE = ON;
        kill1s();
   }
    
    
    return 0;
}
