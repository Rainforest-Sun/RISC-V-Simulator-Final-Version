#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <bits/stdc++.h>
#include "global.hpp"

void lui(const unsigned &imm,unsigned &val)
{
    val=sext(imm,0,19)<<12;
}

void auipc(const unsigned &imm,const unsigned &IA,unsigned &val)
{
    val=IA+(sext(imm,0,19)<<12);
}

void jal(const unsigned &offset,const unsigned &IA,unsigned &val,unsigned &TA)
{
    val=IA+4;
    TA=IA+sext(offset,0,20);
}

void jalr(const unsigned &offset,const unsigned &rv1,const unsigned &IA,unsigned &val,unsigned &TA)
{
    val=IA+4;
    TA=(rv1+sext(offset,0,11))&(~1);
}

bool beq(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
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

bool bne(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
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

bool blt(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
{
    if ((int)rv1<(int)rv2) {
        TA=IA+sext(offset,0,12);//note should convert unsigned to int
        return true;
    }
    else {
        TA=IA+4;
        return false;
    }
}

bool bge(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
{
    if ((int)rv1>=(int)rv2) {
        TA=IA+sext(offset,0,12);//note should convert unsigned to int
        return true;
    }
    else {
        TA=IA+4;
        return false;
    }
}

bool bltu(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
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

bool bgeu(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
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

void lb(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lh(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lw(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lbu(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lhu(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void sb(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}
    
void sh(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void sw(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void addi(const unsigned &imm,const unsigned &rv1,unsigned &val)
{
    val=rv1+sext(imm,0,11);
}

void slti(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=(((int)rv1<(int)sext(imm,0,11))?1:0);
}

void sltiu(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=((rv1<sext(imm,0,11))?1:0);
}

void xori(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=rv1^sext(imm,0,11);
}

void ori(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=rv1|sext(imm,0,11);
}

void andi(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=rv1&sext(imm,0,11);
}

void slli(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=rv1<<imm;
}

void srli(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=rv1>>imm;
}

void srai(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    unsigned move_bit=imm;
    val=sext(rv1>>move_bit,0,31-move_bit);
}

void add(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1+rv2;
}

void sub(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1-rv2;
}

void sll(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1<<get_bit(rv2,0,4);
}

void slt(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=(((int)rv1<(int)rv2)?1:0);
}

void sltu(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=((rv1<rv2)?1:0);
}

void xor_(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1^rv2;
}

void srl(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1>>get_bit(rv2,0,4);
}

void sra(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    unsigned move_bit=get_bit(rv2,0,4);
    val=sext(rv1>>move_bit,0,31-move_bit);
}

void or_(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1|rv2;
}

void and_(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1&rv2;
}

#endif