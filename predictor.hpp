#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include <bits/stdc++.h>
#include "global.hpp"

class Predictor{
public:
    u32 tot,succ;
    u32 bht[256];//Branch History Table
    u8 pht[256][16];//Pattern History Table
    u32 btb[256];//Branch Target Buffer

    #define BRANCH_FEATURE (IA>>1 & 0xFFu)
    #define BHT_VALUE (bht[BRANCH_FEATURE])
    #define PHT_VALUE (pht[BRANCH_FEATURE][BHT_VALUE])
    #define BTB_VALUE (btb[BRANCH_FEATURE])

    Predictor()
    {
        tot=succ=0;
        memset(bht,0b00u,sizeof(bht));
        memset(pht,0b00u,sizeof(pht));
        memset(btb,0u,sizeof(btb));
    }

    void predictPC(u32 IA,u32 ins)
    {
        if (((ins & 0b1111111u)==0b1100011u) && (PHT_VALUE & 0b10u))
            pc=BTB_VALUE;
        else pc=IA+4;
    }

    void update(bool is_succ,bool is_taken,u32 IA,u32 TA)
    {
        ++tot;
        if (is_succ) ++succ;
        if (is_taken) {
            if (PHT_VALUE<0b11u) ++PHT_VALUE;
            BHT_VALUE = BHT_VALUE >> 1u | 0b1000u;
            BTB_VALUE = TA;
        }
        else {
            if (PHT_VALUE>0b00u) --PHT_VALUE;
            BHT_VALUE = BHT_VALUE >> 1u;
        }
    }
};
#endif