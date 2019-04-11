#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdint.h>

namespace Disassembler
{
    typedef uint8_t (*ReadByteF)(uint16_t addr);

    uint16_t disassemble(uint16_t pc, ReadByteF readByte, char* buffer);
}

#endif
