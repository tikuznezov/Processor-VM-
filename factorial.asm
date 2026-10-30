IN

; ---- if equal 0 -----
POP ax
PUSH ax
PUSH 0
JE :zero
PUSH ax
; ---------------------

CALL :factorial
OUT
HLT

;--------------------------------
:factorial
POP ax
PUSH ax
PUSH 1
JNE :NE_1

PUSH 1 ; случай равенства единице
RET
;--------------------------------

:NE_1     ; не равно единице
PUSH ax
PUSH ax
PUSH 1
SUB
CALL :factorial
MUL
RET

;--------------------------------
; факториал нуля
:zero
PUSH 1
OUT
HLT