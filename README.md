# EE30321
Assembly and C code for the PIC24 written for my Embedded Systems class.

University of Notre Dame
Department of Electrical Engineering
EE 30321 Embedded Systems Stevenson
Assignment #6
Light Dimmer
The object of this assignment is to build a light dimmer using the PIC24E prototype system
we have built. We will use two push button switches and an LED. The two push buttons should be
wired to two digital I/O ports on the PIC24E with pull up resistors (10KΩ) and the LED should
be wired up as before to a digital I/O pin on the PIC24 (i.e. with 470Ω pull-up resistor). You can
use any of the Digital I/O pins that are available.
Initial setup - configure the PIC24E to read the push buttons (i.e. set I/O pins to be inputs)
and to control the LED state (i.e. set I/O pin are output). You can use the default clock settings.
LED - We can control the apparent brightness of the LED by controlling the percentage of
time that it is on and the on/off frequncy. If the LED is on 100% of the time it will be at full
brightness, if on 50% it will be at half brightness, 25%, quarter brightness, etc. You should “blink”
the LED at well above the cutoff frequency of the human visual system ( 50Hz) so that the LED
no longer appears to be blinking, just appears to be dimmer.
Push Buttons - By reading the digital I/O pins for the push buttons you should be able to
tell when the push button is pressed. Make sure that you check the I/O pin frequency enough so
that you don’t miss any pushes.
Control Logic - Think of one button as the “up” button and the other as a “down” button.
Your prototype should function as follows
• pressing the “up” button will cause the LED to get brighter
• pressing the “down” button will cause the LED to get dimmer
• pressing and holding the “up” button will cause the LED to get brighter at a reasonable rate
until you reach full brightness.
• pressing and holding the “down” button will cause the LED to get dimmer at a reasonable
rate until you reach completely off.
• pressing both buttons will toggle the on/off state of the LED, i.e. if LED is on and both
buttons are pressed it should turn off, if the LED is off and both buttons are pressed it
should turn back on at the previously brightness)
• if the LED is dimmed to off (e.g. by pressing and holding the down button) and both buttons
are then pressed, the LED should be turn on a full brightness
Assignment
1. Build and test the hardware system. Turn in a program that you used to test your hardware.
2. Implement the control logic in assembly language. Turn in the program and demonstrate the
system to the instructor or TA.
3. Implement the control logic in C. Turn in the program and demonstrate the system to the
instructor or TA.
