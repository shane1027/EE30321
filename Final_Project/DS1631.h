/*
 * File:   DS1631.h
 * Author: Shane Ryan - sryan8@nd.edu
 * Purpose:  Header file to facilitate easy operation of a DS1631 temperature
 * sensor via I2C on the PIC24 microcontroller.  Includes UART control.
 *
 * Created on April 15, 2016, 12:52 AM
 */

#define BAUDRATE 19200
#define FCY 7370000/2
#define ZERO 48
#define NINE 58

#define ADDRESS_READ 0x91
#define ADDRESS_WRITE 0x90
#define START_CONVERT 0x51
#define READ_TEMP 0xAA
#define ACCESS_CONFIG 0xAC
#define CONFIG_DATA 0x0C

#include <stdio.h>

void startI2C1(void);
int putI2C1(char data);
void restartI2C1(void);
void stopI2C1(void);
void startConvertDS1631(void);
void configI2C1(void);
void configDS1631(void);
unsigned int readTempDS1631(void);
char getI2C1(char ack2send);
void configPins(void);
void configUART1(void);

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

char getI2C1(char ack2send) {
    char inByte;
    
    while(I2C1CON & 0x1F);
    I2C1CONbits.RCEN = 1;
    while(!I2C1STATbits.RBF);
    inByte = I2C1RCV;
    
    while(I2C1CON & 0x1F);
    I2C1CONbits.ACKDT = ack2send;
    I2C1CONbits.ACKEN = 1;
    while(I2C1CONbits.ACKEN);
    
    return(inByte);
}

unsigned int readTempDS1631(void) {
    unsigned char temp_hi = 0;
    unsigned char temp_lo = 0;
    
    startI2C1();
    putI2C1(ADDRESS_WRITE);
    putI2C1(READ_TEMP);
    restartI2C1();
    putI2C1(ADDRESS_READ);
    temp_hi = getI2C1(0);
    temp_lo = getI2C1(1);
    stopI2C1();
    return(((temp_hi<<8) | temp_lo)/256);
}

void configDS1631(void) {
    startI2C1();
    putI2C1(ADDRESS_WRITE);
    putI2C1(ACCESS_CONFIG);
    putI2C1(CONFIG_DATA);
    stopI2C1();
}    

void configI2C1(void) {
    I2C1BRG = 0x1F;
    I2C1CONbits.I2CEN = 1;
}

void startConvertDS1631(void) {
    startI2C1();
    putI2C1(ADDRESS_WRITE);
    putI2C1(READ_TEMP);
    stopI2C1();
}

void stopI2C1(void) {
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN);
}

int putI2C1(char data) {
    I2C1TRN = data;
    while(I2C1STATbits.TRSTAT);
    return(I2C1STATbits.ACKSTAT);
}

void startI2C1(void) {
    I2C1CONbits.SEN = 1;
    while(I2C1CONbits.SEN);
}

void restartI2C1(void) {
    I2C1CONbits.RSEN = 1;
    while(I2C1CONbits.RSEN);
}