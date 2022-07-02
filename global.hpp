#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <bits/stdc++.h>

int clk=0;
unsigned pc;
unsigned reg[32];//register
unsigned mem[500005];//memory ,note that though type of mem is unsigned, only low 8 bits legal
bool not_do_IF=false;
bool should_not_do_IF=true;
bool BUBBLE=false;
bool LAST_CYCLE_IS_BUBBLE=false;
bool NEED_TO_CUT=false;
const unsigned Full=-1;//all 1 with length of 32

enum Ins_type {
    LB,LH,LW,LBU,LHU,
    SB,SH,SW,
    ADD,ADDI,SUB,LUI,AUIPC,XOR,XORI,OR,ORI,AND,ANDI,
    SLL,SLLI,SRL,SRLI,SRA,SRAI,SLT,SLTI,SLTU,SLTIU,
    BEQ,BNE,BLT,BGE,BLTU,BGEU,
    JAL,JALR
};

struct INSTRUCTION {
   Ins_type ins_type;
   unsigned rs1,rs2,rd,imm,offset,shamt,rv1,rv2;
   bool is_branch;
};

inline unsigned sext(const unsigned &x,const unsigned &l,const unsigned &r)//signed-extend
{
    bool sign=((unsigned)1<<r)&x;
    if (sign) return (Full<<(r+1))|x;
    else return x;
}

inline unsigned get_bit(const unsigned &x,const unsigned &l,const unsigned &r)//get x from l to r
{
    return (((Full>>(31-r+l))<<l)&x)>>l;
}

#endif