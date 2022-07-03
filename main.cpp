#include <bits/stdc++.h>
#include "cpu.hpp"

int main()
{
    CPU cpu;
    cpu.run_order();
    printf("%u\n",cpu.regi.Load_from_reg(10)&255u);
    return 0;
}

/*
const char* file_name[18]= {
    "C:\\Users\\rainforest\\Desktop\\testcases\\array_test1.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\array_test2.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\basicopt1.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\bulgarian.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\expr.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\gcd.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\hanoi.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\lvalue2.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\magic.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\manyarguments.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\multiarray.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\naive.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\pi.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\qsort.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\queens.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\statement_test.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\superloop.data",
    "C:\\Users\\rainforest\\Desktop\\testcases\\tak.data"
};

int main()
{
    freopen("C:\\Users\\rainforest\\Desktop\\out.txt", "w", stdout);
    for (int i=5;i<6;++i) {
        FILE *p=freopen(file_name[i],"r",stdin);
        pc=0;
        clk=0;
        not_do_IF=false;
        should_not_do_IF=true;
        IF_ID_BUFFER.clear();
        IN_ID.clear();
        ID_EX_BUFFER.clear();
        IN_EX.clear();
        EX_MEM_BUFFER.clear();
        IN_MEM.clear();
        MEM_WB_BUFFER.clear();
        IN_WB.clear();
        BUBBLE=false;
        LAST_CYCLE_IS_BUBBLE=false;
        NEED_TO_CUT=false;
        CPU cpu;
        cpu.run_order();
        printf("%u %d \n",cpu.regi.Load_from_reg(10)&255u,clk);
        printf("%lf %d %d\n",(double)cpu.predictor.succ/cpu.predictor.tot,cpu.predictor.succ,cpu.predictor.tot);
        std::cin.clear();
        fclose(p);
    }
    return 0;
}
*/