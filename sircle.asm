PUSH 0
POP ax

IN
POP bx

:if
PUSH ax
PUSH bx

JB :print

DISP
HLT

:print
PUSH 1
POP [ax]

PUSH 1
PUSH ax
ADD
POP ax

JMP :if