/*
 * File:   pwm_gen.c
 * Author: Shane Ryan
 * Implement a simple two-button control system for producing
 * a 100Hz PWM signal with variable duty cycle.
 * Created on April 4, 2016, 12:05 PM
 */

#include "p24EP128GP202.h"
#include "my_config.h"

#define MAX_dc 0x1CC
// unscaled T value for 1000Hz signal:  0x3E7F

void pin_setup(void);
void pwm_setup(void);

volatile unsigned int duty_cycle = MAX_dc;
volatile unsigned int last_scale = 0;
volatile unsigned int led_on = 0;


void _ISR _T3Interrupt(void) {
    
    if (!PORTAbits.RA0 && !PORTAbits.RA1) {
        if (duty_cycle == 0 && led_on) {
            duty_cycle = MAX_dc;
        }
        else if (led_on) {
            last_scale = duty_cycle;
            led_on = 0;
        }
        else if (!led_on) {
            duty_cycle = last_scale;
            led_on = 1;
        }
    }
    else if (led_on && !PORTAbits.RA0) {
        if (duty_cycle < MAX_dc) {
            duty_cycle = duty_cycle + 2;
        }
    }
    else if (led_on && !PORTAbits.RA1) {
        if (duty_cycle > 0) {
            duty_cycle = duty_cycle - 2;
        }
    }

    if (led_on) {
        OC1R = duty_cycle;
    }
    else {
        OC1R = 0;
    }
  
    if (!PORTAbits.RA0 || !PORTAbits.RA1) {
        IFS1bits.CNIF = 1;
    }
        IFS0bits.T3IF = 0;
        IEC1bits.CNIE = 1;
        IEC0bits.T3IE = 0;

}

void _ISR _CNInterrupt(void) {
    
    
    TMR3 = 0x00;    
    IEC0bits.T3IE = 1;

    IEC1bits.CNIE = 0;

    IFS1bits.CNIF = 0;
    
}

int main(void) {
        
    pin_setup();
    pwm_setup();
    
    while(1) {
        
        /*  Placeholder for code to utilize the microcontroller   */
        
    }
    
    return 0;
}


void pin_setup(void) {
        
    /*  Disable Analog Priority on RA0 and RA1  */
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 0;
    
   /*  Set pins RA0 and RA1 to Inputs  */
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;

    /*  Set pin RP36 to Output for LED   */
    RPOR1bits.RP36R = 0;            // remap enabled
    LATBbits.LATB4 = 1;             // pin RP36 low
    TRISBbits.TRISB4 = 0;           // pin RP36 output enabled
    RPOR1bits.RP36R0 = 0;
    RPOR1bits.RP36R1 = 0;
    RPOR1bits.RP36R2 = 0;
    RPOR1bits.RP36R3 = 0;
    RPOR1bits.RP36R4 = 1;           // RP36R [5:0] uses 0x010000 for OC1
    RPOR1bits.RP36R5 = 0;
    
    /*  Enable Change Notification Interrupts on Inputs */
    IEC1bits.CNIE = 1;
    CNENAbits.CNIEA0 = 1;
    CNENAbits.CNIEA1 = 1;
    IPC4bits.CNIP = 5;
    
    /*  Internal Pullup Resistors on Inputs  */
    CNPUAbits.CNPUA0 = 1;
    CNPUAbits.CNPUA1 = 1;
}


void pwm_setup(void) {
    
    /*  Turn on Timer2 module via
     *  Peripheral Module Disable Control Reg 1 */
    PMD1bits.T2MD = 0;
    PMD1bits.T3MD = 0;
    // Chosen b/c only Timer2 and Timer3 can be time bases for OC mod
    
    /*  Configure Timer2 for OCM time base    */
    T2CONbits.TON = 0;
    T2CONbits.TGATE = 0;
    T2CONbits.TCKPS = 0b01;
    T2CONbits.TCS = 0;
    T2CONbits.TSIDL = 0;
    T2CONbits.T32 = 0;
    PR2 = MAX_dc;             // Tosc * pre-scaler * PR2 = Tpwm
    TMR2 = 0x00;
    T2CONbits.TON = 1;
    
    /*  Configure Timer3 for delaying between button presses    */
    T3CONbits.TON = 0;
    T3CONbits.TGATE = 0;
    T3CONbits.TCKPS = 0b10; // pre-scaler set to 64 to keep PR3 16bits or less
    T3CONbits.TCS = 0;
    T3CONbits.TSIDL = 0;
    PR3 = MAX_dc * 8;
    TMR3 = 0x00;
    T3CONbits.TON = 1;
    IPC2bits.T3IP = 6;
    IEC0bits.T3IE = 0;


    /*  Turn on Output Comparison module 1 via
     * Peripheral Module Disable Control Reg 2  */
    PMD2bits.OC1MD = 0;
    
    /*  Configure Output Comparison module 1    */
    OC1CON1bits.OCM = 0b000;        // disable module to write to control reg
    OC1CON1bits.OCTSEL = 0b000;     // sets timer2 to time base
    OC1CON1bits.OCSIDL = 0;         // continues to function in idle mode
    OC1R = 0;                   // zero brightness to start
    OC1RS = 0;                  // zero in secondary reg
    OC1CON1bits.OCM = 0b110;        // enable mod w/ edge-aligned PWM operation   
}