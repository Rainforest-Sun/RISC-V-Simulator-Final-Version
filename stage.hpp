#ifndef STAGE_HPP
#define STAGE_HPP

#include <bits/stdc++.h>
#include "instruction.hpp"
#include "stage_ins.hpp"
#include "forwarding.hpp"
#include "predictor.hpp"

bool do_IF()
{
    unsigned s=get_ins();
    if (s==(int)0x0ff00513) return false;
    unsigned IA=get_pc();
    predictPC(IA,s);
    IF_ID_BUFFER.flag=true;
    IF_ID_BUFFER.INS=s,IF_ID_BUFFER.IA=IA;
    return true;
}

void do_ID()
{
    INSTRUCTION ins=decode_ins(IN_ID.INS);
    switch (ins.ins_type) {
        case JALR:case LB:case LH:case LW:case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:case ANDI:
        case SLLI:case SRLI:case SRAI: {
            ins.rv1=reg[ins.rs1];
            forwarding1_1(ins);
            break;
        }
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:case SB:case SH:case SW:case ADD:case SUB:case SLL:case SLT:case SLTU:
        case SRL:case SRA:case XOR:case OR:case AND: {
            ins.rv1=reg[ins.rs1];
            ins.rv2=reg[ins.rs2];
            forwarding1_2(ins);
            break;
        }
    }
    ID_EX_BUFFER.ins=ins,ID_EX_BUFFER.IA=IN_ID.IA;

    ID_EX_BUFFER.flag=true;
}

void do_EX()
{
    INSTRUCTION ins=IN_EX.ins;
    unsigned IA=IN_EX.IA;
    unsigned CR,TA;
    switch (ins.ins_type) {
        case LUI:lui(ins.imm,CR);break;
        case AUIPC:auipc(ins.imm,IA,CR);break;
        case JAL:jal(ins.offset,IA,CR,TA);break;
        case JALR:jalr(ins.offset,ins.rv1,IA,CR,TA);break;
        case BEQ:{
            if (!beq(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case BNE:{
            if (!bne(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case BLT:{
            if (!blt(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case BGE:{
            if (!bge(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case BLTU:{
            if (!bltu(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case BGEU:{
            if (!bgeu(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case LB:lb(ins.imm,ins.rv1,CR);break;
        case LH:lh(ins.imm,ins.rv1,CR);break;
        case LW:lw(ins.imm,ins.rv1,CR);break;
        case LBU:lbu(ins.imm,ins.rv1,CR);break;
        case LHU:lhu(ins.imm,ins.rv1,CR);break;
        case SB:sb(ins.imm,ins.rv1,CR);break;
        case SH:sh(ins.imm,ins.rv1,CR);break;
        case SW:sw(ins.imm,ins.rv1,CR);break;
        case ADDI:addi(ins.imm,ins.rv1,CR);break;
        case SLTI:slti(ins.rv1,ins.imm,CR);break;
        case SLTIU:sltiu(ins.rv1,ins.imm,CR);break;
        case XORI:xori(ins.rv1,ins.imm,CR);break;
        case ORI:ori(ins.rv1,ins.imm,CR);break;
        case ANDI:andi(ins.rv1,ins.imm,CR);break;
        case SLLI:slli(ins.rv1,ins.shamt,CR);break;
        case SRLI:srli(ins.rv1,ins.shamt,CR);break;
        case SRAI:srai(ins.rv1,ins.shamt,CR);break;
        case ADD:add(ins.rv1,ins.rv2,CR);break;
        case SUB:sub(ins.rv1,ins.rv2,CR);break;
        case SLL:sll(ins.rv1,ins.rv2,CR);break;
        case SLT:slt(ins.rv1,ins.rv2,CR);break;
        case SLTU:sltu(ins.rv1,ins.rv2,CR);break;
        case XOR:xor_(ins.rv1,ins.rv2,CR);break;
        case SRL:srl(ins.rv1,ins.rv2,CR);break;
        case SRA:sra(ins.rv1,ins.rv2,CR);break;
        case OR:or_(ins.rv1,ins.rv2,CR);break;
        case AND:and_(ins.rv1,ins.rv2,CR);break;
    }
    EX_MEM_BUFFER.CR=CR;
    EX_MEM_BUFFER.TA=TA;
    EX_MEM_BUFFER.ins=ins;
    EX_MEM_BUFFER.IA=IA;

    EX_MEM_BUFFER.flag=true;
}

void do_MEM()
{
    INSTRUCTION ins=IN_MEM.ins;
    unsigned VAL=IN_MEM.CR;
    unsigned IA=IN_MEM.IA;
    unsigned TA=IN_MEM.TA;
    switch (ins.ins_type) {
        case JAL:case JALR:{
            if (IN_EX.IA!=TA) {
                NEED_TO_CUT=true;
                MEM_WB_BUFFER.NEED_TO_CHANGE_TA=true;
                should_not_do_IF=false;
            }
            break;
        }
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:{
            bool is_succ=IN_EX.IA==TA;
            update(is_succ,ins.is_branch,IA,TA);
            if (IN_EX.IA!=TA) {
                NEED_TO_CUT=true;
                MEM_WB_BUFFER.NEED_TO_CHANGE_TA=true;
                should_not_do_IF=false;
            }
            break;
        }
        case LB:VAL=sext(read_bit_from_mem(VAL),0,7);break;
        case LH:VAL=sext(read_bit_from_mem(VAL)|read_bit_from_mem(VAL+1)<<8,0,15);break;
        case LW:VAL=read_bit_from_mem(VAL)|read_bit_from_mem(VAL+1)<<8|read_bit_from_mem(VAL+2)<<16|read_bit_from_mem(VAL+3)<<24;break;
        case LBU:VAL=read_bit_from_mem(VAL);break;
        case LHU:VAL=read_bit_from_mem(VAL)|read_bit_from_mem(VAL+1)<<8;break;
        case SB:store_to_mem(VAL,get_bit(ins.rv2,0,7));break;
        case SH:{
            store_to_mem(VAL,get_bit(ins.rv2,0,7));
            store_to_mem(VAL+1,get_bit(ins.rv2,8,15));
            break;
        }
        case SW:{
            store_to_mem(VAL,get_bit(ins.rv2,0,7));
            store_to_mem(VAL+1,get_bit(ins.rv2,8,15));
            store_to_mem(VAL+2,get_bit(ins.rv2,16,23));
            store_to_mem(VAL+3,get_bit(ins.rv2,24,31));
            break;
        }
    }
    MEM_WB_BUFFER.ins=ins;
    MEM_WB_BUFFER.VAL=VAL;
    MEM_WB_BUFFER.IA=IA;
    MEM_WB_BUFFER.TA=TA;

    MEM_WB_BUFFER.flag=true;
}

void do_WB()
{
    INSTRUCTION ins=IN_WB.ins;
    unsigned VAL=IN_WB.VAL;
    switch (ins.ins_type) {
        case LUI:case AUIPC:case JAL:case JALR:case LB:case LH:case LW:
        case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
        case ANDI:case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:
        case SLT:case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
        store_to_reg(ins.rd,VAL);break;
    }
}

void syn()
{
    if (MEM_WB_BUFFER.NEED_TO_CHANGE_TA) {
        change_pc(MEM_WB_BUFFER.TA);
    }
    if (NEED_TO_CUT) {
        del_ins_now();
        NEED_TO_CUT=false;
    }
    if (!should_not_do_IF) {
        not_do_IF=false;
        should_not_do_IF=true;
    }
    IN_ID=IF_ID_BUFFER;
    IN_EX=ID_EX_BUFFER;
    IN_MEM=EX_MEM_BUFFER;
    IN_WB=MEM_WB_BUFFER;
    MEM_WB_BUFFER.NEED_TO_CHANGE_TA=false;
    if (not_do_IF) IF_ID_BUFFER.flag=false;
    if (!IN_ID.flag) ID_EX_BUFFER.flag=false;
    if (!IN_EX.flag) EX_MEM_BUFFER.flag=false;
    if (!IN_MEM.flag) MEM_WB_BUFFER.flag=false;
    
    if (IN_EX.flag && IN_MEM.flag) {
        switch (IN_MEM.ins.ins_type) {
            case LB:case LH:case LW:case LBU:case LHU:{
                switch (IN_EX.ins.ins_type) {
                    case JALR:case LB:case LH:case LW:case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:case ANDI:
                    case SLLI:case SRLI:case SRAI: {
                        if (IN_MEM.ins.rd==IN_EX.ins.rs1) bubble();
                        break;
                    }
                    case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:case SB:case SH:case SW:case ADD:case SUB:case SLL:case SLT:case SLTU:
                    case SRL:case SRA:case XOR:case OR:case AND: {
                        if (IN_MEM.ins.rd==IN_EX.ins.rs1 || IN_MEM.ins.rd==IN_EX.ins.rs2) bubble();
                        break;
                    }  
                }  
            }
        }
    }
    forwarding(ID_EX_BUFFER.ins);
    forwarding(IN_EX.ins);
}

void IF()
{
    if (!not_do_IF && !BUBBLE) {
        if (!do_IF()) not_do_IF=true,IF_ID_BUFFER.flag=false;
    }
}

void ID()
{
    if (IN_ID.flag && !BUBBLE) do_ID();
}

void EX()
{
    if (IN_EX.flag && !BUBBLE) do_EX();
}

void MEM()
{
    if (IN_MEM.flag) do_MEM();
}

void WB()
{
    if (IN_WB.flag) do_WB();
}

#endif