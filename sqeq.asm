IN 
POP ax

IN
POP bx

IN
POP cx

; main

CALL :1
POP dx ; get discriminant

; x1
PUSH dx
SQRT
PUSH bx
SUB
PUSH ax
PUSH 2
MUL
DIV
OUT

; x2
PUSH dx
SQRT
PUSH -1
MUL
PUSH bx
SUB
PUSH ax
PUSH 2
MUL
DIV
OUT

HLT

; disk
:1
PUSH bx
PUSH bx
MUL
PUSH 4
PUSH ax
PUSH cx
MUL
MUL
SUB
RET