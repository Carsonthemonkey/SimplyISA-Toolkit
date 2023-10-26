# Lab 5: Square
# written by Kat Berge, Carson Reader, Maximum DeCorso  
    BRA start
IN: 2
OUT: 0

start: LDI IN           # pointer to val of IN into X0
    LD X2, X0           # load value in input a register
    LD X1, X0           # also load value into X1 to keep track of count
    LDI -1              # store the constant -1 in X0
square: ADD X3, X2      # add X2 to the overall sum (X3)
    ADD X1, X0          # decrement count using the -1 constant
    BZ end              # if count is zero (count is done) go to end 
    BRA square          # if count is not zero, loop again
end: LDI OUT            # load pointer to val of OUT into X0
    ST X3, X0           # load X0 value from the register into out 
    HLT