#include<stdint.h>
#include<iostream>

using u8 = uint8_t;
using u16 = uint16_t;

struct mem{
               u8 ram[1024*64];
                void clean_ram(){
                        for(int i=0;i<sizeof(ram);i++){
                                ram[i] = 0x00;
                        }
                }
                u8 read(int cycles, u16 pc){
                        return ram[pc];
                }
                
                void write(u16 addr, u8 data){
                        ram[addr] = data;
                }
};



struct cpu{
                u16 pc,sp; // program counter and stack pointer
                u8 A,X,Y; // Generel purpose registers
                
                u8 C:1; 
                u8 Z:1;
                u8 I:1;
                u8 D:1;
                u8 B:1;
                u8 V:1;
                u8 N:1;

                // opcodes

                void initialize(){
                       pc = 0x0000; 
                       sp = 0x0000;
                       C = Z = I = D = B = V = N = 0;
                       A = X = Y = 0;

                }

                // opcodes
               static const u8 LDA_IMM = 0xA9;

                
                // fetching from memory
                u8 fetching(int cycles,mem& a){
                               u8 data = a.ram[pc];
                                pc++;
                                cycles--;
                                return data;
                }

                void exec(int cycles, mem& a){
                        while(cycles>0){
                                u8 ins = fetching(cycles, a);
                                switch (ins){
                                        case LDA_IMM:{
                                                u8 value = fetching(cycles, a);
                                                A = value;
                                                Z = (A == 0);
                                                N = (A & 0b10000000) > 0;
                                        }
                                }break;
                                
                        }
                }
                
};



int main(){
        mem m;
        cpu c;
        m.clean_ram();
        m.write(0xA1, 0xA0); 
        std::cout<<(unsigned)c.A;

}
