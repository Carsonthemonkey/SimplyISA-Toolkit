# Simply ISA lab

This is the code for architecture lab 5.

## Assembler
The assembler can be used to assemble SimplyISA assembly into bytecode. It can be compiled by running `make assembler`. It takes two command line arguments:

1. **Target File**: This is the SimplyISA code to be compiled to bytecode
2. **Output (optional)**: This is the file name to output the assembled bytecode too. default is `a.bin`

For example, to assemble a SimplyISA file called `square.s` you would run `./assembler square.s square.bin` to  output a bytecode file `square.bin`.

### Syntax
Note the following syntax specific to this assembler:

- Instructions must be seperated with newlines
- Labels must end with a `:`
- Labels must include an instruction or constant *on the same line*
- brackets (`[]`) are neither required nor supported
- `LDI` should only be provided with one argument, the memory address. The `X0` argument of `LDI` is implicit.
- Comments must begin with an `#`. They can be on the same line as an instruction or on their own lines
- Constants must be represented in decimal (base 10) 
- Indentation and empty lines are ignored, and can be used at your discretion.

## Simulator
The simulator can be used to run SimplyISA bytecode. It can be compiled by running `make simfun`. It takes two command line arguments:

1. **Target File**: The target bytecode file to execute.
2. **Verbose (optional)**: provide `-v` as the second argument to run the simulator in verbose mode. This will output each step as it is executed instead of just the final program state. Default is off.

For example, you would run `./simfun square.bin -v` to run the the `square.bin` binary in verbose mode.
## Build commands
- Run `make` to compile the assembler and simulator
- `make clean` removes the compiled files, and any binary files ending in `.bin`.