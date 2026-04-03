#include "Motherboard.h"
//? remove all //? comments later

CPUStruct* init(void) {
    CPUStruct *newCPU = (CPUStruct*)malloc(sizeof(CPUStruct));

    newCPU->A = 0;
	newCPU->X = 0;
	newCPU->Y = 0;
	newCPU->PC = 0xFFFC;
    newCPU->SP = 0x00;

	newCPU->c = 0;
    newCPU->z = 0;
    newCPU->i = 1;
    newCPU->d = 0;
    newCPU->b = 0;
    newCPU->v = 0;
    newCPU->n = 0;

	newCPU->cycles = 0;

	newCPU->halt = false;

    return newCPU;
}

int totalCycles = 0;


void static debug(CPUStruct* CPU) {
    char flags[] = { 'n' - 32 * CPU->n, 'v' - 32 * CPU->v, '-', '-', 'd' - 32 * CPU->d, 'i' - 32 * CPU->i, 'z' - 32 * CPU->z, 'c' - 32 * CPU->c, '\0'};
    printf("$%04X	%02X %02X        		        A:%02X    X:%02X    Y:%02X    SP:%02X   %s        Cycle: %d\n", CPU->PC, read(CPU->PC, CPU), read(CPU->PC + 1, CPU), CPU->A, CPU->X, CPU->Y, CPU->SP, flags, totalCycles);
    return;
}

void Run(CPUStruct* CPU) {
    printf("$FFFF	--		RESET			  A:00     X:00     Y:00     SP:00     nv--dizc     Cycle: 0\n");
    while (!CPU->halt) {
        execute(CPU);
    }
}

int end(CPUStruct* CPU) {
    free(CPU);
    return 0;
}






void execute(CPUStruct* CPU) {

    byte opcode = read(CPU->PC, CPU);

	//? debug info
	debug(CPU);

        CPU->PC++;
	Operations[opcode](CPU);


	//? remove this later?
    totalCycles += CPU->cycles;

    return;
}

int Reset(CPUStruct* CPU, char filepath[]) {
    // ^ SHOULD DO HEADER CHECK
    // open up our file
    errno = 0;
    FILE* fptr;
    fopen_s(&fptr, filepath, "rb");

	// check if it opened
    if (fptr == NULL) {
        perror("Error opening file");
        return 1;
    }

    // find the size of the file
    fseek(fptr, 0L, SEEK_END);
    const unsigned int size = (unsigned int)ftell(fptr);

	// go back to the start
    rewind(fptr);
    if (errno != 0) {
        //
        fclose(fptr);
        perror("Error rewinding file");
        return 1;
	}

    // create an array 
    byte* HeaderedROM = (byte*)malloc(size * sizeof(byte));
    if (HeaderedROM == NULL) {
        fclose(fptr);
        free(HeaderedROM);

        perror("Memory allocation failed");
        return 1;
    }

    // then read the entire file, plop it into here from the 10th index onwards
    fread(HeaderedROM, 1, size, fptr);
    memcpy_s(CPU->ROM, 0x8000, &HeaderedROM[0x10], 0x8000);

    // now we just need to read the PC reset vector
    CPU->PC = 0xFFFC;

    byte PCL = read(CPU->PC, CPU);
    byte PCH = read(CPU->PC + 1, CPU);

    CPU->PC = (ushort)((PCH * 0x100) + PCL);

	free(HeaderedROM);

    CPU->cycles = 7;

    //? don't forget to remove this later
    totalCycles += 7;
	fclose(fptr);

    CPU->SP -= 3;
	// stack pointer init
    CPU->i = 1;

    return 0;

}
