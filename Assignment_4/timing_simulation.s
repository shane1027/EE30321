.text   ; this is an Assembler directive - doesn't go on uC

.global __reset   ; tell the uC to go here on reset
__reset:
    mov #0x0100, W0
    mov #0x0010, W1

loop:
    add W0, W1, W2
    add.b W1, #0x03, W1
    mov W2, W0
    goto loop


