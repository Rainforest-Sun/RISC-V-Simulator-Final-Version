#ifndef READ_HPP
#define READ_HPP

#include <bits/stdc++.h>
#include "global.hpp"

unsigned hex_to_dec(const std::string &s) {
    unsigned res=0;
    for (int i=0;i<s.length();++i) {
        if (s[i]>='0' && s[i]<='9') res=(res<<4)+s[i]-'0';
        else res=(res<<4)+s[i]-'A'+10;
    }
    return res;
}

void read_order() {
    std::string s;
    int pos=0;
    while (getline(std::cin,s)) {
        if (s[0]=='@') pos=hex_to_dec(s.substr(1,8));
        else 
            for (int i=0;i<s.length();i+=3) mem[pos++]=hex_to_dec(s.substr(i,2));
    }
}

#endif