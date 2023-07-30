# put 31 in ireg
stlo 0b1111
sthi 0b0001

# move ireg to r1
dupi r1

# set ireg to 0
xori ireg

# move r1 back to ireg
dupr r1

# test and, which will result in 16 in r2
dupi r2
stlo 0b0000
andi r2

# test skipping (fail if r7 contains a value)
skip r2
dupi r7

# test nand, ireg/r3 becomes -17
nand ireg
dupi r3

# test subtraction, r3 becomes -19
sthi 0b0000
stlo 0b0010
subi r3

# test or, r4 becomes 3
dupi r4
stlo 0b0001
iori r4

# test load and store, copying 16 (r2) to r5
stor r2
load r5

# test bit shifting
stlo 0b1000
dupi r6
stlo 0b0010
shif r6 # r6 contains 0b00100000
stlo 0b1011
shif r6 # r6 contains 0b00000001

# test jump (fail if r7 contains a value or loops)
jump J_ONE
dupi r7
J_TWO:
skip ireg # always succeeds
J_ONE:
jump J_TWO

jump 0 # halts

# FINAL STATE
# ireg - 0b00001011 - 0x0B
# r1   - 0b00011111 - 0x1F
# r2   - 0b00010000 - 0x10
# r3   - 0b11101101 - 0xED
# r4   - 0b00000011 - 0x03
# r5   - 0b00010000 - 0x10
# r6   - 0b00000001 - 0x01
# r7   - 0b00000000 - 0x00
