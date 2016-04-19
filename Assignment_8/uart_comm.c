/*
 * File:   uart_comm.c
 * Author: Shane Ryan - sryan8@nd.edu
 * 
 * Configure and utilize the on-board UART to
 * communicate with a serial terminal (minicom!)
 *  *
 * Created on April 12, 2016, 10:21 AM
 */

#define BAUDRATE 19200
#define FCY 7370000/2
#define ZERO 48
#define NINE 58

void configPins(void);
void configUART1(void);
void outChar1(char c);

#include "xc.h"

int main(void) {
    
    configPins();
    configUART1();
    
    int j = ZERO;
    int k = ZERO;
    int l = ZERO;

    while(1) {
        for (j = ZERO; j < NINE; j++) {
            for (k = ZERO; k < NINE; k++) {
                for (l = ZERO; l < NINE; l++) {
                    outChar1(ZERO);
                    outChar1(j);
                    outChar1(k);
                    outChar1('.');
                    outChar1(l);
                    outChar1('\n');
                    outChar1('\r');
                }
            }
        }
        outChar1(ZERO+1);
        outChar1(ZERO);
        outChar1(ZERO);
        outChar1('.');
        outChar1(ZERO);
        outChar1('\n');
        outChar1('\r');
    }
    
    return 0;
}

void configPins(void) {
    RPINR18bits.U1RXR = 20;     // map UART1 RX to remappable pin RP20 (pin 12)
    RPOR1bits.RP36R = 1;        // map UART1 TX to remappable pin RP36 (pin 11)          
}

void configUART1(void) {
    /*  Calculate the Baud Rate generator value     */
    int brg = (FCY/BAUDRATE/4)-1;
    U1MODEbits.BRGH = 1;        // High speed mode
    U1BRG = brg;                // store the value in the register
    
    U1MODEbits.PDSEL = 0;       // 8 bit data, no parity
    U1MODEbits.STSEL = 0;       // 1 stop bit
    
    U1MODEbits.UEN = 0b00;      // UxTX and UxRX pins are enabled and used;
                                // also, no flow control pins
    U1MODEbits.UARTEN = 1;      // enable UART RX/TX
    U1STAbits.UTXEN = 1;        //Enable the transmitter
 
}

void outChar1(char c){
    while(U1STAbits.UTXBF);     // wait for transmit buffer to be empty
    U1TXREG = c;                // write character to transmit register
}

int inChar1(void) {
    while(!U1STAbits.URXDA);    // data received yet
    return(U1RXREG);            // return data in receive buffer
}
