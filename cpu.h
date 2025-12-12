//
// Created by Antonie Gabriel Belu on 12.12.2025.
//

#ifndef CPU_H
#define CPU_H
#include <any>
#include <cstdint>
#include <string>
#include <array>
#include <functional>

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

struct reg {
    int32_t value;
    char name[6];
};

class cpu {
    static size_t       get_register_index(const std::string& reg_name);
    static int16_t      get_imm12(const std::string& s);
    void                write_register(size_t idx, int32_t value);
    int32_t             get_register_value(size_t idx) const;
    uint32_t            get_register_value_unsigned(size_t idx) const;
    static void         prepare_instruction(std::string& inst);
    static constexpr unsigned int hash(const char *s);
    void                instr_ret(bool args_ok);
    void                instr_nop(bool args_ok);
    void                instr_ecall(bool args_ok);
    void                instr_ebreak(bool args_ok);

    void                instr_j(bool args_ok, std::string& arg1, std::string = "", std::string = "");
    void                instr_call(bool args_ok, std::string& arg1, std::string = "", std::string = "");
    void                instr_tail(bool args_ok, std::string& arg1, std::string = "", std::string = "");

    void                instr_lb(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_lh(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_lw(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_lbu(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_lhu(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_sb(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_sh(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_sw(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_li(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_lui(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_auipc(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_mv(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_sextw(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_neg(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_negw(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_seqz(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_snez(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_not(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_jal(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_jr(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_sltz(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");
    void                instr_sqtz(bool args_ok, std::string& arg1, std::string& arg2, std::string = "");

    void                instr_add(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_sub(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_xor(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_or(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_and(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_sll(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    //https://stackoverflow.com/questions/56879794/whats-the-difference-between-srl-and-sra
    void                instr_srl(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_sltu(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_sra(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_beq(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_bne(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_blt(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_bge(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_bltu(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_bgeu(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_jalr(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_mul(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_mulh(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_mulsu(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_mulu(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_div(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_divu(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_rem(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_remu(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_bgt(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_ble(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_bgtu(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
    void                instr_bleu(bool args_ok, std::string& arg1, std::string& arg2, std::string& arg3);
public:
    cpu();
    static uint32_t     stoui_offset(const std::string& s, size_t offset);
    void                print_registers(bool hex = true) const;
    void                execute_instruction(std::string& inst);


    // data
    std::array<reg, 32> registers;
    std::array<std::function<void(std::any)>, 60> instruction_functions;
};



#endif //CPU_H
