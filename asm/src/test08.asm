.setcpu "6502"

.org $0200

; Code provided by ChatGPT 5.2

; ==============================
; Zero page
; ==============================
ZP_PTR = $00     ; $00 = low, $01 = high
LFSR   = $FE     ; LFSR state (must never be 0)

; ==============================
; Init (RUN ONCE)
; ==============================
    lda #$01
    sta LFSR

    lda #$FB
    sta ZP_PTR+1
    lda #$00
    sta ZP_PTR

; ==============================
; Main loop (NO RESETS)
; ==============================
main_loop:
    jsr rand8
    ldy #$00
    sta (ZP_PTR),y

    inc ZP_PTR
    bne next
    inc ZP_PTR+1
    lda ZP_PTR+1
    cmp #$FF        ; wrap after $FEFF
    bne next
    lda #$FB
    sta ZP_PTR+1

next:
    jmp main_loop

; ==============================
; 8-bit Galois LFSR
; ==============================
rand8:
    lda LFSR
    lsr
    bcc no_xor
    eor #$B8
no_xor:
    sta LFSR
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
