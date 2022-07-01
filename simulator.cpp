#include <bits/stdc++.h>
using namespace std;

int clk=0;
unsigned pc;
unsigned reg[32];//register
unsigned mem[500005];//memory ,note that though type of mem is unsigned, only low 8 bits legal
const unsigned Full=-1;//all 1 with length of 32
bool not_do_IF=false;
bool BUBBLE=false;
bool LAST_CYCLE_IS_BUBBLE=false;

enum Ins_type {
    LB,LH,LW,LBU,LHU,
    SB,SH,SW,
    ADD,ADDI,SUB,LUI,AUIPC,XOR,XORI,OR,ORI,AND,ANDI,
    SLL,SLLI,SRL,SRLI,SRA,SRAI,SLT,SLTI,SLTU,SLTIU,
    BEQ,BNE,BLT,BGE,BLTU,BGEU,
    JAL,JALR
};

struct INSTRUCTION {
   Ins_type ins_type;
   unsigned rs1,rs2,rd,imm,offset,shamt,rv1,rv2;
   bool is_branch;
};

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
    unsigned VAL;
    unsigned IA;
    bool flag;
} MEM_WB_BUFFER,IN_WB;

inline unsigned sext(const unsigned &x,const unsigned &l,const unsigned &r)//signed-extend
{
    bool sign=((unsigned)1<<r)&x;
    if (sign) return (Full<<(r+1))|x;
    else return x;
}

inline unsigned get_bit(const unsigned &x,const unsigned &l,const unsigned &r)//get x from l to r
{
    return (((Full>>(31-r+l))<<l)&x)>>l;
}

void lui(const unsigned &imm,unsigned &val)
{
    val=sext(imm,0,19)<<12;
}

void auipc(const unsigned &imm,const unsigned &IA,unsigned &val)
{
    val=IA+(sext(imm,0,19)<<12);
}

void jal(const unsigned &offset,const unsigned &IA,unsigned &val,unsigned &TA)
{
    val=IA+4;
    TA=IA+sext(offset,0,20);
}

void jalr(const unsigned &offset,const unsigned &rv1,const unsigned &IA,unsigned &val,unsigned &TA)
{
    val=IA+4;
    TA=(rv1+sext(offset,0,11))&(~1);
}

bool beq(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
{
    if (rv1==rv2) {
        TA=IA+sext(offset,0,12);
        return true;
    }
    else return false;
}

bool bne(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
{
    if (rv1!=rv2) {
        TA=IA+sext(offset,0,12);
        return true;
    }
    else return false;
}

bool blt(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
{
    if ((int)rv1<(int)rv2) {
        TA=IA+sext(offset,0,12);//note should convert unsigned to int
        return true;
    }
    else return false;
}

bool bge(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
{
    if ((int)rv1>=(int)rv2) {
        TA=IA+sext(offset,0,12);//note should convert unsigned to int
        return true;
    }
    else return false;
}

bool bltu(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
{
    if (rv1<rv2) {
        TA=IA+sext(offset,0,12);
        return true;
    }
    else return false;
}

bool bgeu(const unsigned &rv1,const unsigned &rv2,const unsigned &offset,const unsigned &IA,unsigned &TA)
{
    if (rv1>=rv2) {
        TA=IA+sext(offset,0,12);
        return true;
    }
    else return false;
}

void lb(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lh(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lw(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lbu(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void lhu(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void sb(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}
    
void sh(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void sw(const unsigned &imm,const unsigned &rv1,unsigned &pos)
{
    pos=rv1+sext(imm,0,11);
}

void addi(const unsigned &imm,const unsigned &rv1,unsigned &val)
{
    val=rv1+sext(imm,0,11);
}

void slti(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=(((int)rv1<(int)sext(imm,0,11))?1:0);
}

void sltiu(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=((rv1<sext(imm,0,11))?1:0);
}

void xori(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=rv1^sext(imm,0,11);
}

void ori(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=rv1|sext(imm,0,11);
}

void andi(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=rv1&sext(imm,0,11);
}

void slli(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=rv1<<imm;
}

void srli(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    val=rv1>>imm;
}

void srai(const unsigned &rv1,const unsigned &imm,unsigned &val)
{
    unsigned move_bit=imm;
    val=sext(rv1>>move_bit,0,31-move_bit);
}

void add(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1+rv2;
}

void sub(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1-rv2;
}

void sll(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1<<get_bit(rv2,0,4);
}

void slt(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=(((int)rv1<(int)rv2)?1:0);
}

void sltu(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=((rv1<rv2)?1:0);
}

void xor_(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1^rv2;
}

void srl(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1>>get_bit(rv2,0,4);
}

void sra(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    unsigned move_bit=get_bit(rv2,0,4);
    val=sext(rv1>>move_bit,0,31-move_bit);
}

void or_(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1|rv2;
}

void and_(const unsigned &rv1,const unsigned &rv2,unsigned &val)
{
    val=rv1&rv2;
}

INSTRUCTION decode_ins(const unsigned &s)
{
    INSTRUCTION res;
    unsigned opcode=get_bit(s,0,6);//get 0-6 bits
    if (opcode==0b0110111) {
        unsigned rd=get_bit(s,7,11),imm=get_bit(s,12,31);
        res.ins_type=LUI,res.rd=rd,res.imm=imm;
    }
    else if (opcode==0b0010111) {
        unsigned rd=get_bit(s,7,11),imm=get_bit(s,12,31);
        res.ins_type=AUIPC,res.rd=rd,res.imm=imm;
    }
    else if (opcode==0b1101111) {
        unsigned rd=get_bit(s,7,11);
        unsigned offset=get_bit(s,21,30)<<1|get_bit(s,20,20)<<11|get_bit(s,12,19)<<12|get_bit(s,31,31)<<20;
        res.ins_type=JAL,res.rd=rd,res.offset=offset;
    }
    else if (opcode==0b1100111) {
        unsigned rd=get_bit(s,7,11),offset=get_bit(s,20,31),rs1=get_bit(s,15,19);
        res.ins_type=JALR,res.rd=rd,res.offset=offset,res.rs1=rs1;
    }
    else if (opcode==0b1100011) {
        unsigned offset=get_bit(s,8,11)<<1|get_bit(s,25,30)<<5|get_bit(s,7,7)<<11|get_bit(s,31,31)<<12;
        unsigned rs1=get_bit(s,15,19),rs2=get_bit(s,20,24);
        unsigned func3=get_bit(s,12,14);
        res.offset=offset,res.rs1=rs1,res.rs2=rs2;
        if (func3==0b000) res.ins_type=BEQ;
        else if (func3==0b001) res.ins_type=BNE;
        else if (func3==0b100) res.ins_type=BLT;
        else if (func3==0b101) res.ins_type=BGE;
        else if (func3==0b110) res.ins_type=BLTU;
        else if (func3==0b111) res.ins_type=BGEU;
    }
    else if (opcode==0b0000011) {
        unsigned imm=get_bit(s,20,31),rs1=get_bit(s,15,19),rd=get_bit(s,7,11);
        unsigned func3=get_bit(s,12,14);
        res.imm=imm,res.rs1=rs1,res.rd=rd;
        if (func3==0b000) res.ins_type=LB;
        else if (func3==0b001) res.ins_type=LH;
        else if (func3==0b010) res.ins_type=LW;
        else if (func3==0b100) res.ins_type=LBU;
        else if (func3==0b101) res.ins_type=LHU;
    }
    else if (opcode==0b0100011) {
        unsigned imm=get_bit(s,7,11)|get_bit(s,25,31)<<5;
        unsigned rs1=get_bit(s,15,19),rs2=get_bit(s,20,24);
        unsigned func3=get_bit(s,12,14);
        res.imm=imm,res.rs1=rs1,res.rs2=rs2;
        if (func3==0b000) res.ins_type=SB;
        else if (func3==0b001) res.ins_type=SH;
        else if (func3==0b010) res.ins_type=SW;
    }
    else if (opcode==0b0010011) {
        unsigned imm=get_bit(s,20,31),shamt=get_bit(s,20,25),rs1=get_bit(s,15,19),rd=get_bit(s,7,11);
        unsigned func3=get_bit(s,12,14),func7=get_bit(s,26,31);
        res.rd=rd,res.rs1=rs1;
        if (func3==0b000) res.ins_type=ADDI,res.imm=imm;
        else if (func3==0b010) res.ins_type=SLTI,res.imm=imm;
        else if (func3==0b011) res.ins_type=SLTIU,res.imm=imm;
        else if (func3==0b100) res.ins_type=XORI,res.imm=imm;
        else if (func3==0b110) res.ins_type=ORI,res.imm=imm;
        else if (func3==0b111) res.ins_type=ANDI,res.imm=imm;
        else if (func3==0b001) res.ins_type=SLLI,res.shamt=shamt;
        else if (func3==0b101 && func7==0b000000) res.ins_type=SRLI,res.shamt=shamt;
        else if (func3==0b101 && func7==0b010000) res.ins_type=SRAI,res.shamt=shamt;
    }
    else if (opcode==0b0110011) {
        unsigned rd=get_bit(s,7,11),rs1=get_bit(s,15,19),rs2=get_bit(s,20,24);
        unsigned func3=get_bit(s,12,14),func7=get_bit(s,25,31);
        res.rd=rd,res.rs1=rs1,res.rs2=rs2;
        if (func3==0b000 && func7==0b0000000) res.ins_type=ADD;
        else if (func3==0b000 && func7==0b0100000) res.ins_type=SUB;
        else if (func3==0b001) res.ins_type=SLL;
        else if (func3==0b010) res.ins_type=SLT;
        else if (func3==0b011) res.ins_type=SLTU;
        else if (func3==0b100) res.ins_type=XOR;
        else if (func3==0b101 && func7==0b0000000) res.ins_type=SRL;
        else if (func3==0b101 && func7==0b0100000) res.ins_type=SRA;
        else if (func3==0b110) res.ins_type=OR;
        else if (func3==0b111) res.ins_type=AND;
    }
    return res;
}

unsigned hex_to_dec(const string &s) {
    unsigned res=0;
    for (int i=0;i<s.length();++i) {
        if (s[i]>='0' && s[i]<='9') res=(res<<4)+s[i]-'0';
        else res=(res<<4)+s[i]-'A'+10;
    }
    return res;
}

void read_order() {
    string s;
    int pos=0;
    while (getline(cin,s)) {
        if (s[0]=='@') pos=hex_to_dec(s.substr(1,8));
        else 
            for (int i=0;i<s.length();i+=3) mem[pos++]=hex_to_dec(s.substr(i,2));
    }
}

inline unsigned get_ins()
{
    return (mem[pc]|mem[pc+1]<<8|mem[pc+2]<<16|mem[pc+3]<<24);
}

inline unsigned get_pc()
{
    return pc;
}

void change_pc(const unsigned &TA)
{
    pc=TA;
}

inline unsigned read_bit_from_mem(const unsigned &pos)
{
    return mem[pos];
}

inline void store_to_mem(const unsigned &pos,const unsigned &val)
{
    mem[pos]=val;
}

inline void store_to_reg(const unsigned &rd,const unsigned &val)
{
    reg[rd]=val;
}

inline void del_ins_now()
{
    IF_ID_BUFFER.flag=IN_ID.flag=ID_EX_BUFFER.flag=IN_EX.flag=EX_MEM_BUFFER.flag=IN_MEM.flag=false;
}

bool IF()
{
    unsigned s=get_ins();
    if (s==(int)0x0ff00513) return false;
    unsigned IA=get_pc();
    //记得把二位饱和预测加到这里！！！！！！！！！！！
    pc+=4;
    IF_ID_BUFFER.flag=true;
    IF_ID_BUFFER.INS=s,IF_ID_BUFFER.IA=IA;
    return true;
}

void forwarding1_1(INSTRUCTION &ins)
{
    if (IN_WB.flag) {
        switch (IN_WB.ins.ins_type) {
            case LUI:case AUIPC:case JAL:case JALR:case LB:case LH:case LW:
            case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
            case ANDI:case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:
            case SLT:case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
            if (IN_WB.ins.rd!=0 && IN_WB.ins.rd==ins.rs1) ins.rv1=IN_WB.VAL;
        }
    }
}

void forwarding1_2(INSTRUCTION &ins)
{
    if (IN_WB.flag) {
        switch (IN_WB.ins.ins_type) {
            case LUI:case AUIPC:case JAL:case JALR:case LB:case LH:case LW:
            case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
            case ANDI:case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:
            case SLT:case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
            if (IN_WB.ins.rd!=0 && IN_WB.ins.rd==ins.rs1) ins.rv1=IN_WB.VAL;
            if (IN_WB.ins.rd!=0 && IN_WB.ins.rd==ins.rs2) ins.rv2=IN_WB.VAL;
        }
    }
}

void forwarding2_1(INSTRUCTION &ins)
{
    if (IN_MEM.flag) {
        switch (IN_MEM.ins.ins_type) {
            //LB之类的办不了
            case LUI:case AUIPC:case JAL:case JALR:
            case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
            case ANDI:case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:
            case SLT:case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
            if (IN_MEM.ins.rd!=0 && IN_MEM.ins.rd==ins.rs1) ins.rv1=IN_MEM.CR;
        }
    }
}

void forwarding2_2(INSTRUCTION &ins)
{
    if (IN_MEM.flag) {
        switch (IN_MEM.ins.ins_type) {
            //LB之类的办不了
            case LUI:case AUIPC:case JAL:case JALR:
            case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
            case ANDI:case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:
            case SLT:case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
            if (IN_MEM.ins.rd!=0 && IN_MEM.ins.rd==ins.rs1) ins.rv1=IN_MEM.CR;
            if (IN_MEM.ins.rd!=0 && IN_MEM.ins.rd==ins.rs2) ins.rv2=IN_MEM.CR;
        }
    }
}

void forwarding(INSTRUCTION &ins)
{
    switch (ins.ins_type) {
        case JALR:case LB:case LH:case LW:case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:case ANDI:
        case SLLI:case SRLI:case SRAI: {
            forwarding1_1(ins);
            break;
        }
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:case SB:case SH:case SW:case ADD:case SUB:case SLL:case SLT:case SLTU:
        case SRL:case SRA:case XOR:case OR:case AND: {
            forwarding1_2(ins);
            break;
        }
    }
    switch (ins.ins_type) {
        case JALR:case LB:case LH:case LW:case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:case ANDI:
        case SLLI:case SRLI:case SRAI: {
            forwarding2_1(ins);
            break;
        }
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:case SB:case SH:case SW:case ADD:case SUB:case SLL:case SLT:case SLTU:
        case SRL:case SRA:case XOR:case OR:case AND: {
            forwarding2_2(ins);
            break;
        }
    }
}

void ID()
{
    INSTRUCTION ins=decode_ins(IN_ID.INS);
    switch (ins.ins_type) {
        case JALR:case LB:case LH:case LW:case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:case ANDI:
        case SLLI:case SRLI:case SRAI: {
            ins.rv1=reg[ins.rs1];
            forwarding1_1(ins);
            break;
        }
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:case SB:case SH:case SW:case ADD:case SUB:case SLL:case SLT:case SLTU:
        case SRL:case SRA:case XOR:case OR:case AND: {
            ins.rv1=reg[ins.rs1];
            ins.rv2=reg[ins.rs2];
            forwarding1_2(ins);
            break;
        }
    }
    ID_EX_BUFFER.ins=ins,ID_EX_BUFFER.IA=IN_ID.IA;

    ID_EX_BUFFER.flag=true;
}

void EX()
{
    INSTRUCTION ins=IN_EX.ins;
    unsigned IA=IN_EX.IA;
    unsigned CR,TA;
    switch (ins.ins_type) {
        case LUI:lui(ins.imm,CR);break;
        case AUIPC:auipc(ins.imm,IA,CR);break;
        case JAL:jal(ins.offset,IA,CR,TA);break;
        case JALR:jalr(ins.offset,ins.rv1,IA,CR,TA);break;
        case BEQ:{
            if (!beq(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case BNE:{
            if (!bne(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case BLT:{
            if (!blt(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case BGE:{
            if (!bge(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case BLTU:{
            if (!bltu(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case BGEU:{
            if (!bgeu(ins.rv1,ins.rv2,ins.offset,IA,TA)) ins.is_branch=false;
            else ins.is_branch=true;
            break;
        }
        case LB:lb(ins.imm,ins.rv1,CR);break;
        case LH:lh(ins.imm,ins.rv1,CR);break;
        case LW:lw(ins.imm,ins.rv1,CR);break;
        case LBU:lbu(ins.imm,ins.rv1,CR);break;
        case LHU:lhu(ins.imm,ins.rv1,CR);break;
        case SB:sb(ins.imm,ins.rv1,CR);break;
        case SH:sh(ins.imm,ins.rv1,CR);break;
        case SW:sw(ins.imm,ins.rv1,CR);break;
        case ADDI:addi(ins.imm,ins.rv1,CR);break;
        case SLTI:slti(ins.rv1,ins.imm,CR);break;
        case SLTIU:sltiu(ins.rv1,ins.imm,CR);break;
        case XORI:xori(ins.rv1,ins.imm,CR);break;
        case ORI:ori(ins.rv1,ins.imm,CR);break;
        case ANDI:andi(ins.rv1,ins.imm,CR);break;
        case SLLI:slli(ins.rv1,ins.shamt,CR);break;
        case SRLI:srli(ins.rv1,ins.shamt,CR);break;
        case SRAI:srai(ins.rv1,ins.shamt,CR);break;
        case ADD:add(ins.rv1,ins.rv2,CR);break;
        case SUB:sub(ins.rv1,ins.rv2,CR);break;
        case SLL:sll(ins.rv1,ins.rv2,CR);break;
        case SLT:slt(ins.rv1,ins.rv2,CR);break;
        case SLTU:sltu(ins.rv1,ins.rv2,CR);break;
        case XOR:xor_(ins.rv1,ins.rv2,CR);break;
        case SRL:srl(ins.rv1,ins.rv2,CR);break;
        case SRA:sra(ins.rv1,ins.rv2,CR);break;
        case OR:or_(ins.rv1,ins.rv2,CR);break;
        case AND:and_(ins.rv1,ins.rv2,CR);break;
    }
    EX_MEM_BUFFER.CR=CR;
    EX_MEM_BUFFER.TA=TA;
    EX_MEM_BUFFER.ins=ins;
    EX_MEM_BUFFER.IA=IA;

    EX_MEM_BUFFER.flag=true;
}

void MEM()
{
    INSTRUCTION ins=IN_MEM.ins;
    unsigned VAL=IN_MEM.CR;
    unsigned IA=IN_MEM.IA;
    unsigned TA=IN_MEM.TA;
    switch (ins.ins_type) {
        case JAL:case JALR:{
            change_pc(TA);
            if (IN_EX.IA!=pc) {
                del_ins_now();
                not_do_IF=false;
            }
            break;
        }
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:{
            if (ins.is_branch) {
                change_pc(TA);
                if (IN_EX.IA!=pc) {
                    del_ins_now();
                    not_do_IF=false;
                }
            }
            break;
        }
        case LB:VAL=sext(read_bit_from_mem(VAL),0,7);break;
        case LH:VAL=sext(read_bit_from_mem(VAL)|read_bit_from_mem(VAL+1)<<8,0,15);break;
        case LW:VAL=read_bit_from_mem(VAL)|read_bit_from_mem(VAL+1)<<8|read_bit_from_mem(VAL+2)<<16|read_bit_from_mem(VAL+3)<<24;break;
        case LBU:VAL=read_bit_from_mem(VAL);break;
        case LHU:VAL=read_bit_from_mem(VAL)|read_bit_from_mem(VAL+1)<<8;break;
        case SB:store_to_mem(VAL,get_bit(ins.rv2,0,7));break;
        case SH:{
            store_to_mem(VAL,get_bit(ins.rv2,0,7));
            store_to_mem(VAL+1,get_bit(ins.rv2,8,15));
            break;
        }
        case SW:{
            store_to_mem(VAL,get_bit(ins.rv2,0,7));
            store_to_mem(VAL+1,get_bit(ins.rv2,8,15));
            store_to_mem(VAL+2,get_bit(ins.rv2,16,23));
            store_to_mem(VAL+3,get_bit(ins.rv2,24,31));
            break;
        }
    }
    MEM_WB_BUFFER.ins=ins;
    MEM_WB_BUFFER.VAL=VAL;
    MEM_WB_BUFFER.IA=IA;

    MEM_WB_BUFFER.flag=true;
}

void WB()
{
    INSTRUCTION ins=IN_WB.ins;
    unsigned VAL=IN_WB.VAL;
    switch (ins.ins_type) {
        case LUI:case AUIPC:case JAL:case JALR:case LB:case LH:case LW:
        case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
        case ANDI:case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:
        case SLT:case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
        store_to_reg(ins.rd,VAL);break;
    }
}

void bubble()
{
    if (!LAST_CYCLE_IS_BUBBLE) BUBBLE=true;
}

void syn()
{
    IN_ID=IF_ID_BUFFER;
    IN_EX=ID_EX_BUFFER;
    IN_MEM=EX_MEM_BUFFER;
    IN_WB=MEM_WB_BUFFER;
    if (not_do_IF) IF_ID_BUFFER.flag=false;
    if (!IN_ID.flag) ID_EX_BUFFER.flag=false;
    if (!IN_EX.flag) EX_MEM_BUFFER.flag=false;
    if (!IN_MEM.flag) MEM_WB_BUFFER.flag=false;
    if (IN_EX.flag && IN_MEM.flag) {
        switch (IN_MEM.ins.ins_type) {
            case LB:case LH:case LW:case LBU:case LHU:{
                switch (IN_EX.ins.ins_type) {
                    case JALR:case LB:case LH:case LW:case LBU:case LHU:case ADDI:case SLTI:case SLTIU:case XORI:case ORI:case ANDI:
                    case SLLI:case SRLI:case SRAI: {
                        if (IN_MEM.ins.rd==IN_EX.ins.rs1) bubble();
                        break;
                    }
                    case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:case SB:case SH:case SW:case ADD:case SUB:case SLL:case SLT:case SLTU:
                    case SRL:case SRA:case XOR:case OR:case AND: {
                        if (IN_MEM.ins.rd==IN_EX.ins.rs1 || IN_MEM.ins.rd==IN_EX.ins.rs2) bubble();
                        break;
                    }  
                }  
            }
        }
    }
    forwarding(ID_EX_BUFFER.ins);
    forwarding(IN_EX.ins);
}

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

int main()
{
    read_order();
    run_order();
    printf("%u\n",reg[10]&255u);
    return 0;
}