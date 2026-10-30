IN ; input number
POP ax

PUSH 0
PUSH ax
JE :zero


PUSH 1
POP bx ; bx = 1

CALL :1
OUT
HLT


:1 ; factorial
PUSH ax
PUSH bx
JNE :2 ; if ax > bx
PUSH ax ; else
RET

:2     ; if ax > bx
PUSH bx

PUSH bx
PUSH 1
ADD
POP bx ; bx++;

CALL :1
MUL

RET


:zero
PUSH 1
OUT
HLT