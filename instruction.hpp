#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <bits/stdc++.h>
#include "global.hpp"

void lui(const u32 &imm,u32 &val)
{
    val=sext(imm,0,19)<<12;
}

void auipc(const u32 &imm,const u32 &IA,u32 &val)
{
    val=IA+(sext(imm,0,19)<<12);
}

void jal(const u32 &offset,const u32 &IA,u32 &val,u32 &TA)
{
    val=IA+4;
    TA=IA+sext(offset,0,20);
}

void jalr(const u32 &offset,const u32 &rv1,const u32 &IA,u32 &val,u32 &TA)
{
    val=IA+4;
    TA=(rv1+sext(offset,0,11))&(~1);
}

bool beq(const u32 &rv1,const u32 &rv2,const u32 &offset,const u32 &IA,u32 &TA)
{
    if (rv1==rv2) {
        TA=IA+sext(offset,0,12);
        return true;
    }
    else {
        TA=IA+4;
        return false;
    }
}

bool bne(const u32 &rv1,const u32 &rv2,const u32 &offset,const u32 &IA,u32 &TA)
{
    if (rv1!=rv2) {
        TA=IA+sext(offset,0,12);
        return true;
    }
    else {
        TA=IA+4;
        return false;
    }
}

bool blt(const u32 &rv1,const u32 &rv2,const u32 &offset,const u32 &IA,u32 &TA)
{
    if ((int)rv1<(int)rv2) {
        TA=IA+sext(offset,0,12);//note should convert u32 to int
        return true;
    }
    else {
        TA=IA+4;
        return false;
    }
}

bool bge(const u32 &rv1,const u32 &rv2,const u32 &offset,const u32 &IA,u32 &TA)
{
    if ((int)rv1>=(int)rv2) {
        TA=IA+sext(offset,0,12);//note should convert u32 to int
        return true;
    }
    else {
        TA=IA+4;
        return false;
    }
}

bool bltu(const u32 &rv1,const u32 &rv2,const u32 &offset,const u32 &IA,u32 &TA)
{
    if (rv1<rv2) {
        TA=IA+sext(offset,0,12);
        return true;
    }
    else {
        TA=IA+4;
        return false;
    }
}

bool bgeu(const u32 &rv1,const u32 &rv2,const u32 &offset,const u32 &IA,u32 &TA)
{
    if (rv1>=rv2) {
        TA=IA+sext(offset,0,12);
        return true;
    }
    else {
        TA=IA+4;
        return false;
    }
}

void lb(const u32 &imm,const u32 &rv1,u32 &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lh(const u32 &imm,const u32 &rv1,u32 &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lw(const u32 &imm,const u32 &rv1,u32 &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lbu(const u32 &imm,const u32 &rv1,u32 &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lhu(const u32 &imm,const u32 &rv1,u32 &pos)
{
    pos=rv1+sext(imm,0,11);
}

void sb(const u32 &imm,const u32 &rv1,u32 &pos)
{
    pos=rv1+sext(imm,0,11);
}
    
void sh(const u32 &imm,const u32 &rv1,u32 &pos)
{
    pos=rv1+sext(imm,0,11);
}

void sw(const u32 &imm,const u32 &rv1,u32 &pos)
{
    pos=rv1+sext(imm,0,11);
}

void addi(const u32 &imm,const u32 &rv1,u32 &val)
{
    val=rv1+sext(imm,0,11);
}

void slti(const u32 &rv1,const u32 &imm,u32 &val)
{
    val=(((int)rv1<(int)sext(imm,0,11))?1:0);
}

void sltiu(const u32 &rv1,const u32 &imm,u32 &val)
{
    val=((rv1<sext(imm,0,11))?1:0);
}

void xori(const u32 &rv1,const u32 &imm,u32 &val)
{
    val=rv1^sext(imm,0,11);
}

void ori(const u32 &rv1,const u32 &imm,u32 &val)
{
    val=rv1|sext(imm,0,11);
}

void andi(const u32 &rv1,const u32 &imm,u32 &val)
{
    val=rv1&sext(imm,0,11);
}

void slli(const u32 &rv1,const u32 &imm,u32 &val)
{
    val=rv1<<imm;
}

void srli(const u32 &rv1,const u32 &imm,u32 &val)
{
    val=rv1>>imm;
}

void srai(const u32 &rv1,const u32 &imm,u32 &val)
{
    u32 move_bit=imm;
    val=sext(rv1>>move_bit,0,31-move_bit);
}

void add(const u32 &rv1,const u32 &rv2,u32 &val)
{
    val=rv1+rv2;
}

void sub(const u32 &rv1,const u32 &rv2,u32 &val)
{
    val=rv1-rv2;
}

void sll(const u32 &rv1,const u32 &rv2,u32 &val)
{
    val=rv1<<get_bit(rv2,0,4);
}

void slt(const u32 &rv1,const u32 &rv2,u32 &val)
{
    val=(((int)rv1<(int)rv2)?1:0);
}

void sltu(const u32 &rv1,const u32 &rv2,u32 &val)
{
    val=((rv1<rv2)?1:0);
}

void xor_(const u32 &rv1,const u32 &rv2,u32 &val)
{
    val=rv1^rv2;
}

void srl(const u32 &rv1,const u32 &rv2,u32 &val)
{
    val=rv1>>get_bit(rv2,0,4);
}

void sra(const u32 &rv1,const u32 &rv2,u32 &val)
{
    u32 move_bit=get_bit(rv2,0,4);
    val=sext(rv1>>move_bit,0,31-move_bit);
}

void or_(const u32 &rv1,const u32 &rv2,u32 &val)
{
    val=rv1|rv2;
}

void and_(const u32 &rv1,const u32 &rv2,u32 &val)
{
    val=rv1&rv2;
}

#endif