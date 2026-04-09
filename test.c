//? remove all //? comments later
#include "Motherboard.h"
#include "Renderer.h"


#include <time.h>
#include <string.h>
//void traceDebug(CPUStruct* CPU);

int main(void) {
    //char filepath[1024] = "\0";
    //CPUStruct* CPU = init();
    //do {
    //    printf("HELLO, input file path:\n");
    //    fgets(filepath, 1024, stdin);
    //    *strchr(filepath, '\n') = '\0';
    //    end(CPU);
    //    CPU = init();
    //}while(Reset(CPU, filepath));
    //Run(CPU);

    //end(CPU);

    testWindows();

    return 0;
}


//void traceDebug(CPUStruct* CPU) {
//    FILE* fptr;
//    fopen_s(&fptr, traceFile, "r");
//
//    char buffer[1024];
//    int PC; int A; int X; int Y; int SP; int Cycles;
//    int i = 0;
//
//    fgets(buffer, 1024, fptr);
//    sscanf_s(buffer + 1, "%x ", &PC);
//
//    if (CPU->PC != PC) {
//        printf("PC wrong.\n Expected: \n  %s\n Got:\n  ", buffer);
//        debug(CPU);
//
//        fclose(fptr);
//        return;
//    }
//
//    
//
//    while (buffer[i] != 'A' || buffer[i + 1] != ':') {
//        i++;
//    }
//
//    sscanf_s(buffer + i + 2, "%x ", &A);
//
//    if (CPU->A != A) {
//        printf("A wrong.\n Expected: \n  %s\n Got:\n  "  , buffer);
//        debug(CPU);
//
//        fclose(fptr);
//        return;
//    }
//
//    i += 5;
//
//    sscanf_s(buffer + i + 2, "%x ", &X);
//
//    if (CPU->X != X) {
//        printf("X wrong.\n Expected: \n  %s\n Got:\n  ", buffer);
//        debug(CPU);
//
//        fclose(fptr);
//        return;
//    }
//
//    i += 5;
//
//    sscanf_s(buffer + i + 2, "%x ", &Y);
//
//    if (CPU->Y != Y) {
//        printf("Y wrong.\n Expected: \n  %s\n Got:\n  ", buffer);
//        debug(CPU);
//
//        fclose(fptr);
//        return;
//    }
//
//    i += 5;
//
//    sscanf_s(buffer + i + 3, "%x ", &SP);
//
//    if (CPU->SP != SP) {
//        printf("SP wrong.\n Expected: \n  %s\n Got:\n  ", buffer);
//        debug(CPU);
//
//        fclose(fptr);
//        return;
//    }
//
//    i += 6;
//
//    //flags = { 'n' - 32 * CPU->n, 'v' - 32 * CPU->v, '-', '-', 'd' - 32 * CPU->d, 'i' - 32 * CPU->i, 'z' - 32 * CPU->z, 'c' - 32 * CPU->c, '\0' };
//    if ('n' - 32 * CPU->n != buffer[i]
//        || 'v' - 32 * CPU->v != buffer[i + 1]
//        || 'd' - 32 * CPU->d != buffer[i + 4]
//        || 'i' - 32 * CPU->i != buffer[i + 5]
//        || 'z' - 32 * CPU->z != buffer[i + 6]
//        || 'c' - 32 * CPU->c != buffer[i + 7]
//        ) {
//        printf("flags wrong.\n Expected: \n  %s\n Got:\n  ", buffer);
//        debug(CPU);
//
//        fclose(fptr);
//        return;
//    }
//
//    i += 8;
//
//    sscanf_s(buffer + i + 8, "%d ", &Cycles);
//
//    if (totalCycles != Cycles) {
//        printf("Cycles wrong.\n Expected: \n  %s\n Got:\n  ", buffer);
//        debug(CPU);
//
//        fclose(fptr);
//        return;
//    }
//
//
//
//
//    while (fgets(buffer, 1024, fptr)) {
//        i = 0;
//        execute(CPU);
//
//        sscanf_s(buffer + 1, "%x ", &PC);
//
//        if (CPU->PC != PC) {
//            printf("PC wrong.\n Expected: \n  %s\n Got:  \n  ", buffer);
//            debug(CPU);
//
//            fclose(fptr);
//            return;
//        }
//
//
//
//        while (buffer[i] != 'A' || buffer[i + 1] != ':') {
//            i++;
//        }
//
//        sscanf_s(buffer + i + 2, "%x ", &A);
//
//        if (CPU->A != A) {
//            printf("A wrong.\n Expected: \n  %s\n Got:\n", buffer);
//            debug(CPU);
//            Operations[0](CPU);
//
//            fclose(fptr);
//            return;
//        }
//
//        i += 5;
//
//        sscanf_s(buffer + i + 2, "%x ", &X);
//
//        if (CPU->X != X) {
//            printf("X wrong.\n Expected: \n  %s\n Got:\n  ", buffer);
//            debug(CPU);
//
//            fclose(fptr);
//            return;
//        }
//
//        i += 5;
//
//        sscanf_s(buffer + i + 2, "%x ", &Y);
//
//        if (CPU->Y != Y) {
//            printf("Y wrong.\n Expected: \n  %s\n Got:\n  ", buffer);
//            debug(CPU);
//
//            fclose(fptr);
//            return;
//        }
//
//        i += 5;
//
//        sscanf_s(buffer + i + 3, "%x ", &SP);
//
//        if (CPU->SP != SP) {
//            printf("SP wrong.\n Expected: \n  %s\n Got:\n  ", buffer);
//            debug(CPU);
//
//            fclose(fptr);
//            return;
//        }
//
//        i += 6;
//
//        //flags = { 'n' - 32 * CPU->n, 'v' - 32 * CPU->v, '-', '-', 'd' - 32 * CPU->d, 'i' - 32 * CPU->i, 'z' - 32 * CPU->z, 'c' - 32 * CPU->c, '\0' };
//        if ('n' - 32 * CPU->n != buffer[i]
//            || 'v' - 32 * CPU->v != buffer[i + 1]
//            || 'd' - 32 * CPU->d != buffer[i + 4]
//            || 'i' - 32 * CPU->i != buffer[i + 5]
//            || 'z' - 32 * CPU->z != buffer[i + 6]
//            || 'c' - 32 * CPU->c != buffer[i + 7]
//            ) {
//            printf("flags wrong.\n Expected: \n  %s\n Got:\n  ", buffer);
//            debug(CPU);
//
//            fclose(fptr);
//            return;
//        }
//
//        i += 8;
//
//        sscanf_s(buffer + i + 8, "%d ", &Cycles);
//
//        if (totalCycles != Cycles) {
//            printf("Cycles wrong.\n Expected: \n  %s\n Got:\n  ", buffer);
//            debug(CPU);
//
//            fclose(fptr);
//            return;
//        }
//
//    }
//
//    fclose(fptr);
//}
//
//
