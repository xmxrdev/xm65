.setcpu "6502"

.org $0200

lda #0
ldx #0

loop:
    sta $fb00,x
    adc #1
    inx
    jmp loop

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
