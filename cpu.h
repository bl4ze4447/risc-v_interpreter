//
// Created by Antonie Gabriel Belu on 12.12.2025.
//

#ifndef CPU_H
#define CPU_H
#include <any>
#include <cstdint>
#include <string>
#include <array>

constexpr size_t ZERO = 0;
constexpr size_t RA = 1;
constexpr size_t SP = 2;
constexpr size_t GP = 3;
constexpr size_t TP = 4;
constexpr size_t T0 = 5;
constexpr size_t T1 = 6;
constexpr size_t T2 = 7;
constexpr size_t S0 = 8;
constexpr size_t FP = 8;
constexpr size_t S1 = 9;
constexpr size_t A0 = 10;
constexpr size_t A1 = 11;
constexpr size_t A2 = 12;
constexpr size_t A3 = 13;
constexpr size_t A4 = 14;
constexpr size_t A5 = 15;
constexpr size_t A6 = 16;
constexpr size_t A7 = 17;
constexpr size_t S2 = 18;
constexpr size_t S3 = 19;
constexpr size_t S4 = 20;
constexpr size_t S5 = 21;
constexpr size_t S6 = 22;
constexpr size_t S7 = 23;
constexpr size_t S8 = 24;
constexpr size_t S9 = 25;
constexpr size_t S10 = 26;
constexpr size_t S11 = 27;
constexpr size_t T3 = 28;
constexpr size_t T4 = 29;
constexpr size_t T5 = 30;
constexpr size_t T6 = 31;

constexpr size_t INSTRUCTIONS_COUNT = 67;
struct reg {
    int32_t value;
    char name[6];
};

class cpu {
    const std::array<char, 2> _valid_com_chars = {
        '#',
        ';'
    };
    const std::array<std::string, 67> _instructions = {
        "ret",
        "nop",
        "ecall",
        "ebreak",
        "j",
        "call",
        "tail",
        "lb",
        "lh",
        "lw",
        "lbu",
        "lhu",
        "sb",
        "sh",
        "sw",
        "li",
        "lui",
        "auipc",
        "mv",
        "sext.w",
        "neg",
        "negw",
        "seqz",
        "snez",
        "not",
        "jal",
        "jr",
        "sltz",
        "sgtz",
        "addi",
        "add",
        "subi",
        "sub",
        "xori",
        "xor",
        "ori",
        "or",
        "andi",
        "and",
        "slli",
        "sll",
        "sra",
        "srli",
        "srl",
        "slti",
        "slt",
        "sltiu",
        "sltu",
        "beq",
        "bne",
        "blt",
        "bge",
        "bltu",
        "bgeu",
        "jalr",
        "mul",
        "mulh",
        "mulsu",
        "mulu",
        "div",
        "divu",
        "rem",
        "remu",
        "bgt",
        "ble",
        "bgtu",
        "bleu"
    };

    [[nodiscard]] static size_t     get_register_index(const std::string& reg_name);
    [[nodiscard]] static int16_t    get_imm12(const std::string& s);
    [[nodiscard]] static int32_t    get_imm20(const std::string& s);
    [[nodiscard]] static uint32_t   get_bits_from_range(uint32_t num, size_t start = 0, size_t end = 31);
    [[nodiscard]] int32_t           get_register_value(size_t idx) const;
    [[nodiscard]] uint32_t          get_register_value_unsigned(size_t idx) const;
    void                            write_register(size_t idx, int32_t value);
    static void                     prepare_instruction(std::string& inst);
    [[nodiscard]] bool              is_comment(const std::string& s) const;

    [[nodiscard]] static constexpr unsigned int hash(const char *s);
    /* INSTRUCTIONS */
    void                instr_ret(bool args_ok);
    void                instr_nop(bool args_ok);
    void                instr_ecall(bool args_ok);
    void                instr_ebreak(bool args_ok);

    void                instr_j(bool args_ok, const std::string& arg1);
    void                instr_call(bool args_ok, const std::string& arg1);
    void                instr_tail(bool args_ok, const std::string& arg1);

    void                instr_lb(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_lh(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_lw(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_lbu(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_lhu(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_sb(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_sh(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_sw(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_li(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_lui(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_auipc(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_mv(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_sextw(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_neg(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_negw(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_seqz(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_snez(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_not(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_jal(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_jr(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_sltz(bool args_ok, const std::string& arg1, const std::string& arg2);
    void                instr_sgtz(bool args_ok, const std::string& arg1, const std::string& arg2);

    void                instr_add(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_sub(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_xor(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_or(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_and(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_sll(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    //https://stackoverflow.com/questions/56879794/whats-the-difference-between-srl-and-sra
    void                instr_srl(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_sltu(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_slt(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_sra(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_beq(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_bne(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_blt(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_bge(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_bltu(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_bgeu(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_jalr(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_mul(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_mulh(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_mulsu(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_mulu(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_div(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_divu(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_rem(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_remu(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_bgt(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_ble(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_bgtu(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
    void                instr_bleu(bool args_ok, const std::string& arg1, const std::string& arg2, const std::string& arg3);
public:
    cpu();
    static uint32_t     stoui_offset(const std::string& s, size_t offset);
    void                print_registers(bool hex = true) const;
    void                execute_instruction(std::string& inst);


    // data
    std::array<reg, 32> registers;
};

#endif //CPU_H
