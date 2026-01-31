.setcpu "6502"

.org $0200

lda #1
sta $fe ; Initialize LSFR with seed: 1

loop:
    jsr print_parallel
    jmp loop

print_parallel:
    ldx #0
    print_parallel_loop:
        lda $fe
        jsr lfsr
        sta $FB00,x
        sta $FC00,x
        sta $FD00,x
        sta $FE00,x
        inx
        bne print_parallel_loop
    rts

lfsr:
    lda $fe
    lsr
    bcc skip
    eor #$B8
    skip: sta $fe
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
