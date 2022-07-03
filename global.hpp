#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <bits/stdc++.h>


using u8=unsigned char;
using u32=unsigned int;
int clk=0;
u32 pc=0;
bool not_do_IF=false;
bool should_not_do_IF=true;
bool BUBBLE=false;
bool LAST_CYCLE_IS_BUBBLE=false;
bool NEED_TO_CUT=false;
const u32 Full=-1;//all 1 with length of 32

enum Ins_type {
    LB,LH,LW,LBU,LHU,
    SB,SH,SW,
    ADD,ADDI,SUB,LUI,AUIPC,XOR,XORI,OR,ORI,AND,ANDI,
    SLL,SLLI,SRL,SRLI,SRA,SRAI,SLT,SLTI,SLTU,SLTIU,
    BEQ,BNE,BLT,BGE,BLTU,BGEU,
    JAL,JALR,
    NO
};

struct INSTRUCTION {
    Ins_type ins_type;
    u8 rs1,rs2,rd;
    u32 imm,offset,shamt,rv1,rv2;
    bool is_branch;
    INSTRUCTION():ins_type(NO),rs1(0),rs2(0),rd(0),imm(0),offset(0),shamt(0),rv1(0),rv2(0),is_branch(false){}
    void clear()
    {
        ins_type=NO;
        rs1=rs2=rd=0;
        imm=offset=shamt=rv1=rv2=0;
        is_branch=false;
    }
};

inline u32 sext(const u32 &x,const u32 &l,const u32 &r)//signed-extend
{
    bool sign=((unsigned)1<<r)&x;
    if (sign) return (Full<<(r+1))|x;
    else return x;
}

inline u32 get_bit(const u32 &x,const u32 &l,const u32 &r)//get x from l to r
{
    return (((Full>>(31-r+l))<<l)&x)>>l;
}

inline u32 get_pc()
{
    return pc;
}

void change_pc(const u32 &TA)
{
    pc=TA;
}

const char *order_name[38]={
    "LB","LH","LW","LBU","LHU",
    "SB","SH","SW",
    "ADD","ADDI","SUB","LUI","AUIPC","XOR","XORI","OR","ORI","AND","ANDI",
    "SLL","SLLI","SRL","SRLI","SRA","SRAI","SLT","SLTI","SLTU","SLTIU",
    "BEQ","BNE","BLT","BGE","BLTU","BGEU",
    "JAL","JALR",
    "NO"
};
#endif //RISC-V Simulator GLOBAL