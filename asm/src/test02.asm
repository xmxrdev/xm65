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
.word IRQ
.word RESET
.word NMI
