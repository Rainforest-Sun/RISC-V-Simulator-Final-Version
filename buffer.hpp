#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <bits/stdc++.h>
#include "global.hpp"

struct IF_ID_BUFFER_CLASS {
    unsigned IA,INS;
    bool flag;
} IF_ID_BUFFER,IN_ID;

struct ID_EX_BUFFER_CLASS {
    INSTRUCTION ins;
    unsigned IA;
    bool flag;
} ID_EX_BUFFER,IN_EX;

struct EX_MEM_BUFFER_CLASS {
    INSTRUCTION ins;
    unsigned TA,CR,IA;
    bool flag;
} EX_MEM_BUFFER,IN_MEM;

struct MEM_WB_BUFFER_CLASS {
    INSTRUCTION ins;
    unsigned VAL,IA,TA;
    bool flag,NEED_TO_CHANGE_TA;
} MEM_WB_BUFFER,IN_WB;

#endif