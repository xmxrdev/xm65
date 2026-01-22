.setcpu "6502"

.org $0200

;;; Testing addressing modes

; Draw 4 pixels spaced by 1 pixel on the top-left corner of the screen
    lda #$ff ; White
    ldx #$01
    sta $fb00,x
    ldx #$02
    sta $fb00,x
    ldx #$03
    sta $fb00,x
    ldx #$04
    sta $fb00,x

; Testing indirect mode
    COLOR_RED   := %11100000
    COLOR_GREEN := %00011100
    COLOR_BLUE  := %00000011

    lda #$20
    sta $00
    lda #$fb
    sta $01

    ldx #$00

    lda #COLOR_RED
    sta ($00,x) ; Store color in $fb20 indirectly

    lda #$21
    sta $00

    lda #COLOR_GREEN
    sta ($00,x) ; Store color in $fb21 indirectly

    lda #$fc
    sta $01

    lda #COLOR_BLUE
    sta ($00,x) ; Store color in $fc21 indirectly

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
