# ARC4
ARC4 Cipher (for C language)

This is block cipher with 256 bit key and 64x64 bit block
based on premutation feature


For Bacalaur Examen 2016


One round follow:
1. shift all lines, 
2. shift all columns, 
3. diffusion


How it works - 
Suppose that we have unique hardware register
where we can direct shift for everyone line or column.
We can do it parallel.

for every line and column we set "direct state"

direct state (in bits):
00 - nothing to do, 
01 - shift right, 
10 - shift left, 
11 - bit inverse line or column

and then write it "direct states" as master key

...


P.S. Sorry for my bad English :)
