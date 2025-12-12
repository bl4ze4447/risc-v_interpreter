//
// Created by Antonie Gabriel Belu on 12.12.2025.
//

#include "cpu.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>

cpu::cpu() : registers{{
    {0, "zero"}, {0, "ra"}, {0, "sp"}, {0, "gp"},
    {0, "tp"},   {0, "t0"}, {0, "t1"}, {0, "t2"},
    {0, "s0/fp"},{0, "s1"}, {0, "a0"}, {0, "a1"},
    {0, "a2"},   {0, "a3"}, {0, "a4"}, {0, "a5"},
    {0, "a6"},   {0, "a7"}, {0, "s2"}, {0, "s3"},
    {0, "s4"},   {0, "s5"}, {0, "s6"}, {0, "s7"},
    {0, "s8"},   {0, "s9"}, {0, "s10"},{0, "s11"},
    {0, "t3"},   {0, "t4"}, {0, "t5"}, {0, "t6"}
}} {}
uint32_t cpu::stoui_offset(const std::string& s, size_t offset) {
    if (s.empty())
        throw std::invalid_argument("Empty string");

    if (offset >= s.size())
        throw std::invalid_argument("Offset out of range");

    const bool zero_first = s[offset] == '0';
    const size_t len = s.size();
    if (len-offset == 1 && zero_first)
        return 0;

    if (zero_first)
        throw std::invalid_argument("Invalid number format");

    unsigned int num = 0;
    for (; offset < len; ++offset) {
        const char c = s[offset];
        if (c < '0' || c > '9')
            throw std::invalid_argument("Invalid character");

        num = num * 10 + (c - '0');
    }

    return num;
}
size_t cpu::get_register_index(const std::string &reg_name) {
    if (reg_name.empty())
        throw std::invalid_argument("Empty register");

    if (reg_name.size() == 1 || reg_name.size() > 4)
        throw std::invalid_argument("Invalid register name: " + reg_name);

    switch (reg_name[0]) {
        case 'x': {
            const auto i = stoui_offset(reg_name, 1);
            if (i > 31)
                throw std::invalid_argument("Bad register index: " + reg_name);

            return i;
        }

        case 'z': {
            if (reg_name != "zero")
                throw std::invalid_argument("Invalid register: " + reg_name);

            return ZERO;
        }

        case 'r': {
            if (reg_name != "ra")
                throw std::invalid_argument("Invalid register: " + reg_name);

            return RA;
        }

        case 'g': {
            if (reg_name != "gp")
                throw std::invalid_argument("Invalid register: " + reg_name);

            return GP;
        }

        case 't': {
            if (reg_name == "tp")
                return TP;

            const auto i = stoui_offset(reg_name, 1);
            if (i > 6)
                throw std::invalid_argument("Bad register index: " + reg_name);

            if (i <= 2)
                return T0+i;

            return T3+(i-3);
        }

        case 's': {
            if (reg_name == "sp")
                return SP;

            const auto i = stoui_offset(reg_name, 1);
            if (i > 11)
                throw std::invalid_argument("Bad register index: " + reg_name);

            if (i <= 1)
                return S0+i;

            return S2+(i-2);
        }

        case 'a': {
            const auto i = stoui_offset(reg_name, 1);
            if (i > 7)
                throw std::invalid_argument("Bad register index: " + reg_name);

            return A0+i;
        }

        case 'f': {
            if (reg_name != "fp")
                throw std::invalid_argument("Invalid register: " + reg_name);

            return FP;
        }

        default:
            throw std::invalid_argument("Register not implemented yet, did you discover a new one?: " + reg_name);
    }
}
int16_t cpu::get_imm12(const std::string& s) {
    if (s.empty())
        throw std::invalid_argument("Empty imm12");

    int imm12 = 0;
    try {
        imm12 = std::stoi(s);
    } catch (const std::exception&) {
        throw std::invalid_argument("Invalid imm12: " + s);
    }

    if (imm12 < -2048 || imm12 > 2047)
        throw std::invalid_argument("Invalid imm12 range: " + s);

    return static_cast<int16_t>(imm12);
}
void cpu::print_registers(const bool hex) const {
    std::cout << "\n> RISC-V Registers:\n";
    for (const auto& r : registers) {
        std::cout << std::left << std::setw(6) << std::setfill(' ') << std::dec << r.name
                  << ": "
                  << std::right << (hex ? std::setw(8) : std::setw(0)) << std::setfill('0') << (hex ? std::hex : std::dec) << r.value
                  << "\n";
    }

    std::cout << std::endl;
}
void cpu::write_register(const size_t idx, const int32_t value) {
    if (idx == 0)
        return;

    if (idx > 31)
        throw std::invalid_argument("Invalid register index: " + std::to_string(idx));

    registers[idx].value = value;
}
int32_t cpu::get_register_value(const size_t idx) const {
    if (idx == 0)
        return registers[idx].value;

    if (idx > 31)
        throw std::invalid_argument("Invalid register index: " + std::to_string(idx));

    return registers[idx].value;
}
uint32_t cpu::get_register_value_unsigned(const size_t idx) const {
    if (idx == 0)
        return static_cast<uint32_t>(registers[idx].value);

    if (idx > 31)
        throw std::invalid_argument("Invalid register index: " + std::to_string(idx));

    return static_cast<uint32_t>(registers[idx].value);
}
void cpu::prepare_instruction(std::string &inst) {
    for (char &c : inst)
        if (c == ',')
            c = ' ';

    size_t i = 0;
    const size_t len = inst.size();
    while (i < len && inst[i] == ' ') ++i;
    inst.erase(0, i);
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

        // empty line
        if (op.empty())
            return;

        size_t args = !arg1.empty() + !arg2.empty() + !arg3.empty();
        bool args_ok;
        switch (hash(op.c_str())) {
            /* 0 args */
            case hash("ret"): {
                args_ok = args == 0;
                break;
            }

            case hash("nop"): {
                args_ok = args == 0;
                break;
            }

            case hash("ecall"): {
                args_ok = args == 0;
                break;
            }

            case hash("ebreak"): {
                args_ok = args == 0;
                break;
            }

            /* 1 args */
            case hash("j"): {
                args_ok = args == 1;
                break;
            }

            case hash("call"): {
                args_ok = args == 1;
                break;
            }

            case hash("tail"): {
                args_ok = args == 1;
                break;
            }


            /* 2 args */
            case hash("lb"): {
                args_ok = args == 2;
                break;
            }
            case hash("lh"): {
                args_ok = args == 2;
                break;
            }
            case hash("lw"): {
                args_ok = args == 2;
                break;
            }
            case hash("lbu"): {
                args_ok = args == 2;
                break;
            }
            case hash("lhu"): {
                args_ok = args == 2;
                break;
            }
            case hash("sb"): {
                args_ok = args == 2;
                break;
            }
            case hash("sh"): {
                args_ok = args == 2;
                break;
            }
            case hash("sw"): {
                args_ok = args == 2;
                break;
            }
            case hash("li"): {
                args_ok = args == 2;
                break;
            }
            case hash("lui"): {
                args_ok = args == 2;
                break;
            }
            case hash("auipc"): {
                args_ok = args == 2;
                break;
            }
            case hash("mv"): {
                args_ok = args == 2;
                break;
            }
            case hash("sext.w"): {
                args_ok = args == 2;
                break;
            }
            case hash("neg"): {
                args_ok = args == 2;
                break;
            }
            case hash("negw"): {
                args_ok = args == 2;
                break;
            }
            case hash("seqz"): {
                args_ok = args == 2;
                break;
            }
            case hash("snez"): {
                args_ok = args == 2;
                break;
            }
            case hash("not"): {
                args_ok = args == 2;
                break;
            }
            case hash("jal"): {
                args_ok = args == 2;
                break;
            }
            case hash("jr"): {
                args_ok = args == 2;
                break;
            }
            case hash("sltz"): {
                args_ok = args == 2;
                break;
            }
            case hash("sgtz"): {
                args_ok = args == 2;
                break;
            }

            /* 3 args */
            case hash("addi"):
            case hash("add"): {
                args_ok = args == 3;

                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                if (op[op.size() - 1] == 'i') {
                    const auto imm12 = get_imm12(arg3);
                    write_register(rd, get_register_value(rs1) + imm12);
                    break;
                }

                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value(rs1) + get_register_value(rs2));

                break;
            }

            case hash("subi"):
            case hash("sub"): {
                args_ok = args == 3;
                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                if (op[op.size() - 1] == 'i') {
                    const auto imm12 = get_imm12(arg3);
                    write_register(rd, get_register_value(rs1) - imm12);
                    break;
                }

                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value(rs1) - get_register_value(rs2));
                break;
            }

            case hash("xori"):
            case hash("xor"): {
                args_ok = args == 3;
                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                if (op[op.size() - 1] == 'i') {
                    const auto imm12 = get_imm12(arg3);
                    write_register(rd, get_register_value(rs1) ^ imm12);
                    break;
                }

                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value(rs1) ^ get_register_value(rs2));
                break;
            }

            case hash("ori"):
            case hash("or"): {
                args_ok = args == 3;
                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                if (op[op.size() - 1] == 'i') {
                    const auto imm12 = get_imm12(arg3);
                    write_register(rd, get_register_value(rs1) | imm12);
                    break;
                }

                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value(rs1) | get_register_value(rs2));
                break;
            }

            case hash("andi"):
            case hash("and"): {
                args_ok = args == 3;
                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                if (op[op.size() - 1] == 'i') {
                    const auto imm12 = get_imm12(arg3);
                    write_register(rd, get_register_value(rs1) & imm12);
                    break;
                }

                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value(rs1) & get_register_value(rs2));
                break;
            }

            case hash("slli"):
            case hash("sll"): {
                args_ok = args == 3;
                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                if (op[op.size() - 1] == 'i') {
                    const auto imm12 = get_imm12(arg3);
                    write_register(rd, get_register_value(rs1) << imm12);
                    break;
                }

                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value(rs1) << get_register_value(rs2));
                break;
            }

            case hash("sra"):
            case hash("srli"):
            case hash("srl"): {
                args_ok = args == 3;
                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                if (op[op.size() - 1] == 'i') {
                    const auto imm12 = get_imm12(arg3);
                    write_register(rd, get_register_value(rs1) >> imm12);
                    break;
                }

                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value(rs1) >> get_register_value(rs2));
                break;
            }

            case hash("slti"):
            case hash("slt"): {
                args_ok = args == 3;
                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                if (op[op.size() - 1] == 'i') {
                    const auto imm12 = get_imm12(arg3);
                    write_register(rd, get_register_value(rs1) < imm12);
                    break;
                }

                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value(rs1) < get_register_value(rs2));
                break;
            }

            case hash("sltiu"):
            case hash("sltu"): {
                args_ok = args == 3;
                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                if (op[op.size() - 1] == 'i') {
                    const auto imm12 = get_imm12(arg3);
                    write_register(rd, get_register_value_unsigned(rs1) < static_cast<uint32_t>(imm12));
                    break;
                }

                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value_unsigned(rs1) < get_register_value_unsigned(rs2));
                break;
            }
            case hash("beq"): {
                args_ok = args == 3;
                break;
            }
            case hash("bne"): {
                args_ok = args == 3;
                break;
            }
            case hash("blt"): {
                args_ok = args == 3;
                break;
            }
            case hash("bge"): {
                args_ok = args == 3;
                break;
            }
            case hash("bltu"): {
                args_ok = args == 3;
                break;
            }
            case hash("bgeu"): {
                args_ok = args == 3;
                break;
            }

            case hash("jalr"): {
                args_ok = args == 3;
                break;
            }

            case hash("mul"): {
                args_ok = args == 3;
                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value(rs1) * get_register_value(rs2));
                break;
            }

            case hash("mulh"): {
                args_ok = args == 3;
                break;
            }

            case hash("mulsu"): {
                args_ok = args == 3;
                break;
            }

            case hash("mulu"): {
                args_ok = args == 3;
                break;
            }

            case hash("div"): {
                args_ok = args == 3;
                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value(rs1) / get_register_value(rs2));
                break;
            }

            case hash("divu"): {
                args_ok = args == 3;
                break;
            }

            case hash("rem"): {
                args_ok = args == 3;
                size_t rd = get_register_index(arg1);
                size_t rs1 = get_register_index(arg2);
                size_t rs2 = get_register_index(arg3);
                write_register(rd, get_register_value(rs1) % get_register_value(rs2));
                break;
            }

            case hash("remu"): {
                args_ok = args == 3;
                break;
            }

            case hash("bgt"): {
                args_ok = args == 3;
                break;
            }

            case hash("ble"): {
                args_ok = args == 3;
                break;
            }

            case hash("bgtu"): {
                args_ok = args == 3;
                break;
            }

            case hash("bleu"): {
                args_ok = args == 3;
                break;
            }

            default:
                throw std::invalid_argument("Operation not implemented: " + op);
        }

        if (!args_ok)
            throw std::invalid_argument("Args issue: " + op);
    }