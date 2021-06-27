#include "disassembler.hpp"
#include <stdexcept>

#include <memory>
#include <iostream>
int main(int argc, char** argv)
{
    if(argc == 1)
    {
        throw std::invalid_argument("Missing path to ROM file.");
    }

    Disassembler dasm;

    // Load ROM
    dasm.loadROM(argv[1]);

    // Analyze code
    dasm.discover();

    // Display code
    dasm.render();
    
    return 0;
}