#include <bits/stdc++.h>
#include "read.hpp"
#include "cpu.hpp"

int main()
{
    freopen("C:\\Users\\rainforest\\Desktop\\testcases\\array_test1.data", "r", stdin);
    freopen("C:\\Users\\rainforest\\Desktop\\out.txt", "w", stdout);
    read_order();
    run_order();
    printf("%u\n",reg[10]&255u);
    return 0;
}