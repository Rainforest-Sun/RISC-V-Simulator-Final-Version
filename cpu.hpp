#ifndef RUN_HPP
#define RUN_HPP

#include <bits/stdc++.h>
#include "stage.hpp"

bool ALL_BUFFER_IS_NULL() {
    return !IN_ID.flag&&!IN_EX.flag&&!IN_MEM.flag&&!IN_WB.flag;
}

void run_order() {
    pc=0;
    IF_ID_BUFFER.flag=IN_ID.flag=ID_EX_BUFFER.flag=IN_EX.flag=EX_MEM_BUFFER.flag=IN_MEM.flag=MEM_WB_BUFFER.flag=IN_WB.flag=false;
    while (true) {
        ++clk;
        syn();
        if (!not_do_IF && !BUBBLE) {
            if (!IF()) not_do_IF=true;
        }
        if (IN_ID.flag && !BUBBLE) ID();
        if (IN_EX.flag && !BUBBLE) EX();
        if (IN_MEM.flag) MEM();
        if (IN_WB.flag) WB();
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