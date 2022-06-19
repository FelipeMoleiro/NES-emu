#include <iostream>
#include <string>
#include <string.h>
#include "cpu.h"

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



void CPU::nextIntruction(){
    int opc = memory[PC++]; // get operation code

    char oper;

    switch (opc) {
        case 0x0:
            /* code */
            break;

        //ADC Instructions
        case 0x69:
            oper = get_oper(IMMEDIATE);
            AC = addBytes(AC,oper,1);
            break;
        case 0x65:
            oper = get_oper(ZEROPAGE);
            AC = addBytes(AC,oper,1);
            break;
        case 0x75:
            oper = get_oper(ZEROPAGEX);
            AC = addBytes(AC,oper,1);
            break;
        case 0x6D:
            oper = get_oper(ABSOLUTE);
            AC = addBytes(AC,oper,1);
            break;
        case 0x7D:
            oper = get_oper(ABSOLUTEX);
            AC = addBytes(AC,oper,1);
            break;
        case 0x79:
            oper = get_oper(ABSOLUTEY);
            AC = addBytes(AC,oper,1);
            break;
        case 0x61:
            oper = get_oper(INDIRECTX);
            AC = addBytes(AC,oper,1);
            break;
        case 0x71:
            oper = get_oper(INDIRECTY);
            AC = addBytes(AC,oper,1);
            break;
        //AND INSTRUCTIONS
        default:
            break;
    }

}

char CPU::get_oper(int addr_mode){
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
}

/*
Add bytes with carry indicating overflow
*/
char CPU::addBytes(char a,char b,bool withCarry){
    char carry = 0;
    if(withCarry){
        carry = SR & 1; // carry flag
    }

    //check for carry
    int res = (int)(u_char)a + (int)(u_char)b + carry;
    bool carryFlag = res >> 8;
    setBit(&SR,0,carryFlag);

    //check for overflow
    res = (int)a + (int)b + carry;
    bool overflow = res > 127 ? 1 : (res < -128 ? 1 : 0);
    setBit(&SR,6,overflow);

    //real calculation
    char resChar = a + b + carry;

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(&SR,7,resChar < 0 ? 1 : 0);

    return resChar;
}
