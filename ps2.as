.entry LIST
.extern1 fn1
MAIN: add r3, LIST
jsr fn1
LOOP: prn #48
lea STR, r6
inc r6
mov *r6, L3
sub r1, r4
macr eee
lea STR, r6
lea STR, r6
lea STR, r6
endmacr
cmp r3, #-6
eee
bne END
add r7, *r6
clr K
sub L3, L3
.entry MAIN
jmp LOOP
END: stop
STR: .string "abcd"
LIST: .data 6, -9
.data -100
K: .data 31
.extern L3