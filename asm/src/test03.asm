.setcpu "6502"

.org $0200

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
    jmp $0200

NMI:
    rti

IRQ:
    rti

; TOTAL: 13 instructions

.word NMI
.word RESET
.word IRQ
