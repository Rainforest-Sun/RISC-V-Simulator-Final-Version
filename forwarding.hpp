#ifndef FORWARDING_HPP
#define FORWARDING_HPP

#include <bits/stdc++.h>
#include "global.hpp"
#include "buffer.hpp"

void forwarding1_1(INSTRUCTION &ins)
{
    if (IN_WB.flag) {
        switch (IN_WB.ins.ins_type) {
            case LUI:case AUIPC:case JAL:case JALR:case LB:case LH:case LW:
            case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
            case ANDI:case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:
            case SLT:case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
            if (IN_WB.ins.rd!=0 && IN_WB.ins.rd==ins.rs1) ins.rv1=IN_WB.VAL;
        }
    }
}

void forwarding1_2(INSTRUCTION &ins)
{
    if (IN_WB.flag) {
        switch (IN_WB.ins.ins_type) {
            case LUI:case AUIPC:case JAL:case JALR:case LB:case LH:case LW:
            case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
            case ANDI:case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:
            case SLT:case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
            if (IN_WB.ins.rd!=0 && IN_WB.ins.rd==ins.rs1) ins.rv1=IN_WB.VAL;
            if (IN_WB.ins.rd!=0 && IN_WB.ins.rd==ins.rs2) ins.rv2=IN_WB.VAL;
        }
    }
}

void forwarding2_1(INSTRUCTION &ins)
{
    if (IN_MEM.flag) {
        switch (IN_MEM.ins.ins_type) {
            //LB之类的办不了
            case LUI:case AUIPC:case JAL:case JALR:
            case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
            case ANDI:case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:
            case SLT:case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
            if (IN_MEM.ins.rd!=0 && IN_MEM.ins.rd==ins.rs1) ins.rv1=IN_MEM.CR;
        }
    }
}

void forwarding2_2(INSTRUCTION &ins)
{
    if (IN_MEM.flag) {
        switch (IN_MEM.ins.ins_type) {
            //LB之类的办不了
            case LUI:case AUIPC:case JAL:case JALR:
            case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
            case ANDI:case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:
            case SLT:case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
            if (IN_MEM.ins.rd!=0 && IN_MEM.ins.rd==ins.rs1) ins.rv1=IN_MEM.CR;
            if (IN_MEM.ins.rd!=0 && IN_MEM.ins.rd==ins.rs2) ins.rv2=IN_MEM.CR;
        }
    }
}

void forwarding1(INSTRUCTION &ins)
{
    switch (ins.ins_type) {
        case JALR:case LB:case LH:case LW:case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:case ANDI:
        case SLLI:case SRLI:case SRAI: {
            forwarding1_1(ins);
            break;
        }
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:case SB:case SH:case SW:case ADD:case SUB:case SLL:case SLT:case SLTU:
        case SRL:case SRA:case XOR:case OR:case AND: {
            forwarding1_2(ins);
            break;
        }
    }
}

void forwarding2(INSTRUCTION &ins)
{
    switch (ins.ins_type) {
        case JALR:case LB:case LH:case LW:case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:case ANDI:
        case SLLI:case SRLI:case SRAI: {
            forwarding2_1(ins);
            break;
        }
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:case SB:case SH:case SW:case ADD:case SUB:case SLL:case SLT:case SLTU:
        case SRL:case SRA:case XOR:case OR:case AND: {
            forwarding2_2(ins);
            break;
        }
    }
}

void forwarding(INSTRUCTION &ins)
{
    forwarding1(ins);
    forwarding2(ins);
}

#endif