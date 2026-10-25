IN 
POP ax
IN
POP bx
IN
POP cx

; main

PUSH ax
PUSH 0
JE :1

CALL :5
POP dx ; get discriminant

PUSH 0
PUSH dx
JE :2 ; no solutions
PUSH dx
PUSH 0
JB :3 ; one solution
PUSH dx
PUSH 0
JA :4 ; two solutions
HLT

:4 ; two solutions
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

:2 ; one solution
; x2
PUSH 0
PUSH bx
SUB
PUSH ax
PUSH 2
MUL
DIV
OUT
HLT

:3 ; no solutions
PUSH 0
PUSH 0
PUSH 0
OUT
OUT
OUT
HLT

:1 ; a == 0
PUSH bx
PUSH 0
JE :6
PUSH 0
PUSH cx
SUB
PUSH bx
DIV
OUT
HLT


:6 ; a == 0, b == 0
PUSH 0
PUSH cx
JNE :3
PUSH 8
PUSH 8
PUSH 8
OUT
OUT
OUT
HLT

; end main

; disk
:5
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