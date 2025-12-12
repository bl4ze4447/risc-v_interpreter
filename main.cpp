#include <iostream>
#include <fstream>
#include "cpu.h"

int main() {
    std::ifstream fin("risc-v.asm");
    cpu cpu;

    std::string line;
    // fetch
    while (std::getline(fin, line)) {
        // decode
        try {
            cpu.execute_instruction(line);
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }
    cpu.print_registers(false);

    return 0;
}