IN

CALL :factorial
OUT
HLT


:factorial
POP ax
PUSH ax
PUSH 1
JNE :NE_1

PUSH 1 ; случай равенства единице
RET

:NE_1     ; не равно единице
PUSH ax
PUSH ax
PUSH 1
SUB
CALL :factorial
MUL
RET