#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <bits/stdc++.h>
#include "global.hpp"
using u8=unsigned char;
using u32=unsigned int;

class Memory {
public:
    u8 mem[500005];
    unsigned hex_to_dec(const std::string &s) {
        unsigned res=0;
        for (int i=0;i<s.length();++i) {
            if (s[i]>='0' && s[i]<='9') res=(res<<4)+s[i]-'0';
            else res=(res<<4)+s[i]-'A'+10;
        }
        return res;
    }
public:
    Memory()
    {
        std::string s;
        int pos=0;
        while (getline(std::cin,s)) {
            if (s[0]=='@') pos=hex_to_dec(s.substr(1,8));
            else 
                for (int i=0;i<s.length();i+=3) mem[pos++]=hex_to_dec(s.substr(i,2));
        }
    }
    u32 read_bit_from_mem(const u32 &pos)
    {
        return (u32)mem[pos];
    }

    void store_to_mem(const u32 &pos,const u32 &val)
    {
        mem[pos]=val & 0b11111111u;
    }

    u32 get_ins()
    {
        return ((u32)mem[pc]|(u32)mem[pc+1]<<8|(u32)mem[pc+2]<<16|(u32)mem[pc+3]<<24);
    }
};

#endif //RISC-V Simulator MEMORY