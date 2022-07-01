#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include <bits/stdc++.h>
#include "global.hpp"

unsigned tot,succ;
unsigned bht[4096],btb[256];//Branch History Table & Branch Target Buffer

void init_predictor()
{
    memset(bht,0b01u,sizeof(bht));
    memset(btb,0u,sizeof(btb));
}

void predictPC(unsigned IA,unsigned ins)
{
    if (((ins & 0b1111111u)==0b1100011u) && (bht[IA & 0xfffu] & 0b10u))
        pc=btb[IA & 0xffu];
    else pc=IA+4;
}

void update(bool is_succ,bool is_taken,unsigned IA,unsigned TA)
{
    ++tot;
    if (is_succ) ++succ;
    if (is_taken) {
        if (bht[IA & 0xfffu]<0b11u) ++bht[IA & 0xfffu];
        btb[IA & 0xffu]=TA;
    }
    else {
        if (bht[IA & 0xfffu]>0b00u) --bht[IA & 0xfffu];
    }
}

#endif