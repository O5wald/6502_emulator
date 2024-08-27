#include <iostream>
#include<stdint.h>

//some alias things to make code simple
using u8 = uint8_t;
using u16 = uint16_t;


// ----------------------------------------[Memory]--------------------------------------------------------
struct Memory{
        u8 mem_space[1024*64];
        void clearmem(){
                for(int i=0;i<sizeof(mem_space);i++){
                        mem_space[i] = 0x00;
                }
        }
        u8 read(u16 addr){
                u8 data = mem_space[addr];
                return data;
        }

        void write(u16 addr, u8 data){
                mem_space[addr] = data;
        }
};

//----------------------------------------------------------------------------------------------------------




// --------------------------------------------[CPU]----------------------------------------------------------

struct CPU{
        u16 PC,SP; //defined program counter and stack pointer
        
        // defined registers
        u8 A,X,Y;

        // processor status
        u8 C:1;
        u8 Z:1;
        u8 I:1;
        u8 D:1;
        u8 B:1;
        u8 O:1;
        u8 N:1;

        void initialize(Memory& mem){
                SP = 0x0FFF;
                PC = 0x00FF;

                A = 0x00;
                X = 0x00;
                Y = 0x00;

                C = Z = I = D = B = O = N = 0;
                mem.clearmem();
        }





        // opcodes
        static const u8 LDA_IMM = 0xA9;
        static const u8 LDA_ZP = 0xA5;
        static const u8 LDA_ZX = 0xB5;
        static const u8 LDA_ABS = 0xAD;
        static const u8 LDA_ABX = 0xBD;
        static const u8 LDA_ABY = 0xB9;




        // flag set functions 
        void LDA_flags(){
                Z = (A == 0);
                N = (A & 0b10000000) > 0; 
        }


        // Execute funciton

        u8 fetchbyte(int& cycles, Memory& mem){
                u8 data = mem.read(PC);
                PC++;
                cycles--;
                return data;
        } 

        u8 Readbyte(int& cycles, Memory& mem, u16 addr){
                u8 data = mem.read(addr);
                cycles--;
                return data;
        }


        void exec(int cycles,Memory& mem){
                while(cycles>0){
                        u8 instruction = fetchbyte(cycles, mem);
                        switch (instruction){
                                case LDA_IMM:{
                                                     u8 value = fetchbyte(cycles,mem);
                                                     A = value;
                                                     LDA_flags();
                                                     std::cout<<"Value of Accumalator = "<<(unsigned)A<<" in LDA_IMM opcode\n";
                                             }break;
                                case LDA_ZP:{
                                                    u8 zeropageaddr = fetchbyte(cycles, mem);
                                                    A = Readbyte(cycles,mem,zeropageaddr);
                                                    Z = (A==0);
                                                    N = (A & 0b10000000)>0;
                                                    std::cout<<"A = "<<(unsigned)A<<" in LDA_ZP \n";
                                            }break;
                                case LDA_ZX:{
                                                    u8 some_addr = fetchbyte(cycles, mem);
                                                    X = 0x03;
                                                    some_addr = some_addr + X;
                                                    cycles--;
                                                    A = Readbyte(cycles, mem, some_addr);
                                                    LDA_flags();
                                                    std::cout<<"A = "<<(unsigned)A<<" in LDA_ZX\n";
                                            }
                                case LDA_ABS:{
                                                     u8 some_addr = fetchbyte(cycles, mem);
                                                     u16 tempaddr;
                                                     tempaddr = some_addr << 8;
                                                     u8 addr = fetchbyte(cycles, mem);
                                                     tempaddr = tempaddr | addr;
                                                     std::cout<<"tampaddr = 0x"<<std::hex<<(unsigned)tempaddr<<"\n";
                                                     A = Readbyte(cycles, mem, tempaddr);
                                                     LDA_flags();
                                                     std::cout<<"A = "<<(unsigned)A<<" in LDA_ABS\n";
                                             }break;
                                case LDA_ABX:{
                                                     u8 some_addr = fetchbyte(cycles, mem);
                                                     u16 tempaddr;
                                                     tempaddr = some_addr << 8;
                                                     u8 addr = fetchbyte(cycles, mem);
                                                     tempaddr = tempaddr | addr;
                                                     A = Readbyte(cycles, mem, tempaddr+X);
                                                     LDA_flags();
                                                     std::cout<<"A = "<<(unsigned)A<<" in LDA_ABX\n";

                                             }break;
                                case LDA_ABY:{
                                                     u8 some_addr = fetchbyte(cycles, mem);
                                                     u16 tempaddr;
                                                     tempaddr= some_addr << 8;
                                                     u8 addr = fetchbyte(cycles,mem);
                                                     tempaddr = tempaddr | addr;
                                                     A = Readbyte(cycles, mem, tempaddr+Y);
                                                     LDA_flags();
                                                     std::cout<<"A = "<<(unsigned)A<<" in LDA_ABY\n";
                                             }break;
                                default:
                                {
                                        std::cout<<"Nothing to perform :)\n";
                                }break;
                        }
                }
        }
};


int main(){
        Memory ram;
        CPU cpu;
        cpu.initialize(ram);
        u16 currpc = cpu.PC;
        ram.mem_space[currpc] = cpu.LDA_ABS;
        ram.mem_space[++currpc] = 0xff;
        ram.mem_space[++currpc] = 0xba;
        ram.mem_space[0xffba] = 0x12;
        ram.mem_space[++currpc] = cpu.LDA_ABY;
        ram.mem_space[++currpc] = 0xfa;
        ram.mem_space[++currpc] = 0xbb;
        ram.mem_space[0xfabb] = 0x22;
        cpu.exec(10, ram);
        


        return 0;
}




