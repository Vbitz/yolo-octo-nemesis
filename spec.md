Instructions
----------------------

invalid (0x00) : causes a error
noop (0x01) : has no effects aside from adding to the program counter
hlt (0x02) : stops the VM
brk (0x03) : debug break

mov (0x10) : moves value to register
mov (0x11) : moves register to register

add (0x20) : adds rega to regb and puts the result in a

Register
-----------------------

r0-r3 (size 16bits) : GP Registers


Memory Map
-----------------------
size: 1024 (0x400)

0x000-0x07f : system reserved
0x080-0x18b : program memory
0x18c-0x400 : program scratch