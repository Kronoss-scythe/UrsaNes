#ifndef MOTHERBOARD_H
#define MOTHERBOARD_H
//           X                                                                       
// X                   X                      X                     X                
//                                X                          X                       
//      X               X                           X                         X      
//                   X          X        X                     X                  X  
//           X                                    X                                  
//   X     █    ██  ██▀███    ██████  ▄▄▄   X      ███▄    █ ▓█████   ██████         
//         ██  ▓██▒▓██ ▒ ██▒▒██    ▒ ▒████▄        ██ ▀█   █ ▓█   ▀ ▒██    ▒         
//        ▓██  ▒██░▓██ ░▄█ ▒░ ▓██▄   ▒██  ▀█▄  X  ▓██  ▀█ ██▒▒███ X ░ ▓██▄           
//        ▓▓█  ░██░▒██▀▀█▄    ▒   ██▒░██▄▄▄▄██    ▓██▒  ▐▌██▒▒▓█  ▄   ▒   ██▒    X   
//        ▒▒█████▓ ░██▓ ▒██▒▒██████▒▒ ▓█   ▓██▒   ▒██░   ▓██░░▒████▒▒██████▒▒        
//  X     ░▒▓▒ ▒ ▒ ░ ▒▓ ░▒▓░▒ ▒▓▒ ▒ ░ ▒▒   ▓▒█░   ░ ▒░   ▒ ▒ ░░ ▒░ ░▒ ▒▓▒ ▒ ░   X    
//        ░░▒░ ░ ░   ░▒ ░ ▒░░ ░▒  ░ ░  ▒   ▒▒ ░   ░ ░░   ░ ▒░ ░ ░  ░░ ░▒  ░ ░        
//         ░░░ ░ ░   ░░   ░ ░  ░  ░    ░   ▒         ░   ░ ░    ░   ░  ░  ░         X
//  X        ░        ░           ░        ░  ░  X         ░    ░  ░      ░          
//                      X                                                            
//              X               X             X                  X             X     
//     X                            X                                                
//           X            X                         X        X             X         
//                  X                     X                                      X   
//  X            X                X                              X                   
//                                                                     X             
//                  X            X                X           X               X      
// https://www.asciiart.eu/ascii-draw-studio/app
//? remove all //? comments later
	#include "Includes.h"

	typedef unsigned short ushort;
	typedef unsigned char byte;

	struct CPUValues {
		ushort PC;
		byte SP;
		byte A;
		byte X;
		byte Y;

		byte RAM[0x800]; // ram woaw
		byte ROM[0x8000]; // rom waow


		bool c; // carry
		bool z; // zero
		bool i; // interupt
		bool d; // decimal
		bool b; // break
		bool v; // overflow
		bool n; // negative

		int cycles;
		
		bool halt;
	
	};
	typedef struct CPUValues CPUStruct;

	
	extern int totalCycles;



	//! =============================================================== 6502 =============================================================== //!
		CPUStruct* init(void);
		int end(CPUStruct* CPU);
		void debug(CPUStruct* CPU);

		void execute(CPUStruct* CPU); // execute an instruction
		
		// so the RAM and ROM are all in one like, lookup table, so from 0 - 0x8000 is ram, where it resets every 0x800 indexes to 0, because ram mirroring (0x00 == 0x800)
		// onwards we got our rom bitches
		inline byte read(ushort Address, CPUStruct *CPU) {
			if (Address < 0x8000) {
				return CPU->RAM[Address & 0x7FF];
			}
			else {
				return CPU->ROM[Address - 0x8000];
			}
		}

		inline void write(ushort Address, byte Value, CPUStruct* CPU) {
			if (Address < 0x8000) {
				CPU->RAM[Address & 0x7FF] = Value;
			}
			else {
				//? dont forget to remove this later
				printf("\tYou were trying to write to the ROM. It literally stands for READ ONLY MEMORY, dipshit :3. \n\t\t\tThe Address you were trying to write too was <%X>. \n\t\tRemember only addresses between 0x0000 and 0x7fff are part of RAM and its mirrors. \n\t\t0x000 to 0x7ff are the base RAM adresses and it repeats from there (So 0x0800 = 0x1000 = 0x0000)", Address);

			}

		}

		inline void push(byte Value, CPUStruct *CPU) {
			write((ushort)(0x0100 + CPU->SP), Value, CPU);
			CPU->SP--;
		}
		inline byte pull(CPUStruct* CPU) {
			CPU->SP++;
			return read((ushort)(0x0100 + CPU->SP), CPU);
		}

		int Reset(CPUStruct* CPU, char filepath[]); // reset the CPU

		void Run(CPUStruct* CPU);





		



		
	


		

		


		typedef void (*OperationFunc)(CPUStruct* CPU);
		extern const OperationFunc Operations[256];





#endif
