.setcpu "6502"

.org $0200

; Put a pixel on the top-left corner of the screen
lda #$ff
sta $fb00

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
