#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <bits/stdc++.h>
#include "global.hpp"
using u8=unsigned char;
using u32=unsigned int;

class Register {

    u32 reg[32];//register
public:
    Register()
    { 
        memset(reg,0u,sizeof(reg));
    }
    u32 Load_from_reg(const u8 &index)
    {
        return reg[index];
    }
    void Write_to_reg(const u8 &rd,const u32 &val)
    {
        reg[rd]=val;
    }
    void set_reg0()
    {
        reg[0]=0;
    }
};

#endif //RISC-V Simulator REGISTER