.bss			; still not really sure what this does... compiles tho!

numbits1: .space 2	; reserve some space, 2bytes worth, for variables
numbits2: .space 2
numdone: .space 2

.text			; preprocessor directive

.global __reset		; start here on reset!!!

__reset:
   mov #2, W1		; constant to check for when program is complete
   mov #0x4FC2, W0	; first number to count number of 1's (binary)
   call countbits	; call subroutine now that working reg is loaded
   mov W0, numbits1	; result is stored in working reg, move to file reg
   mov #0xF257, W0	; assignment says "#F257", but that makes no sense..
   call countbits	; call subroutine with second number loaded in reg
   mov W0, numbits2	; move result to file reg again
   mov W1, numdone	; move constant indicating program completion into reg
    
done:
   goto done		; sit, forever, alone.. :(
   
countbits:
    push W2		; save those reg values!!
    push W3
    push W4
    clr W2		; clr what we're working with
    clr W3
    mov #0x10, W4	; compare iteration against 16 (kinda like for() )
count_accum:		; want a separate branch in the subroutine to repeat
    btsc W0, #0		; check if the LSb is a zero or one
    inc W2, W2		; W2++ if 1
    lsr W0, W0		; logical shift right to place next bit as LSb
    inc W3, W3		; W3++ to keep track of which bit we're on
    cpbeq W3, W4, count_end	; if we've tested all 16 bits, finish subroutine
    goto count_accum	; if we haven't finished yet, do it again!
count_end:		; ... and a separate branch to end
    mov W2, W0		; calling routine expects num of 1's in W0 (output)
    pop W4		; pop, pop, pop!
    pop W3
    pop W2
    RETURN		; back to calling routine
    



