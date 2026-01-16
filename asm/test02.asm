clc
adc #$01 ; Expecting 0x01
sec
adc #$01 ; Expecting 0x03

; Interrupt handlers
RESET:
    sei
    cld
    ldx #$ff
    txs
    cli

NMI:
    rti

IRQ:
    rti

; Vectors
.word NMI
.word RESET
.word IRQ

; Assemble & Link
; ca65 test02.asm --cpu 6502; cl65 -t none -C memory.cfg test02.asm -o test02.bin
