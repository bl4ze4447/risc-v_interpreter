; --- SETUP ---
; x5 = INT_MAX (Maximum pozitiv pe 32 biți: 2,147,483,647)
; 0x7FFFFFFF in hex
addi x5, x0, 2047      ; Construim numărul bucată cu bucată
slli x5, x5, 11        ; Shift stânga
addi x5, x5, 2047      ; Adăugăm
slli x5, x5, 10        ; Shift stânga
addi x5, x5, 1023      ; Acum x5 = 2147483647 (0x7FFFFFFF)

addi x6, x0, -2        ; x6 = -2 (Pentru înmulțire negativă)
add  x7, x0, x0        ; x7 = 0 (Pentru EDGE CASE: Diviziune la zero)

; --- EDGE CASE 1: Signed Overflow ---
; Adăugăm 1 la numărul maxim pozitiv.
; În mod normal, matematica spune 2,147,483,648.
; În procesoare, acesta devine cel mai mic număr negativ (INT_MIN).
addi x8, x5, 1         ; x8 = INT_MAX + 1 = INT_MIN (-2,147,483,648)

; --- EDGE CASE 2: High & Low Multiplication ---
; Înmulțim un număr imens (x5) cu unul negativ (x6).
; Rezultatul real ar fi -4,294,967,294. Asta depășește 32 biți.
; mul  = păstrează partea de jos (low 32 bits).
; mulh = păstrează partea de sus (high 32 bits).
mul  x9, x5, x6        ; x9 = Low bits. Matematica modulară: Rezultatul e 2.
mulh x10, x5, x6       ; x10 = High bits. Rezultatul e -1 (0xFFFFFFFF).

; --- EDGE CASE 3: Diviziunea la Zero (Comportament RISC-V) ---
; RISC-V NU dă crash la împărțirea la zero. Are reguli specifice.
; Regulă: x / 0 = -1 (toți biții pe 1).
; Regulă: x % 0 = x (restul e deîmpărțitul).
div  x11, x5, x7       ; x11 = INT_MAX / 0 = -1
rem  x12, x5, x7       ; x12 = INT_MAX % 0 = INT_MAX

; --- COMPLEX LOGIC & SHIFT ---
; Luăm x8 (INT_MIN = 1000...0000 binar) și îl procesăm.
srai x13, x8, 31       ; Arithmetic Shift: Păstrează semnul. 1 -> 111...1 = -1
srli x14, x8, 31       ; Logical Shift: Introduce zero. 1 -> 000...1 = 1

; Combinăm totul cu XOR și AND
xor  x15, x13, x14     ; x15 = -1 XOR 1.
                       ; 111...111 XOR 000...001 = 111...110 (-2)
and  x16, x11, x9      ; x16 = -1 AND 2.
                       ; 111...111 AND 000...010 = 2

; Ultimul test: Subtraction Immediate fără instrucțiune
addi x17, x16, -10     ; x17 = 2 + (-10) = -8