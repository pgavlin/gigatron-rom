#include <stdio.h>

#include "disassembler.h"

namespace Disassembler
{
    enum
    {
        LDWI = 0x11,
        LD = 0x1a,
        LDW = 0x21,
        STW = 0x2b,
        BCC = 0x35,
        EQ = 0x3f,
        GT = 0x4d,
        LT = 0x50,
        GE = 0x53,
        LE = 0x56,
        LDI = 0x59,
        ST = 0x5e,
        POP = 0x63,
        NE = 0x72,
        PUSH = 0x75,
        LUP = 0x7f,
        ANDI = 0x82,
        ORI = 0x88,
        XORI = 0x8c,
        BRA = 0x90,
        INC = 0x93,
        ADDW = 0x99,
        PEEK = 0xad,
        SYS = 0xb4,
        SUBW = 0xb8,
        DEF = 0xcd,
        CALL = 0xcf,
        ALLOC = 0xdf,
        ADDI = 0xe3,
        SUBI = 0xe6,
        LSLW = 0xe9,
        STLW = 0xec,
        LDLW = 0xee,
        POKE = 0xf0,
        DOKE = 0xf3,
        DEEK = 0xf6,
        ANDW = 0xf8,
        ORW = 0xfa,
        XORW = 0xfc,
        RET = 0xff,
    };

    uint16_t disassemble(uint16_t pc, ReadByteF readByte, char* buffer)
    {
        // The longest vCPU instruction is three bytes. For simplicity, we'll always read three, even if we don't
        // need them all.
        uint8_t b[3];
        b[0] = readByte(pc), b[1] = readByte(pc + 1), b[2] = readByte(pc + 2);

        // Lookup the instruction's information.
        const char* fmt = "(ILLEGAL)";
        uint16_t op;
        uint16_t size = 1;
        switch (b[0])
        {
            case LDWI: fmt = "LDWI %04hX", size = 3, op = (uint16_t)b[1] | ((uint16_t)b[2] << 8); break;
            case LD: fmt = "LD %02hX", size = 2, op = b[1]; break;
            case LDW: fmt = "LDW %02hX", size = 2, op = b[1]; break;
            case STW: fmt = "STW %02hX", size = 2, op = b[1]; break;
            case LDI: fmt = "LDI %02hX", size = 2, op = b[1]; break;
            case ST: fmt = "ST %02hX", size = 2, op = b[1]; break;
            case POP: fmt = "POP", size = 1; break;
            case PUSH: fmt = "PUSH", size = 1; break;
            case LUP: fmt = "LUP %02hX", size = 2, op = b[1]; break;
            case ANDI: fmt = "ANDI %02hX", size = 2, op = b[1]; break;
            case ORI: fmt = "ORI %02hX", size = 2, op = b[1]; break;
            case XORI: fmt = "XORI %02hX", size = 2, op = b[1]; break;
            case BRA: fmt = "BRA %02hX", size = 2, op = b[1]; break;
            case INC: fmt = "INC %02hX", size = 2, op = b[1]; break;
            case ADDW: fmt = "ADDW %02hX", size = 2, op = b[1]; break;
            case PEEK: fmt = "PEEK", size = 1; break;
            case SYS: fmt = "SYS %02hX", size = 2, op = b[1]; break;
            case SUBW: fmt = "SUBW %02hX", size = 2, op = b[1]; break;
            case DEF: fmt = "DEF %02hX", size = 2, op = b[1]; break;
            case CALL: fmt = "CALL %02hX", size = 2, op = b[1]; break;
            case ALLOC: fmt = "ALLOC %02hX", size = 2, op = b[1]; break;
            case ADDI: fmt = "ADDI %02hX", size = 2, op = b[1]; break;
            case SUBI: fmt = "SUBI %02hX", size = 2, op = b[1]; break;
            case LSLW: fmt = "LSLW", size = 1; break;
            case STLW: fmt = "STLW %02hX", size = 2, op = b[1]; break;
            case LDLW: fmt = "LDLW %02hX", size = 2, op = b[1]; break;
            case POKE: fmt = "POKE %02hX", size = 2, op = b[1]; break;
            case DOKE: fmt = "DOKE %02hX", size = 2, op = b[1]; break;
            case DEEK: fmt = "DEEK", size = 1; break;
            case ANDW: fmt = "ANDW %02hX", size = 2, op = b[1]; break;
            case ORW: fmt = "ORW %02hX", size = 2, op = b[1]; break;
            case XORW: fmt = "XORW %02hX", size = 2, op = b[1]; break;
            case RET: fmt = "RET", size = 1; break;
            case BCC:
                switch (b[1])
                {
                    case EQ: fmt = "BEQ %02hX"; break;
                    case GT: fmt = "BGT %02hX"; break;
                    case LT: fmt = "BLT %02hX"; break;
                    case GE: fmt = "BGE %02hX"; break;
                    case LE: fmt = "BLE %02hX"; break;
                    case NE: fmt = "BNE %02hX"; break;
                }
                size = 3;
                op = b[2];
                break;
        }

        sprintf(buffer, fmt, op);
        return size;
    }
}
