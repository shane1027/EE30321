; led_dimmer.s - a flashing LED
; Manipulate an LED using two push buttons in assembly.
;
; Shane Ryan
;
.text

.global __reset
__reset:
    
; configure hardware and stack    
    
LNK #0x8
BCLR TRISB, #5
BSET TRISA, #0
BSET TRISA, #1
BCLR ANSELA, #0
BCLR ANSELA, #1

; variables for loops and brightness
CLR W0
MOV W0, [W14]
CLR W0
MOV W0, [W14+2]
CLR W0
MOV W0, [W14+4]
MOV #0xFA, W0
MOV W0, [W14+6]

    
; here's the loop!

loop:
    ; read pin for increment button
    MOV PORTA, W0
    MOV.B W0, W0
    AND.B W0, #0x1, W0
    ZE W0, W0
    MOV W0, [W14]

    ; read pin for decrement button
    MOV PORTA, W0
    LSR W0, W0
    AND.B W0, #0x1, W0
    ZE W0, W0
    MOV W0, [W14+2]

    ; determine if either button is pressed
    MOV [W14+2], W0
    AND W0, [W14], W0
    CP0 W0
    BRA Z, increment_check

    ; check if brightness is 0 and LED is off    
    MOV [W14+6], W0
    BTSC W0, #15
    NEG W0, W0
    DEC W0, W0
    LSR W0, #15, W0
    MOV [W14+4], W1
    AND W0, W1, W0
    CP0 W0
    BRA Z, flip_state

    ; if so, state should be flipped to turn light back on at 100 brightness
    MOV [W14+4], W0
    COM W0, W0
    COM W1, W1
    MOV W0, [W14+4]
    MOV #0x64, W0
    MOV W0, [W14+6]

    
; flip the state since both buttons were pressed    

flip_state:
    MOV [W14+4], W0
    COM W0, W0
    COM W1, W1
    MOV W0, [W14+4]

    ; while either button is still down, keep waiting (effectively debounces)
    MOV [W14+2], W0  ; may need a line before this
    IOR W0, [W14], W0
    CP0 W0
    BRA NZ, read_pins
    BRA state_check

; read increment button to see if loop can be exited    

read_pins:
    MOV PORTA, W0
    MOV.B W0, W0
    AND.B W0, #0x1, W0
    ZE W0, W0
    MOV W0, [W14]

    ; kill a little bit of time
    MOV #0x64, W0
    RCALL kill_time

    ; read decrement button to see if loop can be exited
    MOV PORTA, W0
    LSR W0, W0
    AND.B W0, #0x1, W0
    ZE W0, W0
    MOV W0, [W14+2]

    ; kill a little bit of time
    MOV #0x64, W0
    RCALL kill_time

; if both buttons weren't pressed, check increment button
increment_check:
    MOV [W14], W0
    CP0 W0
    BRA Z, decrement_check

    ; button was pressed! check to see if we're already at max brightness
    MOV [W14+6], W1
    MOV #0xF8, W0
    SUB W1, W0, [W15]
    BRA LE, brightness_up

    ; if so, set to max brightness and move on (don't overflow brightness)
    MOV #0xFA, W0
    MOV W0, [W14+6]
    BRA state_check

; otherwise, brightness not yet at max, increment it
brightness_up:    
    MOV [W14+6], W0
    INC W0, W0
    MOV W0, [W14+6]
    BRA state_check


; if both buttons weren't pressed, and neither was increment, check decrement

decrement_check:    
    MOV [W14+2], W0
    CP0 W0
    BRA Z, state_check

    ; button was pressed! check to see if we're already at min brightness
    MOV [W14+6], W0
    SUB W0, #0x1, [W15]
    BRA GT, brightness_down

    ; if so, set to min brightness and move on (don't go negative for brightness)
    CLR W0
    MOV W0, [W14+6]
    BRA state_check

; otherwise, brightness still greater than min, so decrement it
brightness_down:
    MOV [W14+6], W0
    DEC W0, W0
    MOV W0, [W14+6]

    
; OK! now check to see if state of LED is currently on

state_check:
    MOV [W14+4], W0
    CP0 W0
    BRA Z, kill_time_on

    ; if so, enable the LED ouput pin
    BSET LATB, #5
    
    
; kill appropriate amount of time for given LED brightness
    
kill_time_on:
    MOV [W14+6], W0  ; note: time is determined by brightness
    MUL.SU W0, #10, W0  ; now multiply brightness by a constant for duty cyc
    RCALL kill_time

    ; cool, it's been on long enough.  turn it back off!
    BCLR LATB, #5

    ; now kill the appropriate amout of time while off so that the total time
    ; killed (while off and on) in this one iteration is equal to the period
	; note: the period must be such that the LED flashes above ~50Hz
    MOV [W14+6], W1
    MOV #0xFFF6, W0
    MULW.SS W1, W0, W0
    MOV W0, W1
    MOV #0x9C4, W0
    ADD W1, W0, W0
    RCALL kill_time
    BRA loop    ; do it all again!!
    
    
    
    ; setup branch to kill time
kill_time:
    LNK #0x4
    MOV W0, [W14+2]
    ; variable for killing time, then for loop
    CLR W0
    MOV W0, [W14]
    BRA time_help_1
    time_help_2:
	INC [W14], [W14]
    time_help_1:
	MOV [W14+2], W0
	MOV [W14], W1
	SUB W1, W0, [W15]
	BRA LT, time_help_2
    ULNK
    RETURN



