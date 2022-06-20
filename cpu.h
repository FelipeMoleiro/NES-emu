#include <stdlib.h>

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

    CPU();
    ~CPU(){};
    void nextIntruction();
    char get_oper(int addr_mode,u_short *mempos);
    char addBytes(char a,char b,bool withCarry);
    char andBytes(char a,char b);
    char aslByte(char a);
    char lsrByte(char a);
    void cmpBytes(char a,char b);
    char decByte(char a);
    char incByte(char a);
    char xorBytes(char a,char b);
    char orBytes(char a,char b);

    void pushStack(char byte);
    char pullStack();
};
