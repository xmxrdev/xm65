asm:
	ca65 --cpu 6502 asm/test04.asm -o asm/build/test04.o; ld65 -o asm/bin/test04.bin asm/build/test04.o -C asm/memory.cfg

.PHONY: asm clean
