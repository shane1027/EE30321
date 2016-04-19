.bss

; numentries: .space 2
; startarray: .space 2
; startvalue: .space 2
; maxvalue: .space 2
 
.text

.global __reset
 
__reset:
    mov #0x14, W0	    ; save the number of entries in W1
    mov W0, W1	    
    mov #0x01, W0	    ; save the starting array value in W3
    mov W0, W3
    mov #0x1000, W0	    ; save the array's start address in W2
    mov W0, W2		    ; W2 remains untouched so subroutine can use
    
gen_array:
    mov W3, [W0++]	    ; move starting array value into first array addr.
    cpbeq W3, W1, eval_array	; check if we've entered all 20 values
    inc W3, W3		    ; more values needed! inc W3 as next value
    goto gen_array	    ; repeat, adding W3 into array
    
eval_array:
    mov [--W0], W4
    call array_max

done:
    goto done

array_max:		    ; W0 = max value, W3 = iteration counter
    push W1		    ; W4 = arithmetic results storage
    push W2
    push W3		    ; push push pushhh
    push W4
    clr W0		    ; don't clr W1 (numentries) or W2 (array address)
    clr W3
    clr W4
test_max:
    sub W0, [W2++], W4	    ; subtract (current max value - current array value)
    btsc W4, #0xF	    ; if result is positive, no new max found
    mov [W2], W0	    ; if negative, add current value to new max!
    inc W3, W3		    ; increment iteration counter
    cpbeq W3, W1, max_end   ; check to see if array is at end
    goto test_max	    ; if not, compare next value!
max_end:
    pop W4		    ; pop pop poppp
    pop W3
    pop W2
    pop W1
    RETURN		    ; jump back to calling routine, with max in W0

