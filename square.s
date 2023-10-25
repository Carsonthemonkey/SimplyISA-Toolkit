# Lab 5: Square
# written by Kat Berge, Carson Reader, Maximum DeCorso
# There is an error with this as of right now according to Wyeth. Apparently LD can only load contents of registers into another register. This means we must initally load all constants into X0 using LDI and then transfer them to where we want them. The assembler will need to be updated to support this change as well.   

    BRA start
IN: 42
OUT: 0

start: LD X1, IN       # load input value in input a register
    LDI -1             # Store the constant -1 in X0
    LD X2, IN          # also load value into X2 to keep track of count
    ADD X2, X0         # decrement count to avoid off by one error
square: ADD X3, X1     # add X1 to the overall sum (X3)
    ADD X2, X0         # decrement count using the -1 constant
    BZ end             # If x1 is zero (count is done) go to L3 
    BRA square         # If x1 is not zero, go back to the beginning of L2
end: ST X3, OUT        # load X0 value from the register into out: 
    HLT                # Stop program 