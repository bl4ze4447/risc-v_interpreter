#include <iostream>
#include <fstream>
#include "cpu.h"

int main() {
    std::ifstream fin("risc-v.asm");
    cpu cpu;

    std::string line;
    // fetch
    // while (true) {
    //     if (getchar() && std::getline(fin, line)) {
    //         // decode
    //         try {
    //             auto copy_line = line;
    //             cpu.execute_instruction(line);
    //             cpu.print_registers(false);
    //             std::cout << "ran> " << copy_line << std::endl;
    //         } catch (const std::invalid_argument &e) {
    //             std::cout << e.what() << std::endl;
    //         }
    //     }
    // }
    uint64_t line_number = 0;
    while (std::getline(fin, line)) {
        try {
            ++line_number;
            cpu.execute_instruction(line, line_number);
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }
    cpu.print_registers(false);

    return 0;
}