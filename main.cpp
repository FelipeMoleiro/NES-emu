#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

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
    return 0;
}