.setcpu "6502"

.org $0200

; Testing addressing modes
lda #$ff
sta $fa00

brk

; Interrupt handlers
RESET:
    sei
    cld
    ldx #$ff
    txs
    cli
    jmp $0300

NMI:
    rti

IRQ:
    rti

.word NMI
.word RESET
.word IRQ
