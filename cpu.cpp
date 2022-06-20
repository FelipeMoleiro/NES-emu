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
                PC += oper - 2; //-2?
            }
            break;
        case 0xB0: //Branch on Carry Set
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,0) == 1){
                PC += oper - 2; //-2?
            }
            break;
        case 0xF0: //Branch on Result Zero
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,1) == 1){
                PC += oper - 2; //-2?
            }
            break;
        case 0x30: //Branch on Result Minus
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,7) == 1){
                PC += oper - 2; //-2?
            }
            break;
        case 0xD0: //Branch on Result not Zero
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,1) ==  0){
                PC += oper - 2; //-2?
            }
            break;
        case 0x10: //Branch on Result positive
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,7) == 0){
                PC += oper - 2; //-2?
            }
            break;
        case 0x50: //Branch on Overflow Clear
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,6) == 0){
                PC += oper - 2; //-2?
            }
            break;
        case 0x70: //Branch on Overflow Set
            oper = get_oper(RELATIVE,&mempos);
            if(getBit(SR,6) == 1){
                PC += oper - 2; //-2?
            }
            break;
        //Test Bits
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
        case 0xCA:
            X = decByte(X);
            break;
        case 0x88:
            Y = decByte(Y);
            break;
        //XOR Instruction
        case 0x49:
            oper = get_oper(IMMEDIATE,&mempos);
            AC = xorBytes(AC,oper);
            break;
        case 0x45:
            oper = get_oper(ZEROPAGE,&mempos);
            AC = xorBytes(AC,oper);
            break;
        case 0x55:
            oper = get_oper(ZEROPAGEX,&mempos);
            AC = xorBytes(AC,oper);
            break;
        case 0x4D:
            oper = get_oper(ABSOLUTE,&mempos);
            AC = xorBytes(AC,oper);
            break;
        case 0x5D:
            oper = get_oper(ABSOLUTEX,&mempos);
            AC = xorBytes(AC,oper);
            break;
        case 0x59:
            oper = get_oper(ABSOLUTEY,&mempos);
            AC = xorBytes(AC,oper);
            break;
        case 0x41:
            oper = get_oper(INDIRECTX,&mempos);
            AC = xorBytes(AC,oper);
            break;
        case 0x51:
            oper = get_oper(INDIRECTY,&mempos);
            AC = xorBytes(AC,oper);
            break;
        //INC Instruction
        case 0xE6:
            oper = get_oper(ZEROPAGE,&mempos);
            memory[mempos] = incByte(oper);
            break;
        case 0xF6:
            oper = get_oper(ZEROPAGEX,&mempos);
            memory[mempos] = incByte(oper);
            break;
        case 0xEE:
            oper = get_oper(ABSOLUTE,&mempos);
            memory[mempos] = incByte(oper);
            break;
        case 0xFE:
            oper = get_oper(ABSOLUTEX,&mempos);
            memory[mempos] = incByte(oper);
            break;
        case 0xE8:
            X = incByte(X);
            break;
        case 0xC8:
            Y = incByte(Y);
            break;
        //JMP Intruction
        case 0x4C:
            oper = get_oper(ABSOLUTE,&mempos);
            PC = mempos;
            break;
        case 0x6C:
            oper = get_oper(INDIRECT,&mempos);
            PC = mempos;
            break;
        case 0x20:
            pushStack((u_char)((PC + 1) >> 8)); //PCH
            pushStack((u_char)((PC + 1) & 0xFF)); //PCL
            oper = get_oper(INDIRECT,&mempos);
            PC = mempos;
            break;
        //LDA Instruction
        case 0xA9:
            oper = get_oper(IMMEDIATE,&mempos);
            AC = orBytes(0,oper);
            break;
        case 0xA5:
            oper = get_oper(ZEROPAGE,&mempos);
            AC = orBytes(0,oper);
            break;
        case 0xB5:
            oper = get_oper(ZEROPAGEX,&mempos);
            AC = orBytes(0,oper);
            break;
        case 0xAD:
            oper = get_oper(ABSOLUTE,&mempos);
            AC = orBytes(0,oper);
            break;
        case 0xBD:
            oper = get_oper(ABSOLUTEX,&mempos);
            AC = orBytes(0,oper);
            break;
        case 0xB9:
            oper = get_oper(ABSOLUTEY,&mempos);
            AC = orBytes(0,oper);
            break;
        case 0xA1:
            oper = get_oper(INDIRECTX,&mempos);
            AC = orBytes(0,oper);
            break;
        case 0xB1:
            oper = get_oper(INDIRECTY,&mempos);
            AC = orBytes(0,oper);
            break;
        //LDX Instruction
        case 0xA2:
            oper = get_oper(IMMEDIATE,&mempos);
            X = orBytes(0,oper);
            break;
        case 0xA6:
            oper = get_oper(ZEROPAGE,&mempos);
            X = orBytes(0,oper);
            break;
        case 0xB6:
            oper = get_oper(ZEROPAGEY,&mempos);
            X = orBytes(0,oper);
            break;
        case 0xAE:
            oper = get_oper(ABSOLUTE,&mempos);
            X = orBytes(0,oper);
            break;
        case 0xBE:
            oper = get_oper(ABSOLUTEY,&mempos);
            X = orBytes(0,oper);
            break;
        //LDY Instruction
        case 0xA0:
            oper = get_oper(IMMEDIATE,&mempos);
            Y = orBytes(0,oper);
            break;
        case 0xA4:
            oper = get_oper(ZEROPAGE,&mempos);
            Y = orBytes(0,oper);
            break;
        case 0xB4:
            oper = get_oper(ZEROPAGEX,&mempos);
            Y = orBytes(0,oper);
            break;
        case 0xAC:
            oper = get_oper(ABSOLUTE,&mempos);
            Y = orBytes(0,oper);
            break;
        case 0xBC:
            oper = get_oper(ABSOLUTEX,&mempos);
            Y = orBytes(0,oper);
            break;
        //LSR Instruction
        case 0x4A:
            AC = lsrByte(AC); //ACCUMULATOR
            break;
        case 0x46:
            oper = get_oper(ZEROPAGE,&mempos);
            memory[mempos] = lsrByte(oper);
            break;
        case 0x56:
            oper = get_oper(ZEROPAGEX,&mempos);
            memory[mempos] = lsrByte(oper);
            break;
        case 0x4E:
            oper = get_oper(ABSOLUTE,&mempos);
            memory[mempos] = lsrByte(oper);
            break;
        case 0x5E:
            oper = get_oper(ABSOLUTEX,&mempos);
            memory[mempos] = lsrByte(oper);
            break;
        //NOP Instruction
        case 0xEA:
            break;
        //OR Instruction
        case 0x09:
            oper = get_oper(IMMEDIATE,&mempos);
            AC = orBytes(AC,oper);
            break;
        case 0x05:
            oper = get_oper(ZEROPAGE,&mempos);
            AC = orBytes(AC,oper);
            break;
        case 0x15:
            oper = get_oper(ZEROPAGEX,&mempos);
            AC = orBytes(AC,oper);
            break;
        case 0x0D:
            oper = get_oper(ABSOLUTE,&mempos);
            AC = orBytes(AC,oper);
            break;
        case 0x1D:
            oper = get_oper(ABSOLUTEX,&mempos);
            AC = orBytes(AC,oper);
            break;
        case 0x19:
            oper = get_oper(ABSOLUTEY,&mempos);
            AC = orBytes(AC,oper);
            break;
        case 0x01:
            oper = get_oper(INDIRECTX,&mempos);
            AC = orBytes(AC,oper);
            break;
        case 0x11:
            oper = get_oper(INDIRECTY,&mempos);
            AC = orBytes(AC,oper);
            break;
        //Push Instruction
        case 0x48: //Push Accumulator
            pushStack(AC);
            break;
        case 0x08: //Push Status
            oper = SR;
            setBit(&oper,4,1);
            setBit(&oper,5,1);
            pushStack(SR);
            break;
        //pull Instructions
        case 0x68: //pull Accumulator
            AC = pullStack();
            setBit(&SR,1,AC == 0 ? 1 : 0); //check for 0      
            setBit(&SR,7,AC < 0 ? 1 : 0); //check for negative
            break;
        case 0x28: //pull SR
            oper = pullStack();
            setBit(&oper,4,getBit(SR,4));
            setBit(&oper,5,getBit(SR,5));
            SR = oper;
            break;
        //ROL Instructions
        case 0x2A:
            AC = rolByte(AC); //ACCUMULATOR
            break;
        case 0x26:
            oper = get_oper(ZEROPAGE,&mempos);
            memory[mempos] = rolByte(oper);
            break;
        case 0x36:
            oper = get_oper(ZEROPAGEX,&mempos);
            memory[mempos] = rolByte(oper);
            break;
        case 0x2E:
            oper = get_oper(ABSOLUTE,&mempos);
            memory[mempos] = rolByte(oper);
            break;
        case 0x3E:
            oper = get_oper(ABSOLUTEX,&mempos);
            memory[mempos] = rolByte(oper);
            break;
        //ROR Instructions
        case 0x6A:
            AC = rorByte(AC); //ACCUMULATOR
            break;
        case 0x66:
            oper = get_oper(ZEROPAGE,&mempos);
            memory[mempos] = rorByte(oper);
            break;
        case 0x76:
            oper = get_oper(ZEROPAGEX,&mempos);
            memory[mempos] = rorByte(oper);
            break;
        case 0x6E:
            oper = get_oper(ABSOLUTE,&mempos);
            memory[mempos] = rorByte(oper);
            break;
        case 0x7E:
            oper = get_oper(ABSOLUTEX,&mempos);
            memory[mempos] = rorByte(oper);
            break;
        //RIT Instruction - Return from interrupt
        case 0x40:
            oper = pullStack();
            setBit(&oper,4,getBit(SR,4));
            setBit(&oper,5,getBit(SR,5));
            SR = oper;
            PC = pullStack(); //PCL
            PC += pullStack() << 8; //PCH
            PC++; //next instruction
            break;
        //RTS - Return from subroutine
        case 0x60:
            PC = pullStack(); //PCL
            PC += pullStack() << 8; //PCH
            break;
        //SBC
        case 0xE9:
            oper = get_oper(IMMEDIATE,&mempos);
            AC = subBytes(AC,oper,1);
            break;
        case 0xE5:
            oper = get_oper(ZEROPAGE,&mempos);
            AC = subBytes(AC,oper,1);
            break;
        case 0xF5:
            oper = get_oper(ZEROPAGEX,&mempos);
            AC = subBytes(AC,oper,1);
            break;
        case 0xED:
            oper = get_oper(ABSOLUTE,&mempos);
            AC = subBytes(AC,oper,1);
            break;
        case 0xFD:
            oper = get_oper(ABSOLUTEX,&mempos);
            AC = subBytes(AC,oper,1);
            break;
        case 0xF9:
            oper = get_oper(ABSOLUTEY,&mempos);
            AC = subBytes(AC,oper,1);
            break;
        case 0xE1:
            oper = get_oper(INDIRECTX,&mempos);
            AC = subBytes(AC,oper,1);
            break;
        case 0xF1:
            oper = get_oper(INDIRECTY,&mempos);
            AC = subBytes(AC,oper,1);
            break;
        default:
            break;
    }

}

char CPU::rorByte(char a){
    char resChar = (a >> 1) + (a << 7);

    //check for carry
    setBit(&SR,0,getBit(a,0) == 1 ? 1 : 0);

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(&SR,7,resChar < 0 ? 1 : 0);

    return resChar;
}

char CPU::rolByte(char a){
    char resChar = (a << 1) + (a >> 7);

    //check for carry
    setBit(&SR,0,getBit(a,7) == 1 ? 1 : 0);

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(&SR,7,resChar < 0 ? 1 : 0);

    return resChar;
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

/*
sub bytes with borrow
*/
char CPU::subBytes(char a,char b,bool withBorrow){
    char resChar;
    if(withBorrow){
        resChar = addBytes(a,~b,1);
    }else{
        resChar = addBytes(a,-b,0);
    }
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
    setBit(&SR,0,getBit(a,7) == 1 ? 1 : 0);

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(&SR,7,resChar < 0 ? 1 : 0);

    return resChar;
}

char CPU::lsrByte(char a){
    char resChar = a >> 1;

    //check for carry
    setBit(&SR,0,getBit(a,0) == 1 ? 1 : 0);

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //mark not negative
    setBit(&SR,7,0);

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

char CPU::incByte(char a){
    //real calculation
    char resChar = a + (char)1;

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(&SR,7,resChar < 0 ? 1 : 0);

    return resChar;
}

char CPU::xorBytes(char a,char b){
    //real calculation
    char resChar = a ^ b;

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(&SR,7,resChar < 0 ? 1 : 0);

    return resChar;
}

char CPU::orBytes(char a,char b){
    //real calculation
    char resChar = a | b;

    //check for 0
    setBit(&SR,1,resChar == 0 ? 1 : 0);

    //check for negative
    setBit(&SR,7,resChar < 0 ? 1 : 0);

    return resChar;
}

void CPU::pushStack(char byte){
    //TODO: VERIFY IF STACK OVERFLOW
    memory[SP--] = byte; 
}

char CPU::pullStack(){
    //TODO: VERIFY IF STACK EMPTY
    return memory[SP++];
}