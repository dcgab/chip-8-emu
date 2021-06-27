#pragma once

#include <string>
#include <memory>
#include <set>

class Disassembler
{
    public:
        Disassembler();

        void loadROM(std::string path);
        void discover();
        void render();
    private:
        uint16_t next();

        uint16_t PC_;
        uint16_t max_address_;
        std::streampos romsize_;
        // Needed read-only dynamic buffer when reading the file.
        // Vector was too overpowered
        std::unique_ptr<uint8_t[]> rom_; 
        std::set<uint16_t> labels_;
        std::set<uint16_t> codemap_;

        static const int ROM_CODE_BEGIN;
};