#include "disassembler.hpp"

#include <fstream>
#include <exception>
#include <iostream>
#include <queue>
#include <iomanip>
#include <memory>

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
    size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
    if(size <= 0)
        throw std::runtime_error("Error during formatting.");
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), size - 1); // Dont include \0
}

// Get mnemonics of opcodes
std::string toString(uint16_t opcode)
{
    // Decode arguments
    uint16_t addr = (opcode & 0x0FFF);
    uint16_t byte_val = (opcode & 0x00FF);
    uint16_t nib_val = (opcode & 0x000F);
    return std::string();
}

/* static */ const int Disassembler::ROM_CODE_BEGIN = 0x0200;

Disassembler::Disassembler()
: PC_{0}, max_address_{0}, romsize_{0}, rom_{nullptr}
{}

uint16_t Disassembler::next()
{
    return (static_cast<uint16_t>(rom_[PC_ - ROM_CODE_BEGIN]) << 8)
        | (static_cast<uint16_t>(rom_[PC_ - ROM_CODE_BEGIN + 1]));
}

void Disassembler::loadROM(std::string path)
{
    std::ifstream romfile(path, std::ios::in | std::ios::binary | std::ios::ate);
    if(!romfile.is_open()){
        throw std::runtime_error("Unable to load the ROM.");
    }

    romsize_ = romfile.tellg();
    max_address_ = ROM_CODE_BEGIN + romsize_;

    std::cout << ";----------------------------------------------------" << std::endl;
    std::cout << "; ROM Name: " << path << std::endl;
    std::cout << "; ROM Size: " << romsize_ << std::endl;
    std::cout << ";----------------------------------------------------" << std::endl;

    // Use reset because rom_ already contains nullptr
    rom_.reset(new uint8_t[romsize_]);
    if(rom_ == nullptr)
    {
        throw std::runtime_error("Unable to allocate sufficient memory for the ROM.");
    }

    // Read the file
    romfile.seekg(0, std::ios::beg);
    romfile.read(reinterpret_cast<char *>(rom_.get()), romsize_);

    romfile.close();
}

void Disassembler::discover()
{
    // Queue to register all the code segments to indentify
    std::queue<uint16_t> segments;

    // Push the code beign segment in the queue
    segments.push(ROM_CODE_BEGIN);

    // Main loop
    while(!segments.empty())
    {
        // Retrieve the next element from the queue
        PC_ = segments.front();
        segments.pop();

        // Walk through this segment of code
        while(PC_ < max_address_ && codemap_.find(PC_) == codemap_.end())
        {
            // Retrieve opcode
            uint16_t opcode = next();

            // Mark address as opcode
            codemap_.insert(PC_);

            // Move the PC forward
            PC_ += 2;

            // Analyze the opcode

            // RET returning to address stored in segments queue
            // if(opcode == 0x00EE)
            //     break;

            // These operands jumps and introduces branches
            switch(opcode & 0xF000)
            {
                case 0x1000:
                    PC_ = opcode & 0x0FFF;
                    labels_.insert(PC_);
                    break;
                default:
                    break;
            }
        }
    }
}

void Disassembler::render()
{
    int count = 0;
    PC_ = ROM_CODE_BEGIN;

    // Loop through the ROM

    while(PC_ < max_address_)
    {
        // If no opcode
        if(codemap_.find(PC_) == codemap_.end())
        {
            // Process non-optcode bytes
            PC_ += 1;
            continue;
        }

        // Print opcode
        uint16_t opcode = next();
        std::cout << string_format("    %-30s; %04X\n", toString(opcode).c_str(), opcode);

        // Move to next opcode
        PC_ += 2;
    }

    std::cout << std::endl;
}