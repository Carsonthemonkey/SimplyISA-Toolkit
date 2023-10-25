# Lab 5: Square
# written by Kat Berge, Carson Reader, Maximum DeCorso  
    BRA start
IN: 42
OUT: 0

start: LDI IN           # pointer to val of IN into X0
    LD X1, X0           # load value in input a register
    LD X2, X0           # also load value into X2 to keep track of count
    LDI -1              # Store the constant -1 in X0
    ADD X2, X0          # decrement count to avoid off by one error
square: ADD X3, X1      # add X1 to the overall sum (X3)
    ADD X2, X0          # decrement count using the -1 constant
    BZ end              # If x1 is zero (count is done) go to L3 
    BRA square          # If x1 is not zero, go back to the beginning of L2
end: LDI OUT            # pointer to val of OUT into X0
    ST X3, X0           # load X0 value from the register into out 
    HLT