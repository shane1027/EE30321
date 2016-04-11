.bss    ; not sure what this does
.text   ; this is an Assembler directive - doesn't go on uC

.global __reset   ; tell the uC to go here on reset
__reset:
    .equ num, #0xA4EF	; define the constant for counting 1's
    mov #num, W1	; move it on in to a register
    clr W0		; clear the ol' workin' reg

loop:
    btsc W1, #0		; bit test bit 0 and skip next line if clear
    inc W0, W0		; increment W0 if previous bit was 1
    lsr W1, W1		; logical shift right and store in W1
    goto loop		; restart loop





