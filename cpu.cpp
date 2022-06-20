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
    u_short mempos;
    switch (opc) {
        case 0x00: //Break

            //TODO

            //push to stack PC+2
            //push status register with break flag set
            break;

        //ADC Instructions
        case 0x69:
            oper = get_oper(IMMEDIATE,&mempos);
            AC = addBytes(AC,oper,1);
            break;
        case 0x65:
            oper = get_oper(ZEROPAGE,&mempos);
            AC = addBytes(AC,oper,1);
            break;
        case 0x75:
            oper = get_oper(ZEROPAGEX,&mempos);
            AC = addBytes(AC,oper,1);
            break;
        case 0x6D:
            oper = get_oper(ABSOLUTE,&mempos);
            AC = addBytes(AC,oper,1);
            break;
        case 0x7D:
            oper = get_oper(ABSOLUTEX,&mempos);
            AC = addBytes(AC,oper,1);
            break;
        case 0x79:
            oper = get_oper(ABSOLUTEY,&mempos);
            AC = addBytes(AC,oper,1);
            break;
        case 0x61:
            oper = get_oper(INDIRECTX,&mempos);
            AC = addBytes(AC,oper,1);
            break;
        case 0x71:
            oper = get_oper(INDIRECTY,&mempos);
            AC = addBytes(AC,oper,1);
            break;
        //AND INSTRUCTIONS
        case 0x29:
            oper = get_oper(IMMEDIATE,&mempos);
            AC = andBytes(AC,oper);
            break;
        case 0x25:
            oper = get_oper(ZEROPAGE,&mempos);
            AC = andBytes(AC,oper);
            break;
        case 0x35:
            oper = get_oper(ZEROPAGEX,&mempos);
            AC = andBytes(AC,oper);
            break;
        case 0x2D:
            oper = get_oper(ABSOLUTE,&mempos);
            AC = andBytes(AC,oper);
            break;
        case 0x3D:
            oper = get_oper(ABSOLUTEX,&mempos);
            AC = andBytes(AC,oper);
            break;
        case 0x39:
            oper = get_oper(ABSOLUTEY,&mempos);
            AC = andBytes(AC,oper);
            break;
        case 0x21:
            oper = get_oper(INDIRECTX,&mempos);
            AC = andBytes(AC,oper);
            break;
        case 0x31:
            oper = get_oper(INDIRECTY,&mempos);
            AC = andBytes(AC,oper);
            break;
        //ASL INSTRUCTIONS
        case 0x0A:
            AC = aslByte(AC); //ACCUMULATOR
            break;
        case 0x06:
            oper = get_oper(ZEROPAGE,&mempos);
            memory[mempos] = aslByte(oper);
            break;
        case 0x16:
            oper = get_oper(ZEROPAGEX,&mempos);
            memory[mempos] = aslByte(oper);
            break;
        case 0x0E:
            oper = get_oper(ABSOLUTE,&mempos);
            memory[mempos] = aslByte(oper);
            break;
        case 0x1E:
            oper = get_oper(ABSOLUTEX,&mempos);
            memory[mempos] = aslByte(oper);
            break;
        //Branch INSTRUCTIONS
        case 0x90: //Branch on Carry Clear
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,0) == 0){
                PC += oper;
            }
            break;
        case 0xB0: //Branch on Carry Set
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,0) == 1){
                PC += oper;
            }
            break;
        case 0xF0: //Branch on Result Zero
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,1) == 1){
                PC += oper;
            }
            break;
        case 0x24: //Test Bits in Memory with Accumulator
            oper = get_oper(ZEROPAGE,&mempos);
            andBytes(AC,oper);
            setBit(&SR,7,getBit(oper,7));
            setBit(&SR,6,getBit(oper,6));
            break;
        case 0x2C: //Test Bits in Memory with Accumulator
            oper = get_oper(ABSOLUTE,&mempos);
            andBytes(AC,oper);
            setBit(&SR,7,getBit(oper,7));
            setBit(&SR,6,getBit(oper,6));
            break;
        case 0x30: //Branch on Result Minus
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,7) == 1){
                PC += oper;
            }
            break;
        case 0xD0: //Branch on Result not Zero
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,1) ==  0){
                PC += oper;
            }
            break;
        case 0x10: //Branch on Result positive
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,7) == 0){
                PC += oper;
            }
            break;
        case 0x50: //Branch on Overflow Clear
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,6) == 0){
                PC += oper;
            }
            break;
        case 0x70: //Branch on Overflow Set
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,6) == 1){
                PC += oper;
            }
            break;
        //CLEAR FLAGS
        case 0x18: //Clear Carry Flag 
            setBit(&SR,0,0);
            break;
        case 0xD8: //Clear Decimal Flag 
            setBit(&SR,3,0);
            break;
        case 0x58: //Clear Interrupt Disable Flag 
            setBit(&SR,2,0);
            break;
        case 0xB8: //Clear Overflow Flag 
            setBit(&SR,6,0);
            break;

        //CMP With Accumulator
        case 0xC9:
            oper = get_oper(IMMEDIATE,&mempos);
            cmpBytes(AC,oper);
            break;
        case 0xC5:
            oper = get_oper(ZEROPAGE,&mempos);
            cmpBytes(AC,oper);
            break;
        case 0xD5:
            oper = get_oper(ZEROPAGEX,&mempos);
            cmpBytes(AC,oper);
            break;
        case 0xCD:
            oper = get_oper(ABSOLUTE,&mempos);
            cmpBytes(AC,oper);
            break;
        case 0xDD:
            oper = get_oper(ABSOLUTEX,&mempos);
            cmpBytes(AC,oper);
            break;
        case 0xD9:
            oper = get_oper(ABSOLUTEY,&mempos);
            cmpBytes(AC,oper);
            break;
        case 0xC1:
            oper = get_oper(INDIRECTX,&mempos);
            cmpBytes(AC,oper);
            break;
        case 0xD1:
            oper = get_oper(INDIRECTY,&mempos);
            cmpBytes(AC,oper);
            break;
        //CMP With X
        case 0xE0:
            oper = get_oper(IMMEDIATE,&mempos);
            cmpBytes(X,oper);
            break;
        case 0xE4:
            oper = get_oper(ZEROPAGE,&mempos);
            cmpBytes(X,oper);
            break;
        case 0xEC:
            oper = get_oper(ABSOLUTE,&mempos);
            cmpBytes(X,oper);
            break;
        //CMP With Y
        case 0xC0:
            oper = get_oper(IMMEDIATE,&mempos);
            cmpBytes(Y,oper);
            break;
        case 0xC4:
            oper = get_oper(ZEROPAGE,&mempos);
            cmpBytes(Y,oper);
            break;
        case 0xCC:
            oper = get_oper(ABSOLUTE,&mempos);
            cmpBytes(Y,oper);
            break;
        //DEC Instructions
        case 0xC6:
            oper = get_oper(ZEROPAGE,&mempos);
            memory[mempos] = decByte(oper);
            break;
        case 0xD6:
            oper = get_oper(ZEROPAGEX,&mempos);
            memory[mempos] = decByte(oper);
            break;
        case 0xCE:
            oper = get_oper(ABSOLUTE,&mempos);
            memory[mempos] = decByte(oper);
            break;
        case 0xDE:
            oper = get_oper(ABSOLUTEX,&mempos);
            memory[mempos] = decByte(oper);
            break;

        default:
            break;
    }

}

char CPU::get_oper(int addr_mode,u_short *mempos){
    char oper;
    u_char LL;
    u_char HH;
    u_char LL2;
    u_char HH2;
    switch (addr_mode){
        case ABSOLUTE:
            LL = (u_char)memory[PC++];
            HH = (u_char)memory[PC++];
            *mempos = ((u_short)HH << 8) + (u_short)LL;
            oper = memory[*mempos];
            break;
        case ABSOLUTEX:
            LL = (u_char)memory[PC++];
            HH = (u_char)memory[PC++];
            *mempos = ((u_short)HH << 8) + (u_short)LL + (u_char)X; //with carry
            oper = memory[*mempos]; 
            break;
        case ABSOLUTEY:
            LL = (u_char)memory[PC++];
            HH = (u_char)memory[PC++];
            *mempos = ((u_short)HH << 8) + (u_short)LL + (u_char)Y; //with carry
            oper = memory[*mempos]; 
            break;
        case IMMEDIATE:
            oper = memory[PC++];
            break;
        case INDIRECT:
            LL = (u_char)memory[PC++];
            HH = (u_char)memory[PC++];
            LL2 = memory[((u_short)HH << 8) + (u_short)LL];
            HH2 = memory[((u_short)HH << 8) + (u_short)(u_char)(LL + (u_char)1)]; //without carry(although its not supposed to happen anyway)
           *mempos = ((u_short)HH2 << 8) + (u_short)LL2;
            oper = memory[*mempos];
            break;
        case INDIRECTX:
            LL = (u_char)memory[PC++];
            LL2 = memory[(u_short)(u_char)(LL + (u_char)X)]; //without carry
            HH2 = memory[(u_short)(u_char)(LL + (u_char)X + (u_char)1)]; //without carry
            *mempos = ((u_short)HH2 << 8) + (u_short)LL2;
            oper = memory[*mempos];
            break;
        case INDIRECTY:
            LL = (u_char)memory[PC++];
            LL2 = memory[(u_short)LL];
            HH2 = memory[(u_short)(u_char)(LL + (u_char)1)];
            *mempos = ((u_short)HH2 << 8) + (u_short)LL2 + (u_char)Y; // with carry
            oper = memory[*mempos]; 
            break;
        case ZEROPAGE:
            LL = (u_char)memory[PC++];
            *mempos = (u_short)LL;
            oper = memory[*mempos];
            break;
        case ZEROPAGEX:
            LL = (u_char)memory[PC++];
            *mempos = (u_short)(u_char)(LL + (u_char)X);
            oper = memory[*mempos];
            break;
        case ZEROPAGEY:
            LL = (u_char)memory[PC++];
            *mempos = (u_short)(u_char)(LL + (u_char)Y);
            oper = memory[*mempos];
            break;
        case RELATIVE:
            *mempos = PC++;
            oper = memory[*mempos];
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

char CPU::andBytes(char a,char b){
    char resChar = a & b;

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(&SR,7,resChar < 0 ? 1 : 0);

    return resChar;
}


char CPU::aslByte(char a){
    char resChar = a << 1;

    //check for carry
    setBit(&SR,1,getBit(a,7) == 1 ? 1 : 0);

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(&SR,7,resChar < 0 ? 1 : 0);

    return resChar;
}

void CPU::cmpBytes(char a,char b){
    //check for carry
    int res = (int)(u_char)a + (int)(u_char)b;
    bool carryFlag = res >> 8;
    setBit(&SR,0,carryFlag);

    //real calculation
    char resChar = a - b;

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(&SR,7,resChar < 0 ? 1 : 0);
}


char CPU::decByte(char a){
    //real calculation
    char resChar = a - (char)1;

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(&SR,7,resChar < 0 ? 1 : 0);

    return resChar;
}