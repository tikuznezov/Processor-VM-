; Init circle parameters
PUSH 0
POP ax       ; x = 0

PUSH 7
POP bx       ; y = r = 7

; Calculate d = 3 - 2 * r
PUSH 3
PUSH 2
PUSH 7       ; r = 7
MUL          ; 2 * r
SUB          ; 3 - 2 * r
POP dx       ; dx = d

; draw symmetry pixels in 8 sectors
CALL :draw_8_pixels

; MAIN cycle 
:loop
PUSH bx
PUSH ax
JB :end_loop ; if (y < x) end loop (more than 45 degree)

; Step X: x++
PUSH ax
PUSH 1
ADD
POP ax

; check d (dx)
PUSH dx
PUSH 0
JA :d_greater_than_zero  ; if (d > 0) goto step Y

; if (d <= 0) d = d + 4 * x + 6
PUSH dx
PUSH 4
PUSH ax
MUL          ; 4 * x
ADD          ; d + 4 * x
PUSH 6
ADD          ; d + 4 * x + 6
POP dx       ; Update d
JMP :step_done

:d_greater_than_zero    ; if (d > 0) y--, d = d + 4 * (x - y) + 10
; y--
PUSH bx
PUSH 1
SUB
POP bx

; d = d + 4 * (x - y) + 10
PUSH dx
PUSH 4
PUSH ax
PUSH bx
SUB          ; (x - y)
MUL          ; 4 * (x - y)
ADD          ; d + 4 * (x - y)
PUSH 10
ADD          ; d + 4 * (x - y) + 10
POP dx       ; Update d

:step_done
CALL :draw_8_pixels
JMP :loop

:end_loop
DISP
HLT


;------------------------------------------------------------------------------------------------------

; disp 1
:print_dot
POP cx       ; cx = Y
POP fx       ; fx = X

; Calc mem address
PUSH cx
PUSH 60      ; WIDTH = 60
MUL          ; Y * WIDTH
PUSH fx
ADD          ; Y * WIDTH + X
POP cx       ; cx = mem address

PUSH 1
POP [cx] ; print dot to mem[cx]
RET

;--------------------------------------------------

; draw 8 pixel in circle sectors
; xc yc is centre coordinate
; ax is X
; bx is Y
:draw_8_pixels

; 1. (xc + ax, yc + bx)
PUSH xc
PUSH ax
ADD
PUSH yc
PUSH bx
ADD
CALL :print_dot

; 2. (xc - ax, yc + bx)
PUSH xc
PUSH ax
SUB
PUSH yc
PUSH bx
ADD
CALL :print_dot

; 3. (xc + ax, yc - bx)
PUSH xc
PUSH ax
ADD
PUSH yc
PUSH bx
SUB
CALL :print_dot

; 4. (xc - ax, yc - bx)
PUSH xc
PUSH ax
SUB
PUSH yc
PUSH bx
SUB
CALL :print_dot

; 5. (yc + bx, xc + ax)
PUSH yc
PUSH bx
ADD
PUSH xc
PUSH ax
ADD
CALL :print_dot

; 6. (yc - bx, xc + ax)
PUSH yc
PUSH bx
SUB
PUSH xc
PUSH ax
ADD
CALL :print_dot

; 7. (yc + bx, xc - ax)
PUSH yc
PUSH bx
ADD
PUSH xc
PUSH ax
SUB
CALL :print_dot

; 8. (yc - bx, xc - ax)
PUSH yc
PUSH bx
SUB
PUSH xc
PUSH ax
SUB
CALL :print_dot

RET