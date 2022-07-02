#include <bits/stdc++.h>
#include "read.hpp"
#include "cpu.hpp"

int main()
{
    read_order();
    run_order();
    printf("%u\n",reg[10]&255u);
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
    int all_succ=0,all_tot=0;
    freopen("C:\\Users\\rainforest\\Desktop\\out.txt", "w", stdout);
    for (int i=0;i<18;++i) {
        FILE *p=freopen(file_name[i],"r",stdin);
        //std::cout<<file_name[i]<<std::endl;
        memset(reg,0u,sizeof(reg));
        memset(mem,0u,sizeof(mem));
        pc=0;
        clk=0;
        succ=0;
        tot=0;
        not_do_IF=false;
        should_not_do_IF=true;
        read_order();
        run_order();
        printf("%u %d ",reg[10]&255u,clk);
        printf("%lf %d %d\n",(double)succ/tot,succ,tot);
        all_succ+=succ,all_tot+=tot;
        std::cin.clear();
        fclose(p);
    }
    printf("ALL DATA:\n%lf %d %d\n",(double)all_succ/all_tot,all_succ,all_tot);
    return 0;
}
*/