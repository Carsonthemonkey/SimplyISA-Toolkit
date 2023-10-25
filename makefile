COMPILER = gcc
CFLAGS = -Wall

all: assembler

assembler: assembler.c
	$(COMPILER) $(CFLAGS) -o assembler assembler.c

clean:
	@rm -rf assembler *.bin