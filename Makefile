CC=clang
CFLAGS=-Wall -Werror

run: emu testing.bin
	./emu testing.bin

testing.bin: testing.src assembler.js
	node assembler.js testing.src

emu: main.o
	clang -o emu main.o

clean:
	rm main.o