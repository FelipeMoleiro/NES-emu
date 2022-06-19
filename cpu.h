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
    char get_oper(int addr_mode);
    char addBytes(char a,char b,bool withCarry);
    void nextIntruction();
};
