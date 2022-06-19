#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

bool getBit(char byte,int pos){
    return ((byte >> pos) & 1);
}

void setBit(char *byte,int pos,bool value){
    if(value){
        *byte = *byte | ((char)value << pos);
    }else{
        u_char tmp = 0b11111111;
        tmp -= ((u_char)1 << pos);
        *byte = *byte & tmp;
    }
}

/*
    Add bytes with carry indicating overflow
*/
char addBytes(char a,char b,bool withCarry,char *flags){
    char carry = 0;
    if(withCarry){
        carry = (*flags) & 1; // carry flag
    }

    //check for carry
    int res = (int)(u_char)a + (int)(u_char)b + carry;
    bool carryFlag = res >> 8;
    setBit(flags,0,carryFlag);

    //check for overflow
    res = (int)a + (int)b + carry;
    bool overflow = res > 127 ? 1 : (res < -128 ? 1 : 0);
    setBit(flags,6,overflow);

    //real calculation
    char resChar = a + b + carry;

    //check for 0
    setBit(flags,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(flags,7,resChar < 0 ? 1 : 0);

    return resChar;
}


enum ADDR_MODE{
    ACCUMULATOR,
    ABSOLUTE,
    ABSOLUTEX,
    ABSOLUTEY,
    IMMEDIATE,
    IMPLIED,
    INDIRECT,
    INDIRECTX,
    INDIRECTY,
    RELATIVE,
    ZEROPAGE,
    ZEROPAGEX,
    ZEROPAGEY
};




//CPU
class CPU{
    const int masterClock = 21.477272;//MHz

    u_short PC; //Program Counter - 16 bits
    char AC; //Accumulator - 8 bits
    char X; //X register - 8 bits
    char Y; //Y register - 8 bits
    char SR; //status regiter - 8 bits
    char SP; //stack pointer - 8 bits 

    char *memory;

    CPU(){
        // PC = memory[0xfffc];
    };
    ~CPU(){};

    char get_oper(int addr_mode){
        char oper;
        u_char LL;
        u_char HH;
        u_char LL2;
        u_char HH2;
        switch (addr_mode){
            case ABSOLUTE:
                LL = (u_char)memory[PC++];
                HH = (u_char)memory[PC++];
                oper = memory[((u_short)HH << 8) + (u_short)LL];
                break;
            case ABSOLUTEX:
                LL = (u_char)memory[PC++];
                HH = (u_char)memory[PC++];
                oper = memory[((u_short)HH << 8) + (u_short)LL + (u_char)X]; //with carry
                break;
            case ABSOLUTEY:
                LL = (u_char)memory[PC++];
                HH = (u_char)memory[PC++];
                oper = memory[((u_short)HH << 8) + (u_short)LL + (u_char)Y]; //with carry
                break;
            case IMMEDIATE:
                oper = memory[PC++];
                break;
            case INDIRECT:
                LL = (u_char)memory[PC++];
                HH = (u_char)memory[PC++];
                LL2 = memory[((u_short)HH << 8) + (u_short)LL];
                HH2 = memory[((u_short)HH << 8) + (u_short)(u_char)(LL + (u_char)1)]; //without carry(although its not supposed to happen anyway)
                oper = memory[((u_short)HH2 << 8) + (u_short)LL2];
                break;
            case INDIRECTX:
                LL = (u_char)memory[PC++];
                LL2 = memory[(u_short)(u_char)(LL + (u_char)X)]; //without carry
                HH2 = memory[(u_short)(u_char)(LL + (u_char)X + (u_char)1)]; //without carry
                oper = memory[((u_short)HH2 << 8) + (u_short)LL2];
                break;
            case INDIRECTY:
                LL = (u_char)memory[PC++];
                LL2 = memory[(u_short)LL];
                HH2 = memory[(u_short)(u_char)(LL + (u_char)1)];
                oper = memory[((u_short)HH2 << 8) + (u_short)LL2 + (u_char)Y]; // with carry
                break;
            case ZEROPAGE:
                LL = (u_char)memory[PC++];
                oper = memory[(u_short)LL];
                break;
            case ZEROPAGEX:
                LL = (u_char)memory[PC++];
                oper = memory[(u_short)(u_char)(LL + (u_char)X)];
                break;
            case ZEROPAGEY:
                LL = (u_char)memory[PC++];
                oper = memory[(u_short)(u_char)(LL + (u_char)Y)];
                break;
            case RELATIVE:
                oper = memory[PC++];
                break;
            default:
                oper = 0;
                break;
        }
        return oper;
    };

    void nextIntruction(){
        int opc = memory[PC++]; // get operation code

        char oper;

        switch (opc) {
            case 0x0:
                /* code */
                break;

            //ADC Instructions
            case 0x69:
                oper = get_oper(IMMEDIATE);
                AC = addBytes(AC,oper,1,&SR);
                break;
            case 0x65:
                AC = addBytes(AC,oper,1,&SR);
                break;
            case 0x75:
                AC = addBytes(AC,oper,1,&SR);
                break;
            case 0x6D:
                
                AC = addBytes(AC,oper,1,&SR);
                break;
            case 0x7D:
                AC = addBytes(AC,oper,1,&SR);
                break;
            case 0x79:
                AC = addBytes(AC,oper,1,&SR);
                break;
            default:
                break;
        }

    };
};
