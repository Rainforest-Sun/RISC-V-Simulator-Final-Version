#ifndef CPU_HPP
#define CPU_HPP

#include <bits/stdc++.h>
#include "stage.hpp"
#include "Memory.hpp"
#include "Register.hpp"

    
class CPU {
public:
    Memory memory;
    Register regi;
    Predictor predictor;
    StageIF IF;
    StageID ID;
    StageEX EX;
    StageMEM MEM;
    StageWB WB;
    //typedef void(* Fuc)();
    //Fuc arr[] = {StageIF.IF,StageID.ID,StageEX.EX,StageMEM.MEM,StageWB.WB};
    CPU():memory(),regi(),predictor(),IF(memory,predictor,IF_ID_BUFFER),ID(IN_ID,ID_EX_BUFFER,regi),EX(IN_EX,EX_MEM_BUFFER),
    MEM(IN_MEM,MEM_WB_BUFFER,predictor,memory),WB(IN_WB,regi){}
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
    void run_order() {
        pc=0;
        //srand((int)time(NULL));
        while (true) {
            ++clk;
            if (clk==139) {
                int xx=123;
                --xx;
            }
            syn();
            //std::random_shuffle(arr,arr+4);
            //for (int i=0;i<=4;++i) arr[i]();
            MEM.exe();
            IF.exe();
            ID.exe();
            EX.exe();
            
            WB.exe();
            if (BUBBLE) {
                BUBBLE=false;
                LAST_CYCLE_IS_BUBBLE=true;
            }
            else LAST_CYCLE_IS_BUBBLE=false;
            regi.set_reg0();
            if (not_do_IF && ALL_BUFFER_IS_NULL()) break;
        }
    }   

};

#endif