# --- 1. INITIALIZARE & PSEUDO-INSTRUCTIUNI FUNCȚIONALE ---
nop                 # No Operation
li x1, 20           # x1 = 20 (Load Immediate -> addi x1, x0, 20)
li x2, 10           # x2 = 10
li x3, -5           # x3 = -5
mv x4, x1           # x4 = 20 (Move -> add x4, x1, x0)
lui x5, 1           # x5 = 4096 (1 << 12) - ACUM FUNCTIONEAZA!

# --- 2. ARITMETICĂ DE BAZĂ (ADD, SUB, NEG) ---
add x6, x1, x2      # x6 = 20 + 10 = 30
addi x7, x1, 5      # x7 = 20 + 5 = 25
sub x8, x1, x2      # x8 = 20 - 10 = 10
sub x9, x1, 5       # x9 = 20 - 5 = 15 (Folosind logica ta specială din instr_sub)
neg x10, x2         # x10 = -10 (Pseudo -> sub x10, x0, x2)

# --- 3. LOGICĂ (AND, OR, XOR, NOT) ---
and x11, x1, x2     # x11 = 20 & 10 (10100 & 01010) = 0
andi x12, x1, 20    # x12 = 20 & 20 = 20
or x13, x1, x2      # x13 = 20 | 10 (10100 | 01010) = 30 (11110)
ori x14, x0, 7      # x14 = 7
xor x15, x1, x2     # x15 = 20 ^ 10 = 30
xori x16, x14, 7    # x16 = 7 ^ 7 = 0
not x17, x0         # x17 = -1 (Pseudo -> xor x17, x0, -1)

# --- 4. SHIFTĂRI (SLL, SRL, SRA) - TESTAREA FIX-ULUI ---
# x1 = 20 (Binary: ...00010100)
sll x18, x1, 2      # x18 = 20 << 2 = 80
slli x19, x1, 2     # x19 = 20 << 2 = 80
srl x20, x1, 2      # x20 = 20 >> 2 = 5
srli x21, x1, 2     # x21 = 20 >> 2 = 5
li x22, -20         # x22 = -20
sra x23, x22, 2     # x23 = -20 >> 2 (Arithmetic) = -5
srai x24, x22, 2    # x24 = -5

# --- 5. COMPARAȚII (SLT, SLTU, SEQZ, SNEZ, ETC) ---
slt x25, x3, x2     # x25 = 1 (pentru că -5 < 10)
sltu x26, x3, x2    # x26 = 0 (pentru că -5 unsigned e uriaș > 10)
seqz x27, x16       # x27 = 1 (Set Equal Zero: x16 e 0, deci true)
snez x28, x1        # x28 = 1 (Set Not Equal Zero: x1 e 20, deci true)
sltz x29, x3        # x29 = 1 (Set Less Than Zero: -5 < 0)
sgtz x30, x1        # x30 = 1 (Set Greater Than Zero: 20 > 0)

# --- 6. M-EXTENSION (MUL, DIV, REM) ---
mul x1, x2, x3      # x1 = 10 * -5 = -50 (Refolosim x1)
mulh x2, x2, x3     # x2 = High bits (-1 cel mai probabil)
mov x3, x5, x5     # x3 = 4096 * 4096 (Unsigned)
div x4, x1, x2      # x4 = -50 / -1 = 50
divu x5, x1, x2     # x5 = Unsigned division
rem x6, x1, x2      # x6 = Remainder
remu x7, x1, x2     # x7 = Unsigned remainder