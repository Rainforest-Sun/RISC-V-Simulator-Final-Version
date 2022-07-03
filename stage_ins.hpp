#ifndef STAGE_INS_HPP
#define STAGE_INS_HPP

#include <bits/stdc++.h>
#include "buffer.hpp"



inline u32 get_pc()
{
    return pc;
}

void change_pc(const u32 &TA)
{
    pc=TA;
}

inline void del_ins_now()
{
    IF_ID_BUFFER.flag=IN_ID.flag=ID_EX_BUFFER.flag=IN_EX.flag=EX_MEM_BUFFER.flag=IN_MEM.flag=false;
}

void bubble()
{
    if (!LAST_CYCLE_IS_BUBBLE) BUBBLE=true;
}

INSTRUCTION decode_ins(const u32 &s)
{
    INSTRUCTION res;
    u8 opcode=get_bit(s,0,6);//get 0-6 bits
    if (opcode==0b0110111) {
        u32 rd=get_bit(s,7,11),imm=get_bit(s,12,31);
        res.ins_type=LUI,res.rd=rd,res.imm=imm;
    }
    else if (opcode==0b0010111) {
        u32 rd=get_bit(s,7,11),imm=get_bit(s,12,31);
        res.ins_type=AUIPC,res.rd=rd,res.imm=imm;
    }
    else if (opcode==0b1101111) {
        u32 rd=get_bit(s,7,11);
        u32 offset=get_bit(s,21,30)<<1|get_bit(s,20,20)<<11|get_bit(s,12,19)<<12|get_bit(s,31,31)<<20;
        res.ins_type=JAL,res.rd=rd,res.offset=offset;
    }
    else if (opcode==0b1100111) {
        u32 rd=get_bit(s,7,11),offset=get_bit(s,20,31),rs1=get_bit(s,15,19);
        res.ins_type=JALR,res.rd=rd,res.offset=offset,res.rs1=rs1;
    }
    else if (opcode==0b1100011) {
        u32 offset=get_bit(s,8,11)<<1|get_bit(s,25,30)<<5|get_bit(s,7,7)<<11|get_bit(s,31,31)<<12;
        u32 rs1=get_bit(s,15,19),rs2=get_bit(s,20,24);
        u32 func3=get_bit(s,12,14);
        res.offset=offset,res.rs1=rs1,res.rs2=rs2;
        if (func3==0b000) res.ins_type=BEQ;
        else if (func3==0b001) res.ins_type=BNE;
        else if (func3==0b100) res.ins_type=BLT;
        else if (func3==0b101) res.ins_type=BGE;
        else if (func3==0b110) res.ins_type=BLTU;
        else if (func3==0b111) res.ins_type=BGEU;
    }
    else if (opcode==0b0000011) {
        u32 imm=get_bit(s,20,31),rs1=get_bit(s,15,19),rd=get_bit(s,7,11);
        u32 func3=get_bit(s,12,14);
        res.imm=imm,res.rs1=rs1,res.rd=rd;
        if (func3==0b000) res.ins_type=LB;
        else if (func3==0b001) res.ins_type=LH;
        else if (func3==0b010) res.ins_type=LW;
        else if (func3==0b100) res.ins_type=LBU;
        else if (func3==0b101) res.ins_type=LHU;
    }
    else if (opcode==0b0100011) {
        u32 imm=get_bit(s,7,11)|get_bit(s,25,31)<<5;
        u32 rs1=get_bit(s,15,19),rs2=get_bit(s,20,24);
        u32 func3=get_bit(s,12,14);
        res.imm=imm,res.rs1=rs1,res.rs2=rs2;
        if (func3==0b000) res.ins_type=SB;
        else if (func3==0b001) res.ins_type=SH;
        else if (func3==0b010) res.ins_type=SW;
    }
    else if (opcode==0b0010011) {
        u32 imm=get_bit(s,20,31),shamt=get_bit(s,20,25),rs1=get_bit(s,15,19),rd=get_bit(s,7,11);
        u32 func3=get_bit(s,12,14),func7=get_bit(s,26,31);
        res.rd=rd,res.rs1=rs1;
        if (func3==0b000) res.ins_type=ADDI,res.imm=imm;
        else if (func3==0b010) res.ins_type=SLTI,res.imm=imm;
        else if (func3==0b011) res.ins_type=SLTIU,res.imm=imm;
        else if (func3==0b100) res.ins_type=XORI,res.imm=imm;
        else if (func3==0b110) res.ins_type=ORI,res.imm=imm;
        else if (func3==0b111) res.ins_type=ANDI,res.imm=imm;
        else if (func3==0b001) res.ins_type=SLLI,res.shamt=shamt;
        else if (func3==0b101 && func7==0b000000) res.ins_type=SRLI,res.shamt=shamt;
        else if (func3==0b101 && func7==0b010000) res.ins_type=SRAI,res.shamt=shamt;
    }
    else if (opcode==0b0110011) {
        u32 rd=get_bit(s,7,11),rs1=get_bit(s,15,19),rs2=get_bit(s,20,24);
        u32 func3=get_bit(s,12,14),func7=get_bit(s,25,31);
        res.rd=rd,res.rs1=rs1,res.rs2=rs2;
        if (func3==0b000 && func7==0b0000000) res.ins_type=ADD;
        else if (func3==0b000 && func7==0b0100000) res.ins_type=SUB;
        else if (func3==0b001) res.ins_type=SLL;
        else if (func3==0b010) res.ins_type=SLT;
        else if (func3==0b011) res.ins_type=SLTU;
        else if (func3==0b100) res.ins_type=XOR;
        else if (func3==0b101 && func7==0b0000000) res.ins_type=SRL;
        else if (func3==0b101 && func7==0b0100000) res.ins_type=SRA;
        else if (func3==0b110) res.ins_type=OR;
        else if (func3==0b111) res.ins_type=AND;
    }
    return res;
}


#endif