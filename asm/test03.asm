.setcpu "6502"

lda #$FF
clc
adc #$01 ; Expecting 0x00
sec
adc #$01 ; Expecting 0x02

brk

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

; TOTAL: 13 instructions

; Vectors
.word NMI
.word RESET
.word IRQ

; Assemble & Link
; ca65 test03.asm; cl65 -t none -C memory.cfg test03.asm -o test03.bin
