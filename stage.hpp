#ifndef STAGE_HPP
#define STAGE_HPP

#include <bits/stdc++.h>
#include "instruction.hpp"
#include "forwarding.hpp"
#include "predictor.hpp"
#include "Memory.hpp"
#include "Register.hpp"

class StageIF
{
public:
    Memory &memory;
    Predictor &predictor;
    IF_ID_BUFFER_CLASS &IF_ID_BUFFER;
    StageIF(Memory &_memory,Predictor &_predictor,IF_ID_BUFFER_CLASS &_IF_ID_BUFFER):memory(_memory),predictor(_predictor),IF_ID_BUFFER(_IF_ID_BUFFER){}
    bool do_IF()
    {
        u32 s=memory.get_ins();
        if (s==(int)0x0ff00513) return false;
        u32 IA=get_pc();
        predictor.predictPC(IA,s);
        IF_ID_BUFFER.flag=true;
        IF_ID_BUFFER.INS=s,IF_ID_BUFFER.IA=IA;
        return true;
    }
    void exe()
    {
        if (!not_do_IF && !BUBBLE) {
            if (!do_IF()) not_do_IF=true,IF_ID_BUFFER.flag=false;
        }
    }
};

class StageID
{
public:
    IF_ID_BUFFER_CLASS &IN_ID;
    ID_EX_BUFFER_CLASS &ID_EX_BUFFER;
    MEM_WB_BUFFER_CLASS &IN_WB;
    Register &regi;
    StageID(IF_ID_BUFFER_CLASS &_IN_ID,ID_EX_BUFFER_CLASS &_ID_EX_BUFFER,MEM_WB_BUFFER_CLASS &_IN_WB,Register &_regi):IN_ID(_IN_ID),ID_EX_BUFFER(_ID_EX_BUFFER),IN_WB(_IN_WB),regi(_regi){}

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

    void do_ID()
    {
        INSTRUCTION ins=decode_ins(IN_ID.INS);
        switch (ins.ins_type) {
            case JALR:case LB:case LH:case LW:case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:case ANDI:
            case SLLI:case SRLI:case SRAI: {
                ins.rv1=regi.Load_from_reg(ins.rs1);
                forwarding1_1(ins,IN_WB);
                break;
            }
            case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:case SB:case SH:case SW:case ADD:case SUB:case SLL:case SLT:case SLTU:
            case SRL:case SRA:case XOR:case OR:case AND: {
                ins.rv1=regi.Load_from_reg(ins.rs1);
                ins.rv2=regi.Load_from_reg(ins.rs2);
                forwarding1_2(ins,IN_WB);
                break;
            }
        }
        ID_EX_BUFFER.ins=ins,ID_EX_BUFFER.IA=IN_ID.IA;

        ID_EX_BUFFER.flag=true;
    }
    void exe()
    {
        if (IN_ID.flag && !BUBBLE) do_ID();
    }
};

class StageEX
{
public:
    ID_EX_BUFFER_CLASS &IN_EX;
    EX_MEM_BUFFER_CLASS &EX_MEM_BUFFER;
    StageEX(ID_EX_BUFFER_CLASS &_IN_EX,EX_MEM_BUFFER_CLASS &_EX_MEM_BUFFER):IN_EX(_IN_EX),EX_MEM_BUFFER(_EX_MEM_BUFFER){}
    void do_EX()
    {
        INSTRUCTION ins=IN_EX.ins;
        u32 IA=IN_EX.IA;
        u32 CR,TA;
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
    void exe()
    {
        if (IN_EX.flag && !BUBBLE) {
            do_EX();
            //std::cout<<clk<<" "<<EX_MEM_BUFFER.IA<<" "<<order_name[EX_MEM_BUFFER.ins.ins_type]<<std::endl;
        }
    }
};

class StageMEM
{
public:
    ID_EX_BUFFER_CLASS &IN_EX;
    EX_MEM_BUFFER_CLASS &IN_MEM;
    MEM_WB_BUFFER_CLASS &MEM_WB_BUFFER;
    Predictor &predictor;
    Memory &memory;
    StageMEM(ID_EX_BUFFER_CLASS &_IN_EX,EX_MEM_BUFFER_CLASS &_IN_MEM,MEM_WB_BUFFER_CLASS &_MEM_WB_BUFFER,Predictor &_predictor,Memory &_memory):IN_EX(_IN_EX),IN_MEM(_IN_MEM),MEM_WB_BUFFER(_MEM_WB_BUFFER),predictor(_predictor),memory(_memory){}
    void do_MEM()
    {
        INSTRUCTION ins=IN_MEM.ins;
        u32 VAL=IN_MEM.CR;
        u32 IA=IN_MEM.IA;
        u32 TA=IN_MEM.TA;
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
                predictor.update(is_succ,ins.is_branch,IA,TA);
                if (IN_EX.IA!=TA) {
                    NEED_TO_CUT=true;
                    MEM_WB_BUFFER.NEED_TO_CHANGE_TA=true;
                    should_not_do_IF=false;
                }
                break;
            }
            case LB:VAL=sext(memory.read_bit_from_mem(VAL),0,7);break;
            case LH:VAL=sext(memory.read_bit_from_mem(VAL)|memory.read_bit_from_mem(VAL+1)<<8,0,15);break;
            case LW:VAL=memory.read_bit_from_mem(VAL)|memory.read_bit_from_mem(VAL+1)<<8|memory.read_bit_from_mem(VAL+2)<<16|memory.read_bit_from_mem(VAL+3)<<24;break;
            case LBU:VAL=memory.read_bit_from_mem(VAL);break;
            case LHU:VAL=memory.read_bit_from_mem(VAL)|memory.read_bit_from_mem(VAL+1)<<8;break;
            case SB:memory.store_to_mem(VAL,get_bit(ins.rv2,0,7));break;
            case SH:{
                memory.store_to_mem(VAL,get_bit(ins.rv2,0,7));
                memory.store_to_mem(VAL+1,get_bit(ins.rv2,8,15));
                break;
            }
            case SW:{
                memory.store_to_mem(VAL,get_bit(ins.rv2,0,7));
                memory.store_to_mem(VAL+1,get_bit(ins.rv2,8,15));
                memory.store_to_mem(VAL+2,get_bit(ins.rv2,16,23));
                memory.store_to_mem(VAL+3,get_bit(ins.rv2,24,31));
                break;
            }
        }
        MEM_WB_BUFFER.ins=ins;
        MEM_WB_BUFFER.VAL=VAL;
        MEM_WB_BUFFER.IA=IA;
        MEM_WB_BUFFER.TA=TA;

        MEM_WB_BUFFER.flag=true;
    }
    void exe()
    {
        if (IN_MEM.flag) do_MEM();
    }

};

class StageWB
{
public:
    MEM_WB_BUFFER_CLASS &IN_WB;
    Register &regi;
    StageWB(MEM_WB_BUFFER_CLASS &_IN_WB,Register &_regi):IN_WB(_IN_WB),regi(_regi){}
    void do_WB()
    {
        INSTRUCTION ins=IN_WB.ins;
        u32 VAL=IN_WB.VAL;
        switch (ins.ins_type) {
            case LUI:case AUIPC:case JAL:case JALR:case LB:case LH:case LW:
            case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
            case ANDI:case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:
            case SLT:case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
            regi.Write_to_reg(ins.rd,VAL);break;
        }
    }
    void exe()
    {
        if (IN_WB.flag) do_WB();
    }
};

#endif