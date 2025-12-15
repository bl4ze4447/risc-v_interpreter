//
// Created by Antonie Gabriel Belu on 12.12.2025.
//

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "cpu.h"

cpu::cpu() : registers{{{0, "zero"}, {0, "ra"}, {0, "sp"}, {0, "gp"}, {0, "tp"}, {0, "t0"}, {0, "t1"}, {0, "t2"}, {0, "s0/fp"}, {0, "s1"}, {0, "a0"}, {0, "a1"}, {0, "a2"}, {0, "a3"}, {0, "a4"}, {0, "a5"}, {0, "a6"}, {0, "a7"}, {0, "s2"}, {0, "s3"}, {0, "s4"}, {0, "s5"}, {0, "s6"}, {0, "s7"}, {0, "s8"}, {0, "s9"}, {0, "s10"}, {0, "s11"}, {0, "t3"}, {0, "t4"}, {0, "t5"}, {0, "t6"}}} {}
uint32_t cpu::stoui_offset(const std::string &s, size_t offset) {
    if (s.empty()) throw std::invalid_argument("Empty string");

    if (offset >= s.size()) throw std::invalid_argument("Offset out of range");

    const bool zero_first = s[offset] == '0';
    const size_t len = s.size();
    if (len - offset == 1 && zero_first) return 0;

    if (zero_first) throw std::invalid_argument("Invalid number format");

    unsigned int num = 0;
    for (; offset < len; ++offset) {
        const char c = s[offset];
        if (c < '0' || c > '9') throw std::invalid_argument("Invalid character");

        num = num * 10 + (c - '0');
    }

    return num;
}
size_t cpu::get_register_index(const std::string &reg_name) {
    if (reg_name.empty()) throw std::invalid_argument("Empty register");

    if (reg_name.size() == 1 || reg_name.size() > 4) throw std::invalid_argument("Invalid register name: " + reg_name);

    switch (reg_name[0]) {
        case 'x': {
            const auto i = stoui_offset(reg_name, 1);
            if (i > 31) throw std::invalid_argument("Bad register index: " + reg_name);

            return i;
        }

        case 'z': {
            if (reg_name != "zero") throw std::invalid_argument("Invalid register: " + reg_name);

            return ZERO;
        }

        case 'r': {
            if (reg_name != "ra") throw std::invalid_argument("Invalid register: " + reg_name);

            return RA;
        }

        case 'g': {
            if (reg_name != "gp") throw std::invalid_argument("Invalid register: " + reg_name);

            return GP;
        }

        case 't': {
            if (reg_name == "tp") return TP;

            const auto i = stoui_offset(reg_name, 1);
            if (i > 6) throw std::invalid_argument("Bad register index: " + reg_name);

            if (i <= 2) return T0 + i;

            return T3 + (i - 3);
        }

        case 's': {
            if (reg_name == "sp") return SP;

            const auto i = stoui_offset(reg_name, 1);
            if (i > 11) throw std::invalid_argument("Bad register index: " + reg_name);

            if (i <= 1) return S0 + i;

            return S2 + (i - 2);
        }

        case 'a': {
            const auto i = stoui_offset(reg_name, 1);
            if (i > 7) throw std::invalid_argument("Bad register index: " + reg_name);

            return A0 + i;
        }

        case 'f': {
            if (reg_name != "fp") throw std::invalid_argument("Invalid register: " + reg_name);

            return FP;
        }

        default:
            throw std::invalid_argument("Register not implemented yet, did you discover a new one?: " + reg_name);
    }
}
int16_t cpu::get_imm12(const std::string &s) {
    if (s.empty()) throw std::invalid_argument("Empty imm12");

    int imm12 = 0;
    try {
        imm12 = std::stoi(s);
    } catch (const std::exception &) {
        throw std::invalid_argument("Invalid imm12: " + s);
    }

    if (imm12 < -2048 || imm12 > 2047) throw std::invalid_argument("Invalid imm12 range: " + s);

    return static_cast<int16_t>(imm12);
}
void cpu::print_registers(const bool hex) const {
    std::cout << "\n------------- Registers -------------\n";
    size_t i = 0;
    for (const auto &r : registers) {
        std::cout << std::left << std::setw(6) << std::setfill(' ') << std::dec << r.name  << "| x" << i << std::left << std::setw(3) << std::setfill(' ') << std::dec<< " = " << std::right << (hex ? std::setw(8) : std::setw(0)) << std::setfill('0') << (hex ? std::hex : std::dec) << r.value << "\n";
        ++i;
    }

    std::cout << std::endl;
}
void cpu::write_register(const size_t idx, const int32_t value) {
    if (idx == 0) return;

    if (idx > 31) throw std::invalid_argument("Invalid register index: " + std::to_string(idx));

    registers[idx].value = value;
}
int32_t cpu::get_register_value(const size_t idx) const {
    if (idx == 0) return registers[idx].value;

    if (idx > 31) throw std::invalid_argument("Invalid register index: " + std::to_string(idx));

    return registers[idx].value;
}
uint32_t cpu::get_register_value_unsigned(const size_t idx) const {
    if (idx == 0) return static_cast<uint32_t>(registers[idx].value);

    if (idx > 31) throw std::invalid_argument("Invalid register index: " + std::to_string(idx));

    return static_cast<uint32_t>(registers[idx].value);
}
void cpu::prepare_instruction(std::string &inst) {
    for (char &c : inst)
        if (c == ',') c = ' ';

    size_t i = 0;
    const size_t len = inst.size();
    while (i < len && inst[i] == ' ') ++i;
    inst.erase(0, i);

    if (inst.front() == '#')
        inst.clear();
}

bool cpu::is_comment(const std::string &s) {
    if (s.empty())
        return false;

    for (const auto &c : _valid_com_chars)
        if (s.front() == c) return true;

    return false;
}

void cpu::instr_ret(bool args_ok) {
}

void cpu::instr_nop(const bool args_ok) {
    if (!args_ok)
        throw std::invalid_argument("Invalid argument");
}

void cpu::instr_ecall(bool args_ok) {
}

void cpu::instr_ebreak(bool args_ok) {
}

void cpu::instr_j(bool args_ok, const std::string &arg1) {
}

void cpu::instr_call(bool args_ok, const std::string &arg1) {
}

void cpu::instr_tail(bool args_ok, const std::string &arg1) {
}

void cpu::instr_lb(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_lh(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_lw(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_lbu(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_lhu(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_sb(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_sh(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_sw(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_li(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_lui(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_auipc(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_mv(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_sextw(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_neg(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_negw(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_seqz(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_snez(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_not(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_jal(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_jr(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_sltz(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_sqtz(bool args_ok, const std::string &arg1, const std::string &arg2) {
}

void cpu::instr_sub(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    if (arg3.front() == '-' || std::isdigit(arg3.front())) {
        const int16_t imm12 = get_imm12(arg3);
        write_register(rd, get_register_value(rs1) - imm12);
        return;
    }

    const size_t rs2 = get_register_index(arg3);
    write_register(rd, get_register_value(rs1) - get_register_value(rs2));
}

void cpu::instr_xor(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    if (arg3.front() == '-' || std::isdigit(arg3.front())) {
        const int16_t imm12 = get_imm12(arg3);
        write_register(rd, get_register_value(rs1) ^ imm12);
        return;
    }

    const size_t rs2 = get_register_index(arg3);
    write_register(rd, get_register_value(rs1) ^ get_register_value(rs2));
}

void cpu::instr_or(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    if (arg3.front() == '-' || std::isdigit(arg3.front())) {
        const int16_t imm12 = get_imm12(arg3);
        write_register(rd, get_register_value(rs1) | imm12);
        return;
    }

    const size_t rs2 = get_register_index(arg3);
    write_register(rd, get_register_value(rs1) | get_register_value(rs2));
}

void cpu::instr_and(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    if (arg3.front() == '-' || std::isdigit(arg3.front())) {
        const int16_t imm12 = get_imm12(arg3);
        write_register(rd, get_register_value(rs1) & imm12);
        return;
    }

    const size_t rs2 = get_register_index(arg3);
    write_register(rd, get_register_value(rs1) & get_register_value(rs2));
}

void cpu::instr_sll(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    if (arg3.front() == '-' || std::isdigit(arg3.front())) {
        const int16_t imm12 = get_imm12(arg3);
        write_register(rd, static_cast<int32_t>(get_register_value_unsigned(rs1) << imm12));
        return;
    }

    const size_t rs2 = get_register_index(arg3);
    write_register(rd, static_cast<int32_t>(get_register_value_unsigned(rs1) << get_register_value_unsigned(rs2)));
}

void cpu::instr_srl(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    if (arg3.front() == '-' || std::isdigit(arg3.front())) {
        const int16_t imm12 = get_imm12(arg3);
        write_register(rd, static_cast<int32_t>(get_register_value_unsigned(rs1) >> imm12));
        return;
    }

    const size_t rs2 = get_register_index(arg3);
    write_register(rd, static_cast<int32_t>(get_register_value_unsigned(rs1) >> get_register_value_unsigned(rs2)));
}

void cpu::instr_sltu(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    if (arg3.front() == '-' || std::isdigit(arg3.front())) {
        const int16_t imm12 = get_imm12(arg3);
        write_register(rd, get_register_value_unsigned(rs1) < static_cast<uint16_t>(imm12));
        return;
    }

    const size_t rs2 = get_register_index(arg3);
    write_register(rd, get_register_value_unsigned(rs1) < get_register_value_unsigned(rs2));
}

void cpu::instr_slt(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    if (arg3.front() == '-' || std::isdigit(arg3.front())) {
        const int16_t imm12 = get_imm12(arg3);
        write_register(rd, get_register_value(rs1) < imm12);
        return;
    }

    const size_t rs2 = get_register_index(arg3);
    write_register(rd, get_register_value(rs1) < get_register_value(rs2));
}

void cpu::instr_sra(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    if (arg3.front() == '-' || std::isdigit(arg3.front())) {
        const int16_t imm12 = get_imm12(arg3);
        write_register(rd, get_register_value(rs1) >> imm12);
        return;
    }

    const size_t rs2 = get_register_index(arg3);
    write_register(rd, get_register_value(rs1) >> get_register_value(rs2));
}

void cpu::instr_beq(bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
}

void cpu::instr_bne(bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
}

void cpu::instr_blt(bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
}

void cpu::instr_bge(bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
}

void cpu::instr_bltu(bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
}

void cpu::instr_bgeu(bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
}

void cpu::instr_jalr(bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
}

void cpu::instr_mul(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    const size_t rs2 = get_register_index(arg3);
    write_register(rd, get_register_value(rs1) * get_register_value(rs2));
}

void cpu::instr_mulh(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    const size_t rs2 = get_register_index(arg3);
    const int64_t result = static_cast<int64_t>(get_register_value(rs1)) * static_cast<int64_t>(get_register_value(rs2));
    write_register(rd, static_cast<int32_t>(result >> 32));
}

void cpu::instr_mulsu(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    const size_t rs2 = get_register_index(arg3);
    const int64_t result = static_cast<int64_t>(static_cast<int64_t>(get_register_value(rs1)) * static_cast<uint64_t>(get_register_value(rs2)));
    write_register(rd, static_cast<int32_t>(result >> 32));
}

void cpu::instr_mulu(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    const size_t rs2 = get_register_index(arg3);
    write_register(rd, static_cast<int32_t>(get_register_value_unsigned(rs1) * get_register_value_unsigned(rs2)));
}

void cpu::instr_div(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    const size_t rs2 = get_register_index(arg3);
    const int32_t rs2_value = get_register_value(rs2);
    if (rs2_value == 0) {
        write_register(rd, -1);
        return;
    }

    write_register(rd, get_register_value(rs1) / rs2_value);
}

void cpu::instr_divu(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    const size_t rs2 = get_register_index(arg3);
    const uint32_t rs2_value = get_register_value_unsigned(rs2);
    if (rs2_value == 0) {
        write_register(rd, static_cast<int32_t>(-1u));
        return;
    }

    write_register(rd, static_cast<int32_t>(get_register_value_unsigned(rs1) / rs2_value));
}

void cpu::instr_rem(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    const size_t rs2 = get_register_index(arg3);
    const int32_t rs2_value = get_register_value(rs2);
    if (rs2_value == 0) {
        write_register(rd, get_register_value(rs1));
        return;
    }

    write_register(rd, get_register_value(rs1) % rs2_value);
}

void cpu::instr_remu(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    const size_t rs2 = get_register_index(arg3);
    const uint32_t rs2_value = get_register_value_unsigned(rs2);
    if (rs2_value == 0) {
        write_register(rd, static_cast<int32_t>(get_register_value_unsigned(rs1)));
        return;
    }

    write_register(rd, static_cast<int32_t>(get_register_value_unsigned(rs1) % rs2_value));
}

void cpu::instr_bgt(bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
}

void cpu::instr_ble(bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
}

void cpu::instr_bgtu(bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
}

void cpu::instr_bleu(bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
}

void cpu::instr_add(const bool args_ok, const std::string &arg1, const std::string &arg2, const std::string &arg3) {
    if (!args_ok)
        throw std::invalid_argument("Invalid number of args!");

    const size_t rd = get_register_index(arg1);
    const size_t rs1 = get_register_index(arg2);
    if (arg3.front() == '-' || std::isdigit(arg3.front())) {
        const int16_t imm12 = get_imm12(arg3);
        write_register(rd, get_register_value(rs1) + imm12);
        return;
    }

    const size_t rs2 = get_register_index(arg3);
    write_register(rd, get_register_value(rs1) + get_register_value(rs2));
}


constexpr unsigned int cpu::hash(const char *s) {
    unsigned int h = 5381;
    while (*s) {
        h = (h * 33) ^ *s;
        s++;
    }
    return h;
}

void cpu::execute_instruction(std::string &inst) {
    prepare_instruction(inst);
    std::istringstream iss(inst);
    std::string op, arg1, arg2, arg3;
    iss >> op >> arg1 >> arg2 >> arg3;

    if (op.empty()) return;
    if (is_comment(op)) return;
    if (is_comment(arg1)) arg1.clear();
    if (is_comment(arg2)) arg2.clear();
    if (is_comment(arg3)) arg3.clear();

    size_t args = !arg1.empty() + !arg2.empty() + !arg3.empty();
    switch (hash(op.c_str())) {
        /* 0 args */
        case hash("ret"):    instr_ret(args == 0); break;
        case hash("nop"):    instr_nop(args == 0); break;
        case hash("ecall"):  instr_ecall(args == 0); break;
        case hash("ebreak"): instr_ebreak(args == 0); break;

        /* 1 args */
        case hash("j"):      instr_j(args == 1, arg1); break;
        case hash("call"):   instr_call(args == 1, arg1); break;
        case hash("tail"):   instr_tail(args == 1, arg1); break;

        /* 2 args */
        case hash("lb"):     instr_lb(args == 2, arg1, arg2); break;
        case hash("lh"):     instr_lh(args == 2, arg1, arg2); break;
        case hash("lw"):     instr_lw(args == 2, arg1, arg2); break;
        case hash("lbu"):    instr_lbu(args == 2, arg1, arg2); break;
        case hash("lhu"):    instr_lhu(args == 2, arg1, arg2); break;
        case hash("sb"):     instr_sb(args == 2, arg1, arg2); break;
        case hash("sh"):     instr_sh(args == 2, arg1, arg2); break;
        case hash("sw"):     instr_sw(args == 2, arg1, arg2); break;
        case hash("li"):     instr_li(args == 2, arg1, arg2); break;
        case hash("lui"):    instr_lui(args == 2, arg1, arg2); break;
        case hash("auipc"):  instr_auipc(args == 2, arg1, arg2); break;
        case hash("mv"):     instr_mv(args == 2, arg1, arg2); break;
        case hash("sext.w"): instr_sextw(args == 2, arg1, arg2); break;
        case hash("neg"):    instr_neg(args == 2, arg1, arg2); break;
        case hash("negw"):   instr_negw(args == 2, arg1, arg2); break;
        case hash("seqz"):   instr_seqz(args == 2, arg1, arg2); break;
        case hash("snez"):   instr_snez(args == 2, arg1, arg2); break;
        case hash("not"):    instr_not(args == 2, arg1, arg2); break;
        case hash("jal"):    instr_jal(args == 2, arg1, arg2); break;
        case hash("jr"):     instr_jr(args == 2, arg1, arg2); break;
        case hash("sltz"):   instr_sltz(args == 2, arg1, arg2); break;
        case hash("sgtz"):   instr_sqtz(args == 2, arg1, arg2); break;

        /* 3 args */
        case hash("add"):
        case hash("addi"):
            instr_add(args == 3, arg1, arg2, arg3);
            break;

        case hash("sub"):
        case hash("subi"):
            instr_sub(args == 3, arg1, arg2, arg3);
            break;

        case hash("xor"):
        case hash("xori"):
            instr_xor(args == 3, arg1, arg2, arg3);
            break;

        case hash("or"):
        case hash("ori"):
            instr_or(args == 3, arg1, arg2, arg3);
            break;

        case hash("and"):
        case hash("andi"):
            instr_and(args == 3, arg1, arg2, arg3);
            break;

        case hash("sll"):
        case hash("slli"):
            instr_sll(args == 3, arg1, arg2, arg3);
            break;

        case hash("srli"):
        case hash("srl"):
            instr_srl(args == 3, arg1, arg2, arg3);
            break;

        case hash("srai"):
        case hash("sra"):
            instr_sra(args == 3, arg1, arg2, arg3);
            break;

        case hash("beq"):    instr_beq(args == 3, arg1, arg2, arg3); break;
        case hash("bne"):    instr_bne(args == 3, arg1, arg2, arg3); break;
        case hash("blt"):    instr_blt(args == 3, arg1, arg2, arg3); break;
        case hash("bge"):    instr_bge(args == 3, arg1, arg2, arg3); break;
        case hash("bltu"):   instr_bltu(args == 3, arg1, arg2, arg3); break;
        case hash("bgeu"):   instr_bgeu(args == 3, arg1, arg2, arg3); break;
        case hash("jalr"):   instr_jalr(args == 3, arg1, arg2, arg3); break;
        case hash("mul"):    instr_mul(args == 3, arg1, arg2, arg3); break;
        case hash("mulh"):   instr_mulh(args == 3, arg1, arg2, arg3); break;
        case hash("mulsu"):  instr_mulsu(args == 3, arg1, arg2, arg3); break;
        case hash("mulu"):   instr_mulu(args == 3, arg1, arg2, arg3); break;
        case hash("div"):    instr_div(args == 3, arg1, arg2, arg3); break;
        case hash("divu"):   instr_divu(args == 3, arg1, arg2, arg3); break;
        case hash("rem"):    instr_rem(args == 3, arg1, arg2, arg3); break;
        case hash("remu"):   instr_remu(args == 3, arg1, arg2, arg3); break;
        case hash("bgt"):    instr_bgt(args == 3, arg1, arg2, arg3); break;
        case hash("ble"):    instr_ble(args == 3, arg1, arg2, arg3); break;
        case hash("bgtu"):   instr_bgtu(args == 3, arg1, arg2, arg3); break;
        case hash("bleu"):   instr_bleu(args == 3, arg1, arg2, arg3); break;

        default:
            throw std::invalid_argument("Operation not implemented: " + op);
    }

    print_registers(false);
}