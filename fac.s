# Computes the factorial of an input (as long as it is less than 6 or else the product won't fit in the 8 bit registers)
# @author Carson Reader

BRA start
IN: 5
OUT: 0
start: LDI IN
    LD X1, X0       # load input into X1
    LDI OUT         # switch memory pointer to OUT
    ST X1, X0       # store inital number in memory
loop: LDI IN    
    LD X1, X0       # load current num into X1
    LD X2, X0       # also load into X1
    LDI -1          # load -1 into X0
    ADD X1, X0      # decrement X1
    BZ end          # If it has reached 0, end
    ADD X2, X0      # decrement X2
    LDI IN          # switch pointer to in
    ST X1, X0       # store current counter number in IN
    BRA skip        # skip past outpost
outpost: BRA loop   # a place for weary travelers to rest on their way back from mul (I don't have enough bits to branch directly from mul to loop)
end: HLT        
skip: LDI OUT       # switch pointer to out
    LD X1, X0       # load current total into X1
    LDI IN          # switch pointer to input
    LD X3, X0       # load X3 with current number
    BRA mul         # multiply X2 (current num) and X1 (running total)

store: LDI OUT      # switch memory pointer to OUT
    ST X2, X0       # stores multiplication of X2 and X1 in OUT
    BRA outpost     # return to main loop (stopping to rest at outpost first of course)

# multiplies X2 by X1 storing result in X2
mul: LDI -1 
    ADD X1, X0
    BZ store
    ADD X2, X3
    BRA mul