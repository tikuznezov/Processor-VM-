IN

CALL :1

OUT


:1
POP ax
PUSH ax
PUSH 1
JNE :2


PUSH 1 ; случай равенства единице
RET

:2     ; не равно единице
PUSH ax
PUSH ax
PUSH 1
SUB
CALL :1
MUL
RET
