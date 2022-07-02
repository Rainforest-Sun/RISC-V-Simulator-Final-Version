#ifndef CPU_HPP
#define CPU_HPP

#include <bits/stdc++.h>
#include "stage.hpp"

bool ALL_BUFFER_IS_NULL() {
    return !IN_ID.flag&&!IN_EX.flag&&!IN_MEM.flag&&!IN_WB.flag;
}

typedef void(* Fuc)();
Fuc arr[] = {IF,ID,EX,MEM,WB};

void run_order() {
    pc=0;
    init_predictor();
    IF_ID_BUFFER.flag=IN_ID.flag=ID_EX_BUFFER.flag=IN_EX.flag=EX_MEM_BUFFER.flag=IN_MEM.flag=MEM_WB_BUFFER.flag=MEM_WB_BUFFER.NEED_TO_CHANGE_TA=IN_WB.flag=false;
    while (true) {
        ++clk;
        syn();
        std::random_shuffle(arr,arr+4);
        for (int i=0;i<=4;++i) arr[i]();
        if (BUBBLE) {
            BUBBLE=false;
            LAST_CYCLE_IS_BUBBLE=true;
        }
        else LAST_CYCLE_IS_BUBBLE=false;
        reg[0]=0;
        if (not_do_IF && ALL_BUFFER_IS_NULL()) break;
    }
}

#endif