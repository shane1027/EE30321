/*
 * File:   buttons.h
 * Author: Shane Ryan
 * Separate Interrupt Service Routines to deal with button presses.
 * 
 * 
 * Created on April 4, 2016, 12:05 PM
 */

void _ISR _CNInterrupt(void) {

    TMR3 = 0x00;
    IEC0bits.T3IE = 1;
    IEC1bits.CNIE = 0;
    IFS1bits.CNIF = 0;

}

void _ISR _T3Interrupt(void) {

    if (!PORTAbits.RA0 && !PORTAbits.RA1) {

        my_puts(state_update[1]); // ack the two-button press on UART

        switch (mode) {
            case 0:
                mode = 1;
                my_puts(state_update[10]);
                my_puts(state_update[7]);
                LCDUserScreen(mode, high_temp, low_temp);
                break;
            case 1:
                mode = 2;
                my_puts(state_update[8]);
                LCDUserScreen(mode, high_temp, low_temp);
                break;
            case 2:
                my_puts(state_update[9]);
                mode = 0;
                UpdateTemp();
                LCDTemp(current_temp, high_temp, low_temp, condition);
                break;
        }

    }


    if (!PORTAbits.RA0 && mode == 1) {
        my_puts(state_update[2]);
        if (high_temp < 99) {
            LCDUserScreen(mode, ++high_temp, low_temp);
        } else {
            my_puts(state_update[11]);
        }
    }
    if (!PORTAbits.RA0 && mode == 2) {
        my_puts(state_update[2]);
        if (low_temp < high_temp) {
            LCDUserScreen(mode, high_temp, ++low_temp);
        } else {
            my_puts(state_update[11]);
        }
    }
    if (!PORTAbits.RA1 && mode == 1) {
        my_puts(state_update[3]);
        if (high_temp > low_temp) {
            LCDUserScreen(mode, --high_temp, low_temp);
        } else {
            my_puts(state_update[11]);
        }
    }
    if (!PORTAbits.RA1 && mode == 2) {
        my_puts(state_update[3]);
        if (low_temp > 0) {
            LCDUserScreen(mode, high_temp, --low_temp);
        }
        else {
            my_puts(state_update[11]);
        }
    }

    /*  Don't clear flag if a single button is held (allows fast scrolling */
    if (!PORTAbits.RA0 && !PORTAbits.RA1) {
        IFS1bits.CNIF = 0;
    }
    else if (!PORTAbits.RA0 || !PORTAbits.RA1) {
        IFS1bits.CNIF = 1;
    }

    else {
        IFS1bits.CNIF = 0;
    }

    IFS0bits.T3IF = 0;
    IEC1bits.CNIE = 1;
    IEC0bits.T3IE = 0;

}