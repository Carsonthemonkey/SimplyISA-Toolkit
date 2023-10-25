COMPILER = gcc
CFLAGS = -Wall

all: assembler simfun

assembler: assembler.c
	$(COMPILER) $(CFLAGS) -o assembler assembler.c

simfun: simfun.c
	$(COMPILER) $(CFLAGS) -o simfun simfun.c

clean:
	@rm -rf assembler simfun *.bin