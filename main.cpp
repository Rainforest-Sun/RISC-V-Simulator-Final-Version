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