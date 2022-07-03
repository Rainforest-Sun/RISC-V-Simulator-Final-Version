#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <bits/stdc++.h>
#include "global.hpp"

struct IF_ID_BUFFER_CLASS {
    u32 IA,INS;
    bool flag;
    IF_ID_BUFFER_CLASS():IA(0),INS(0),flag(false){}
    void clear()
    {
        IA=INS=0,flag=false;
    }
};

struct ID_EX_BUFFER_CLASS {
    INSTRUCTION ins;
    u32 IA;
    bool flag;
    ID_EX_BUFFER_CLASS():ins(),IA(0),flag(false){}
    void clear()
    {
        ins.clear();
        IA=0,flag=false;
    }
};

struct EX_MEM_BUFFER_CLASS {
    INSTRUCTION ins;
    u32 TA,CR,IA;
    bool flag;
    EX_MEM_BUFFER_CLASS():ins(),TA(0),CR(0),IA(0),flag(false){}
    void clear()
    {
        ins.clear();
        TA=CR=IA=0;
        flag=false;
    }
};

struct MEM_WB_BUFFER_CLASS {
    INSTRUCTION ins;
    u32 VAL,IA,TA;
    bool flag,NEED_TO_CHANGE_TA;
    MEM_WB_BUFFER_CLASS():ins(),VAL(0),IA(0),TA(0),flag(false),NEED_TO_CHANGE_TA(false){}
    void clear()
    {
        ins.clear();
        VAL=IA=TA=0;
        flag=NEED_TO_CHANGE_TA=false;
    }
};

#endif //RISC-V Simulator BUFFER