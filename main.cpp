#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

#include "cpu.h"

#define KB 1024

void read_ines(std::string path){
    FILE* inesFile = fopen(path.c_str(),"r");
    
    u_char header[16];
    fread(header,1,16,inesFile);

    bool hasTrainer = (header[6] & 0b00000100) >> 2;
    if(hasTrainer){
        //TODO
        u_char trainer[512];
        fread(trainer,1,512,inesFile);
    }

    int sizePRGROM = header[4] * 16 * KB;
    u_char PRGROM[sizePRGROM];
    fread(PRGROM,1,sizePRGROM,inesFile);

    fclose(inesFile);
}

char* load_rom(std::string path){
    FILE* rom = fopen(path.c_str(),"r");

    char* memory = (char*)malloc(64*KB);

    fread(memory,1,64*KB,rom);

    fclose(rom);
    return memory;
}

void printCpu(CPU &cpu){
    std::cout << "--------------------------------------------------\n";
    printf("PC: %.4X\n", cpu.PC);
    printf("AC: %.2X X: %.2X Y: %.2X\n", (u_char)cpu.AC,(u_char)cpu.X,(u_char)cpu.Y);
    printf("SR: ");
    for(int i=7;i>=0;i--){
        printf("%d", (cpu.SR >> i) & 1);
    }
    printf("\n");
    printf("SP: %.2X\n", (u_char)cpu.SP);
    std::cout << "--------------------------------------------------\n\n";
}


int main(){
    //read_ines("");
    
    
    /*
    FILE* test = fopen("test.bin","w+");
    
    char rom[64*KB];
    memset(rom,0xff,sizeof(char)*64*KB);
    rom[0x0] = 0x6C;
    rom[0x1] = 0xff;
    rom[0x2] = 0x00;
    rom[0x00ff] = 00;
    rom[0x00ff + 1] = 00;

    fclose(test);
    */

   /*
    u_char LL = 255;
    char X = 10;
    std::cout << (u_short)(u_char)(LL + (u_char)X) << '\n'; 
    */
    char* memory = load_rom("rom.bin");

    CPU main_cpu = CPU(memory);


    printCpu(main_cpu);
    char stopChar;
    scanf("%c", &stopChar);
    while(stopChar != 's'){
        main_cpu.nextIntruction();
        printCpu(main_cpu);
        scanf("%c", &stopChar);
    }

    return 0;
}