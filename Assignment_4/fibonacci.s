.bss
.text

.global __reset
__reset:
    .equ init, #0x1000
    mov #init, W1
    inc2 W1, W2
    clr [W1]
    clr [W2]
    mov #0x01, W0
    mov W0, [W2]
    clr W0
    goto fibon
    
fibon:
    mov [W2++], W0
    add W0, [W1++], [W2]
    ; mov [W2], W0
    bra c, done
    goto fibon
   
done:
    goto done

