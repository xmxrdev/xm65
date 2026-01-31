.setcpu "6502"

.org $0200

lda #1
sta $fe       ; Initialize LSFR with seed: 1

loop: 
    lda $fe       ;|
    jsr lfsr      ;|
    sta $00       ;|- 0x00 = #random(0 - 0xFF)

    lda $fe       ;|
    jsr lfsr      ;|
    and #$3       ;|
    clc           ;|
    adc #$FB      ;|
    sta $01       ;| 0x01 = #random(0xFB - 0xFE)

    lda $fe       ;|
    jsr lfsr      ;|
    ldy #$0       ;|
    sta ($00),y   ;| Print #random(0 - 255) to the whole screen

    jmp loop

lfsr:
    lda $fe       ;|
    lsr           ;|
    bcc skip      ;|
    eor #$B8      ;| Feedback mask
    skip: sta $fe ;|
    rts

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

.word NMI
.word RESET
.word IRQ
