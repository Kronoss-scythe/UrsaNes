#include "Motherboard.h"
//? remove all //? comments later

/* immediate:
CPU->cycles = _;

read(CPU.PC, CPU)
CPU.PC++;
*/

/* zeropage:
CPU->cycles = _;

byte address = read(CPU->PC, CPU) + arg;
	CPU->PC++;

read(address, CPU);
*/

/* absolute:
CPU->cycles = _;

byte low = read(CPU->PC, CPU);
	CPU->PC++;
byte high = read(CPU->PC, CPU);
	CPU->PC++;

//if we want to add CPU->cycles
ushort address = low + args;
if (address > 0xFF) {
	CPU->cycles++;
}
ushort address = address + (high * 256);
read(address, CPU);
*/

/* indirect:
CPU->cycles = _;

byte low = read(CPU->PC, CPU);
	CPU->PC++;
byte high = read(CPU->PC, CPU);
	CPU->PC++;

//if we want to add CPU->cycles
if (low > low + args) {
	CPU->cycles++;
}
low = low + args;

ushort address = (high * 256) + low;
byte l2 = read(address, CPU);
byte h2 = read(address + 1, CPU);

address = (h2 * 256) + l2;
*/

//static inline void ADC(CPUStruct* CPU, byte value) {
//	int Sum = CPU->A + value + CPU->c;
//
//	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
//	CPU->v = ((~(CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
//	CPU->c = (Sum > 0xFF);
//
//	CPU->A = (byte)(Sum);
//
//	CPU->n = (CPU->A > 0x7F);
//	CPU->z = (CPU->A == 0x00);
//}
////
//static inline byte zeroPage(CPUStruct* CPU) {
//	return read(CPU->PC, CPU);
//}
//
//static inline byte zeroPageX(CPUStruct* CPU) {
//	return read(CPU->PC, CPU) + CPU->X;
//}
//
//static inline ushort absolute(CPUStruct* CPU) {
//	byte low = read(CPU->PC, CPU);
//		CPU->PC++;
//	byte high = read(CPU->PC, CPU);
//
//	return (ushort)((high * 256) + low);
//}

//static inline ushort absoluteX(CPUStruct* CPU) {
//	byte low = read(CPU->PC, CPU);
//		CPU->PC++;
//	byte high = read(CPU->PC, CPU);
//
//	ushort address = low + CPU->X;
//
//	if (address > 0xFF) {
//		CPU->cycles++;
//	}
//
//	return address + (high * 256);
//}

//static inline ushort absoluteXNoAdd(CPUStruct* CPU) {
//	byte low = read(CPU->PC, CPU);
//		CPU->PC++;
//	byte high = read(CPU->PC, CPU);
//
//	ushort address = low + CPU->X;
//
//	return address + (high * 256);
//}

//================================================ ADC ================================================//
static void ADC_69(CPUStruct* CPU) { // 0x69, add with carry, immediate
	CPU->cycles = 2;
	
	byte value = read(CPU->PC, CPU);
	int Sum = CPU->A + value + CPU->c;
		CPU->PC++;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = ((~(CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum > 0xFF);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ADC_65(CPUStruct* CPU) { // 0x65, add with carry, zeropage
	CPU->cycles = 3;
	
	byte address = read(CPU->PC, CPU) + CPU->X;
		CPU->PC++;

	byte value = read(address, CPU);
	int Sum = CPU->A + value + CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = ((~(CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum > 0xFF);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ADC_75(CPUStruct* CPU) { // 0x75, add with carry, zeropage,x
	CPU->cycles = 4;

	byte address = read(CPU->PC, CPU) + CPU->X;
		CPU->PC++;

	byte value = read(address, CPU);
	int Sum = CPU->A + value + CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = ((~(CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum > 0xFF);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ADC_6D(CPUStruct* CPU) { // 0x6D, add with carry, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);
	int Sum = CPU->A + value + CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = ((~(CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum > 0xFF);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ADC_7D(CPUStruct* CPU) { // 0x7D, add with carry, absolute,x
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = low + CPU->X;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	byte value = read(address, CPU);
	int Sum = CPU->A + value + CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = ((~(CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum > 0xFF);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ADC_79(CPUStruct* CPU) { // 0x79, add with carry, absolute,y
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	byte value = read(address, CPU);
	int Sum = CPU->A + value + CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = ((~(CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum > 0xFF);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ADC_61(CPUStruct* CPU) { // 0x61, add with carry, (indirect,x)
	CPU->cycles = 6;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	initAddress = initAddress + CPU->X;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);
	int Sum = CPU->A + value + CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = ((~(CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum > 0xFF);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ADC_71(CPUStruct* CPU) { // 0x71, add with carry, (indirect),y
	CPU->cycles = 6;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	byte value = read(address, CPU);
	int Sum = CPU->A + value + CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = ((~(CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum > 0xFF);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}



//================================================ AND ================================================//
static void AND_29(CPUStruct* CPU) { // 0x29, and, immediate
	CPU->cycles = 2;

	CPU->A = CPU->A & read(CPU->PC, CPU);
		CPU->PC++;

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void AND_25(CPUStruct* CPU) { // 0x25, and, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
		CPU->PC++;

	CPU->A = CPU->A & read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void AND_35(CPUStruct* CPU) { // 0x35, and, zeropage,x
	CPU->cycles = 4;

	byte address = read(CPU->PC, CPU) + CPU->X;
		CPU->PC++;

	CPU->A = CPU->A & read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void AND_2D(CPUStruct* CPU) {  // 0x2D, and, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = (high * 256) + low;

	CPU->A = CPU->A & read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void AND_3D(CPUStruct* CPU) {  // 0x3D, and, absolute,x
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = low + CPU->X;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = CPU->A & read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void AND_39(CPUStruct* CPU) {  // 0x39, and, absolute,y
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = CPU->A & read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void AND_21(CPUStruct* CPU) {  // 0x21, and, (indirect,x)
	CPU->cycles = 6;

	byte initAddress = read(CPU->PC, CPU);
		CPU->PC++;

	initAddress = initAddress + CPU->X;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = (high * 256) + low;

	CPU->A = CPU->A & read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void AND_31(CPUStruct* CPU) {  // 0x31, and, (indirect),y
	CPU->cycles = 5;

	byte initAddress = read(CPU->PC, CPU);
		CPU->PC++;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = CPU->A & read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}


//============================================== ASL / LSR ==============================================// 
static void ASL_0A(CPUStruct* CPU) { // 0x0A, arithmetic shift left, accumulator
	CPU->cycles = 2;

	CPU->c = (CPU->A > 0x7F);

	CPU->A = (byte)(CPU->A << 1);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0);
}
static void ASL_06(CPUStruct* CPU) { // 0x06, arithmetic shift left, zeropage
	CPU->cycles = 5;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	byte value = read(address, CPU);

	CPU->c = (value > 0x7F);

	value = (byte)(value << 1);
	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0);
}
static void ASL_16(CPUStruct* CPU) { // 0x16, arithmetic shift left, zeropage,x
	CPU->cycles = 6;

	byte address = read(CPU->PC, CPU) + CPU->X;
		CPU->PC++;

	byte value = read(address, CPU);

	CPU->c = (value > 0x7F);

	value = (byte)(value << 1);
	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0);
}
static void ASL_0E(CPUStruct* CPU) { // 0x0E, arithmetic shift left, absolute
	CPU->cycles = 6;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);

	CPU->c = (value > 0x7F);

	value = (byte)(value << 1);
	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0);

}
static void ASL_1E(CPUStruct* CPU) { // 0x1E, arithmetic shift left, absolute,x
	CPU->cycles = 7;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = (high * 256) + low + CPU->X;
	
	byte value = read(address, CPU);
	
	CPU->c = (value > 0x7F);
	
	value = (byte)(value << 1);
	write(address, value, CPU);
	
	CPU->n = (value > 0x7F);
	CPU->z = (value == 0);
}

static void LSR_4A(CPUStruct* CPU) { // 0x4A, logical shift right, accumulator
	CPU->cycles = 2;

	CPU->c = (CPU->A & 1);

	CPU->A = (byte)(CPU->A >> 1);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0);
}
static void LSR_46(CPUStruct* CPU) { // 0x46, logical shift right, zeropage
	CPU->cycles = 5;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	byte value = read(address, CPU);

	CPU->c = (value & 1);

	value = (byte)(value >> 1);
	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0);
}
static void LSR_56(CPUStruct* CPU) { // 0x56, logical shift right, zeropage,x
	CPU->cycles = 6;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	byte value = read(address, CPU);

	CPU->c = (value & 1);

	value = (byte)(value >> 1);
	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0);
}
static void LSR_4E(CPUStruct* CPU) { // 0x4E, logical shift right, absolute
	CPU->cycles = 6;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);

	CPU->c = (value & 1);

	value = (byte)(value >> 1);
	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0);

}
static void LSR_5E(CPUStruct* CPU) { // 0x5E, logical shift right, absolute,x
	CPU->cycles = 7;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low + CPU->X;

	byte value = read(address, CPU);

	CPU->c = (value & 1);

	value = (byte)(value >> 1);
	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0);
}



//================================================ BIT ================================================//
static void BIT_24(CPUStruct* CPU) { // 0x24, bit test, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	byte value = read(address, CPU);

	CPU->n = ((value & 0x80) > 0);
	CPU->z = (0 == value);
	CPU->v = ((value & 0x80) > 0);
}
static void BIT_2C(CPUStruct* CPU) {  // 0x2C, bit test, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);

	CPU->n = ((value & 0x80) > 0);
	CPU->z = (0 == value);
	CPU->v = ((value & 0x80) > 0);
}


//================================================ BRK ================================================//
static void BRK_00(CPUStruct* CPU) { // 0x00, break into interrupt, implied
	CPU->cycles = 7;

	CPU->PC++;
	push((byte)(CPU->PC >> 8), CPU);
	push((byte)CPU->PC, CPU);

	byte flag = CPU->c;
	flag += CPU->z * 2;
	flag += 4;
	flag += CPU->d * 8;
	flag += 0x30;

	flag += CPU->v * 0x40;
	flag += CPU->n * 0x80;

	push(flag, CPU);

	CPU->PC = (ushort)(read(0xFFFE, CPU) + (read(0xFFFF, CPU) * 0x100));
}


//================================================ BRN ================================================//
static void BCC_90(CPUStruct* CPU) { // 0x90, branch if carry clear, Relative
	CPU->cycles = 2;
	int distance = (int)read(CPU->PC, CPU);
	CPU->PC++;

	if (!CPU->c) {
		if (distance > 0x7F) {
			distance -= 256;
		}

		ushort temp = CPU->PC & 0xFF00;
		CPU->PC = (ushort)(CPU->PC + distance);

		if (temp != (CPU->PC & 0xFF00)) {
			CPU->cycles++;
		}

		CPU->cycles++;

	}
}
static void BCS_B0(CPUStruct* CPU) { // 0xB0, branch if carry set, Relative
	CPU->cycles = 2;
	int distance = (int)read(CPU->PC, CPU);
	CPU->PC++;

	if (CPU->c) {
		if (distance > 0x7F) {
			distance -= 256;
		}

		ushort temp = CPU->PC & 0xFF00;
		CPU->PC = (ushort)(CPU->PC + distance);

		if (temp != (CPU->PC & 0xFF00)) {
			CPU->cycles++;
		}

		CPU->cycles++;

	}


}
static void BEQ_F0(CPUStruct* CPU) { // 0xF0, branch if equal, Relative
	CPU->cycles = 2;
	int distance = (int)read(CPU->PC, CPU);
	CPU->PC++;

	if (CPU->z) {
		if (distance > 0x7F) {
			distance -= 256;
		}

		ushort temp = CPU->PC & 0xFF00;
		CPU->PC = (ushort)(CPU->PC + distance);

		if (temp != (CPU->PC & 0xFF00)) {
			CPU->cycles++;
		}

		CPU->cycles++;

	}
}
static void BNE_D0(CPUStruct* CPU) { // 0xD0, branch if not equal, Relative
	CPU->cycles = 2;
	int distance = (int)read(CPU->PC, CPU);
	CPU->PC++;

	if (!CPU->z) {
		if (distance > 0x7F) {
			distance -= 256;
		}

		ushort temp = CPU->PC & 0xFF00;
		CPU->PC = (ushort)(CPU->PC + distance);

		if (temp != (CPU->PC & 0xFF00)) {
			CPU->cycles++;
		}

		CPU->cycles++;

	}
}
static void BMI_30(CPUStruct* CPU) { // 0x30, branch if minus, Relative
	CPU->cycles = 2;
	int distance = (int)read(CPU->PC, CPU);
	CPU->PC++;

	if (CPU->n) {
		if (distance > 0x7F) {
			distance -= 256;
		}

		ushort temp = CPU->PC & 0xFF00;
		CPU->PC = (ushort)(CPU->PC + distance);

		if (temp != (CPU->PC & 0xFF00)) {
			CPU->cycles++;
		}

		CPU->cycles++;

	}
}
static void BPL_10(CPUStruct* CPU) { // 0x10, branch if positive, Relative
	CPU->cycles = 2;
	int distance = (int)read(CPU->PC, CPU);
	CPU->PC++;

	if (!CPU->n) {
		if (distance > 0x7F) {
			distance -= 256;
		}

		ushort temp = CPU->PC & 0xFF00;
		CPU->PC = (ushort)(CPU->PC + distance);

		if (temp != (CPU->PC & 0xFF00)) {
			CPU->cycles++;
		}

		CPU->cycles++;

	}
}
static void BVC_50(CPUStruct* CPU) { // 0x50, branch if overflow clear, Relative
	CPU->cycles = 2;
	int distance = (int)read(CPU->PC, CPU);
	CPU->PC++;

	if (!CPU->v) {
		if (distance > 0x7F) {
			distance -= 256;
		}

		ushort temp = CPU->PC & 0xFF00;
		CPU->PC = (ushort)(CPU->PC + distance);

		if (temp != (CPU->PC & 0xFF00)) {
			CPU->cycles++;
		}

		CPU->cycles++;

	}
}
static void BVS_70(CPUStruct* CPU) { // 0x70, branch if overflow set, Relative
	CPU->cycles = 2;
	int distance = (int)read(CPU->PC, CPU);
	CPU->PC++;

	if (CPU->v) {
		if (distance > 0x7F) {
			distance -= 256;
		}

		ushort temp = CPU->PC & 0xFF00;
		CPU->PC = (ushort)(CPU->PC + distance);

		if (temp != (CPU->PC & 0xFF00)) {
			CPU->cycles++;
		}

		CPU->cycles++;

	}
}

//============================================== CLF / SEF ==============================================// 
static void CLC_18(CPUStruct* CPU) { // 0x18, clear carry flag, implied
	CPU->cycles = 2;
	CPU->c = false;
}
static void CLD_D8(CPUStruct* CPU) { // 0xD8, clear decimal mode, implied
	CPU->cycles = 2;
	CPU->d = false;
}
static void CLI_58(CPUStruct* CPU) { // 0x58, clear interrupt disable, implied
	CPU->cycles = 2;
	CPU->i = false;
}
static void CLV_B8(CPUStruct* CPU) { // 0xB8, clear overflow flag, implied
	CPU->cycles = 2;
	CPU->v = false;
}

static void SEC_38(CPUStruct* CPU) { // 0x38, set carry flag, implied
	CPU->cycles = 2;
	CPU->c = true;
}
static void SED_F8(CPUStruct* CPU) { // 0xF8, set decimal mode, implied
	CPU->cycles = 2;
	CPU->d = true;
}
static void SEI_78(CPUStruct* CPU) { // 0x78, set interrupt disable, implied
	CPU->cycles = 2;
	CPU->i = true;
}


//================================================ CMP ================================================//
static void CMP_C9(CPUStruct* CPU) { // 0xC9, compare with a, immediate
	CPU->cycles = 2;

	byte value = read(CPU->PC, CPU);
	CPU->PC++;

	CPU->n = ((byte)(CPU->A - value) > 0x7F);
	CPU->z = (CPU->A == value);
	CPU->c = (value <= CPU->A);
}
static void CMP_C5(CPUStruct* CPU) { // 0xC5, compare with a, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	byte value = read(address, CPU);

	CPU->n = ((byte)(CPU->A - value) > 0x7F);
	CPU->z = (CPU->A == value);
	CPU->c = (value <= CPU->A);
}
static void CMP_D5(CPUStruct* CPU) { // 0xD5, compare with a, zeropage,x
	CPU->cycles = 4;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	byte value = read(address, CPU);

	CPU->n = ((byte)(CPU->A - value) > 0x7F);
	CPU->z = (CPU->A == value);
	CPU->c = (value <= CPU->A);
}
static void CMP_CD(CPUStruct* CPU) {  // 0xCD, compare with a, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);

	CPU->n = ((byte)(CPU->A - value) > 0x7F);
	CPU->z = (CPU->A == value);
	CPU->c = (value <= CPU->A);
}
static void CMP_DD(CPUStruct* CPU) {  // 0xDD, compare with a, absolute,x
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->X;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	byte value = read(address, CPU);

	CPU->n = ((byte)(CPU->A - value) > 0x7F);
	CPU->z = (CPU->A == value);
	CPU->c = (value <= CPU->A);
}
static void CMP_D9(CPUStruct* CPU) {  // 0xD9, compare with a, absolute,y
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	byte value = read(address, CPU);

	CPU->n = ((byte)(CPU->A - value) > 0x7F);
	CPU->z = (CPU->A == value);
	CPU->c = (value <= CPU->A);
}
static void CMP_C1(CPUStruct* CPU) {  // 0xC1, compare with a, (indirect,x)
	CPU->cycles = 6;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	initAddress = initAddress + CPU->X;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);

	CPU->n = ((byte)(CPU->A - value) > 0x7F);
	CPU->z = (CPU->A == value);
	CPU->c = (value <= CPU->A);
}
static void CMP_D1(CPUStruct* CPU) {  // 0xD1, compare with a, (indirect),y
	CPU->cycles = 5;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	byte value = read(address, CPU);

	CPU->n = ((byte)(CPU->A - value) > 0x7F);
	CPU->z = (CPU->A == value);
	CPU->c = (value <= CPU->A);
}

static void CPX_E0(CPUStruct* CPU) { // 0xE0, compare with x, immediate
	CPU->cycles = 2;

	byte value = read(CPU->PC, CPU);
	CPU->PC++;

	CPU->n = ((byte)(CPU->X - value) > 0x7F);
	CPU->z = (CPU->X == value);
	CPU->c = (value <= CPU->X);
}
static void CPX_E4(CPUStruct* CPU) { // 0xE4, compare with x, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	byte value = read(address, CPU);

	CPU->n = ((byte)(CPU->X - value) > 0x7F);
	CPU->z = (CPU->X == value);
	CPU->c = (value <= CPU->X);
}
static void CPX_EC(CPUStruct* CPU) {  // 0xEC, compare with x, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);

	CPU->n = ((byte)(CPU->X - value) > 0x7F);
	CPU->z = (CPU->X == value);
	CPU->c = (value <= CPU->X);
}

static void CPY_C0(CPUStruct* CPU) { // 0xC0, compare with y, immediate
	CPU->cycles = 2;

	byte value = read(CPU->PC, CPU);
	CPU->PC++;

	CPU->n = ((byte)(CPU->Y - value) > 0x7F);
	CPU->z = (CPU->Y == value);
	CPU->c = (value <= CPU->Y);
}
static void CPY_C4(CPUStruct* CPU) { // 0xE4, compare with y, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	byte value = read(address, CPU);

	CPU->n = ((byte)(CPU->Y - value) > 0x7F);
	CPU->z = (CPU->Y == value);
	CPU->c = (value <= CPU->Y);
}
static void CPY_CC(CPUStruct* CPU) {  // 0xCC, compare with y, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);

	CPU->n = ((byte)(CPU->Y - value) > 0x7F);
	CPU->z = (CPU->Y == value);
	CPU->c = (value <= CPU->Y);
}


//================================================ DEC ================================================//
static void DEX_CA(CPUStruct* CPU) { // 0xCA, decrement x, implied
	CPU->cycles = 2;
	
	CPU->X--;

	CPU->n = (CPU->X > 0x7F);
	CPU->z = (CPU->X == 0x00);
}
static void DEY_88(CPUStruct* CPU) { // 0x88, decrement y, implied
	CPU->cycles = 2;

	CPU->Y--;

	CPU->n = (CPU->Y > 0x7F);
	CPU->z = (CPU->Y == 0x00);
}

static void DEC_C6(CPUStruct* CPU) { // 0xC6, decrement memory, zeropage
	CPU->cycles = 5;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	byte value = read(address, CPU);
		
	value--;

	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);
}
static void DEC_D6(CPUStruct* CPU) { // 0xD6, decrement memory, zeropage,x
	CPU->cycles = 6;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	byte value = read(address, CPU);

	value--;

	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);
}
static void DEC_CE(CPUStruct* CPU) { // 0xCE, decrement memory, absolute
	CPU->cycles = 6;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);

	value--;

	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);
}
static void DEC_DE(CPUStruct* CPU) { // 0xDE, decrement memory, absolute,x
	CPU->cycles = 7;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = (high * 256) + low + CPU->X;

	byte value = read(address, CPU);

	value--;

	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);
}

//================================================ EOR ================================================//
static void EOR_49(CPUStruct* CPU) { // 0x49, eor, immediate
	CPU->cycles = 2;

	CPU->A = CPU->A ^ read(CPU->PC, CPU);
	CPU->PC++;

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void EOR_45(CPUStruct* CPU) { // 0x45, eor, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	CPU->A = CPU->A ^ read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void EOR_55(CPUStruct* CPU) { // 0x55, eor, zeropage,x
	CPU->cycles = 4;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	CPU->A = CPU->A ^ read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void EOR_4D(CPUStruct* CPU) {  // 0x4D, eor, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;

	CPU->A = CPU->A ^ read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void EOR_5D(CPUStruct* CPU) {  // 0x5D, eor, absolute,x
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->X;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = CPU->A ^ read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void EOR_59(CPUStruct* CPU) {  // 0x59, eor, absolute,y
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = CPU->A ^ read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void EOR_41(CPUStruct* CPU) {  // 0x21, eor, (indirect,x)
	CPU->cycles = 6;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	initAddress = initAddress + CPU->X;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = (high * 256) + low;

	CPU->A = CPU->A ^ read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void EOR_51(CPUStruct* CPU) {  // 0x31, eor, (indirect),y
	CPU->cycles = 5;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = CPU->A ^ read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}



//================================================ INC ================================================//
static void INX_E8(CPUStruct* CPU) { // 0xE8, increment x, implied
	CPU->cycles = 2;

	CPU->X++;

	CPU->n = (CPU->X > 0x7F);
	CPU->z = (CPU->X == 0x00);
}
static void INY_C8(CPUStruct* CPU) { // 0xC8, increment y, implied
	CPU->cycles = 2;

	CPU->Y++;

	CPU->n = (CPU->Y > 0x7F);
	CPU->z = (CPU->Y == 0x00);
}

static void INC_E6(CPUStruct* CPU) { // 0xE6, increment memory, zeropage
	CPU->cycles = 5;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	byte value = read(address, CPU);

	value++;

	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);
}
static void INC_F6(CPUStruct* CPU) { // 0xF6, increment memory, zeropage,x
	CPU->cycles = 6;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	byte value = read(address, CPU);

	value++;

	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);
}
static void INC_EE(CPUStruct* CPU) { // 0xEE, increment memory, absolute
	CPU->cycles = 6;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = (high * 256) + low;
	
	byte value = read(address, CPU);

	value++;

	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);
}
static void INC_FE(CPUStruct* CPU) { // 0xFE, increment memory, absolute,x
	CPU->cycles = 7;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = (high * 256) + low + CPU->X;

	byte value = read(address, CPU);

	value++;

	write(address, value, CPU);

	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);
}



//============================================== JSR / RTS ==============================================// 
static void JSR_20(CPUStruct* CPU) { // 0x20, jump to subroutine, absolute
	CPU->cycles = 6;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);

	// push high and low bytes of 
	push(CPU->PC / 256, CPU);
	push(CPU->PC, CPU);


	CPU->PC = (high * 256) + low;
}
static void RTS_60(CPUStruct* CPU) { // 0x60, return from subroutine, implied
	CPU->cycles = 6;

	byte low = pull(CPU);
	byte high = pull(CPU);

	CPU->PC = (ushort)((high * 256) + low);
	CPU->PC++;
}

static void JMP_4C(CPUStruct* CPU) { // 0x4C, jump, absolute
	CPU->cycles = 3;
	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	CPU->PC = (ushort)((high * 256) + low);
}
static void JMP_6C(CPUStruct* CPU) { // 0x6C, jump, indirect
	CPU->cycles = 5;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	//if we want to add CPU->cycles

	ushort address = (high * 256) + low;
	byte l2 = read(address, CPU);
	byte h2 = read(address + 1, CPU);

	CPU->PC = (h2 * 256) + l2;
}


//================================================ LDS ================================================//
static void LDA_A9(CPUStruct * CPU) { // 0xA9, load a, immediate
	CPU->cycles = 2;

	CPU->A = read(CPU->PC, CPU);
	CPU->PC++;
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);

}
static void LDX_A2(CPUStruct * CPU) { // 0xA2, load x, immediate
	CPU->cycles = 2;

	CPU->X = read(CPU->PC, CPU);
	CPU->PC++;
	CPU->n = (CPU->X > 0x7F);
	CPU->z = (CPU->X == 0x00);
}
static void LDY_A0(CPUStruct * CPU) {// 0xA0, load y, 
	CPU->cycles = 2;

	CPU->Y = read(CPU->PC, CPU);
	CPU->PC++;
	CPU->n = (CPU->Y > 0x7F);
	CPU->z = (CPU->Y == 0x00);

}


static void LDA_A5(CPUStruct * CPU) { //0xA5, load a, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
		CPU->PC++;

	CPU->A = read(address, CPU);
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void LDX_A6(CPUStruct * CPU) { //0xA6, load x, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	CPU->X = read(address, CPU);
	CPU->n = (CPU->X > 0x7F);
	CPU->z = (CPU->X == 0x00);
}
static void LDY_A4(CPUStruct * CPU) { //0xA4, load y, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	CPU->Y = read(address, CPU);
	CPU->n = (CPU->Y > 0x7F);
	CPU->z = (CPU->Y == 0x00);
}

static void LDA_B5(CPUStruct * CPU) { //0xA5, load a, zeropage,x
	CPU->cycles = 4;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	CPU->A = read(address, CPU);
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void LDX_B6(CPUStruct * CPU) { //0xB6, load x, zeropage,y
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU) + CPU->Y;
	CPU->PC++;

	CPU->X = read(address, CPU);
	CPU->n = (CPU->X > 0x7F);
	CPU->z = (CPU->X == 0x00);
}
static void LDY_B4(CPUStruct * CPU) { //0xB4, load y, zeropage,x
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	CPU->Y = read(address, CPU);
	CPU->n = (CPU->Y > 0x7F);
	CPU->z = (CPU->Y == 0x00);
}


static void LDA_AD(CPUStruct * CPU) { //0xAD, load a, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = (high * 256) + low;


	CPU->A = read(address, CPU);
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void LDX_AE(CPUStruct * CPU) { //0xAE, load x, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;

	CPU->X = read(address, CPU);
	CPU->n = (CPU->X > 0x7F);
	CPU->z = (CPU->X == 0x00);
}
static void LDY_AC(CPUStruct * CPU) { //0xAC, load y, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;

	CPU->Y = read(address, CPU);
	CPU->n = (CPU->Y > 0x7F);
	CPU->z = (CPU->Y == 0x00);
}

static void LDA_BD(CPUStruct * CPU) { //0xBD, load a, absolute,x
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->X;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = read(address, CPU);
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void LDX_BE(CPUStruct * CPU) { //0xBE, load x, absolute,y
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->X = read(address, CPU);
	CPU->n = (CPU->X > 0x7F);
	CPU->z = (CPU->X == 0x00);
}
static void LDY_BC(CPUStruct * CPU) { //0xBC, load y, absolute,x
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->X;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->Y = read(address, CPU);
	CPU->n = (CPU->Y > 0x7F);
	CPU->z = (CPU->Y == 0x00);
}

static void LDA_B9(CPUStruct * CPU) { //0xB9, load a, absolute,y
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = read(address, CPU);
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
// IT DOESNT FUCKING WRAP THE FUCKING PAGE AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
static void LDA_A1(CPUStruct * CPU) { //0xA1, load a, (indirect,x)
	CPU->cycles = 6;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	initAddress = initAddress + CPU->X;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = (high * 256) + low;

	CPU->A = read(address, CPU);
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);

}
static void LDA_B1(CPUStruct * CPU) { //0xB1, load a, (indirect),y
	CPU->cycles = 5;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = read(address, CPU);
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}


//================================================ NOP ================================================//
static void NOP_EA(CPUStruct* CPU) { // 0xEA, no operation, implied
	CPU->cycles = 2;
}


//================================================ ORA ================================================//
static void ORA_09(CPUStruct* CPU) { // 0x09, ora, immediate
	CPU->cycles = 2;

	CPU->A = CPU->A | read(CPU->PC, CPU);
	CPU->PC++;

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ORA_05(CPUStruct* CPU) { // 0x05, ora, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	CPU->A = CPU->A | read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ORA_15(CPUStruct* CPU) { // 0x15, ora, zeropage,x
	CPU->cycles = 4;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	CPU->A = CPU->A | read(address, CPU);


	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ORA_0D(CPUStruct* CPU) {  // 0x0D, ora, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;

	CPU->A = CPU->A | read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ORA_1D(CPUStruct* CPU) {  // 0x1D, ora, absolute,x
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->X;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = CPU->A | read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ORA_19(CPUStruct* CPU) {  // 0x19, ora, absolute,y
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = CPU->A | read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ORA_01(CPUStruct* CPU) {  // 0x01, ora, (indirect,x)
	CPU->cycles = 6;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	initAddress = initAddress + CPU->X;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = (high * 256) + low;

	CPU->A = CPU->A | read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ORA_11(CPUStruct* CPU) {  // 0x11, ora, (indirect),y
	CPU->cycles = 5;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	CPU->A = CPU->A | read(address, CPU);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}


//============================================== PHN / PLN ==============================================// 
static void PHA_48(CPUStruct* CPU) { // 0x48, push a onto stack, implied
	CPU->cycles = 3;

	push(CPU->A, CPU);
}
static void PLA_68(CPUStruct* CPU) { // 0x68, pull a from stack, implied
	CPU->cycles = 4;

	CPU->A = pull(CPU);
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);

}

static void PHP_08(CPUStruct* CPU) { // 0x08, push processor status onto stack, implied
	CPU->cycles = 3;
	byte flag = CPU->c;
	flag += CPU->z * 2;
	flag += CPU->i * 4;
	flag += CPU->d * 8;
	flag += 0x10;
	flag += 0x20;

	flag += CPU->v * 0x40;
	flag += CPU->n * 0x80;

	push(flag, CPU);
}
static void PLP_28(CPUStruct* CPU) { // 0x28, pull processor status from stack, implied
	CPU->cycles = 4;

	byte flag = pull(CPU);
	CPU->c = (flag & 0x01);
	CPU->z = (flag & 0x02) != 0;
	CPU->i = (flag & 0x04) != 0;
	CPU->d = (flag & 0x08) != 0;
	
	CPU->b = (flag & 0x10) != 0;
	CPU->v = (flag & 0x40) != 0;
	CPU->n = (flag & 0x80) != 0;

}


//============================================== ROL / ROR ==============================================// 
static void ROL_2A(CPUStruct* CPU) { // 0x2A, rotate left, accumulator
	CPU->cycles = 2;
	
	byte c = (CPU->A > 0x7F);
	CPU->A = (byte)((CPU->A << 1) + CPU->c);

	CPU->c = c;
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ROL_26(CPUStruct* CPU) { // 0x26, rotate left, zeropage
	CPU->cycles = 5;

	byte address = read(CPU->PC, CPU);
		CPU->PC++;

	byte value = read(address, CPU);

	byte c = (value > 0x7F);
	value = (byte)((value << 1) + CPU->c);

	CPU->c = c;
	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);

	write(address, value, CPU);
}
static void ROL_36(CPUStruct* CPU) { // 0x36, rotate left, zeropage,x
	CPU->cycles = 6;

	byte address = read(CPU->PC, CPU) + CPU->X;
		CPU->PC++;

	byte value = read(address, CPU);

	byte c = (value > 0x7F);
	value = (byte)((value << 1) + CPU->c);

	CPU->c = c;
	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);

	write(address, value, CPU);
}
static void ROL_2E(CPUStruct* CPU) { // 0x2E, rotate left, absolute
	CPU->cycles = 6;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = (high * 256) + low;
	byte value = read(address, CPU);

	byte c = (value > 0x7F);
	value = (byte)((value << 1) + CPU->c);

	CPU->c = c;
	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);

	write(address, value, CPU);
}
static void ROL_3E(CPUStruct* CPU) { // 0x3E, rotate left, absolute,x
	CPU->cycles = 7;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = (high * 256) + low + CPU->X;
	byte value = read(address, CPU);

	byte c = (value > 0x7F);
	value = (byte)((value << 1) + CPU->c);

	CPU->c = c;
	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);

	write(address, value, CPU);
}

static void ROR_6A(CPUStruct* CPU) { // 0x6A, rotate right, accumulator
	CPU->cycles = 2;

	byte c = (CPU->A & 1);
	CPU->A = (byte)((CPU->A >> 1) + CPU->c * 0x80);

	CPU->c = c;
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void ROR_66(CPUStruct* CPU) { // 0x66, rotate right, zeropage
	CPU->cycles = 5;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	byte value = read(address, CPU);

	byte c = (value & 1);
	value = (byte)((value >> 1) + CPU->c * 0x80);

	CPU->c = c;
	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);

	write(address, value, CPU);
}
static void ROR_76(CPUStruct* CPU) { // 0x76, rotate right, zeropage,x
	CPU->cycles = 6;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	byte value = read(address, CPU);

	byte c = (value & 1);
	value = (byte)((value >> 1) + CPU->c * 0x80);

	CPU->c = c;
	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);

	write(address, value, CPU);
}
static void ROR_6E(CPUStruct* CPU) { // 0x6E, rotate right, absolute
	CPU->cycles = 6;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;
	byte value = read(address, CPU);

	byte c = (value & 1);
	value = (byte)((value >> 1) + CPU->c * 0x80);

	CPU->c = c;
	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);

	write(address, value, CPU);
}
static void ROR_7E(CPUStruct* CPU) { // 0x7E, rotate right, absolute,x
	CPU->cycles = 7;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low + CPU->X;
	byte value = read(address, CPU);

	byte c = (value & 1);
	value = (byte)((value >> 1) + CPU->c * 0x80);

	CPU->c = c;
	CPU->n = (value > 0x7F);
	CPU->z = (value == 0x00);

	write(address, value, CPU);
}

//================================================ RTI ================================================//
static void RTI_40(CPUStruct* CPU) { // 0x40, return from interrupt, implied
	CPU->cycles = 6;

	byte flag = pull(CPU);
	CPU->c = (flag & 0x01);
	CPU->z = (flag & 0x02) != 0;
	CPU->i = (flag & 0x04) != 0;
	CPU->d = (flag & 0x08) != 0;

	CPU->b = (flag & 0x10) != 0;
	CPU->v = (flag & 0x40) != 0;
	CPU->n = (flag & 0x80) != 0;

	byte low = pull(CPU);
	byte high = pull(CPU);

	CPU->PC = (ushort)((high * 256) + low);
}


//================================================ SBC ================================================//
static void SBC_E9(CPUStruct* CPU) { // 0xE9, sub with carry, immediate
	CPU->cycles = 2;

	byte value = read(CPU->PC, CPU);
	int Sum = CPU->A - value - (CPU->c ? 0 : 1);
		CPU->PC++;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = (((CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum >= 0);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void SBC_E5(CPUStruct* CPU) { // 0xE5, sub with carry, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	byte value = read(address, CPU);
	int Sum = CPU->A - value - CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = (((CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum >= 0);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void SBC_F5(CPUStruct* CPU) { // 0xF5, sub with carry, zeropage,x
	CPU->cycles = 4;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	byte value = read(address, CPU);
	int Sum = CPU->A - value - CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = (((CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum >= 0);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void SBC_ED(CPUStruct* CPU) { // 0xED, sub with carry, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);
	int Sum = CPU->A - value - CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = (((CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum >= 0);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void SBC_FD(CPUStruct* CPU) { // 0xFD, sub with carry, absolute,x
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->X;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	byte value = read(address, CPU);
	int Sum = CPU->A - value - CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = (((CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum >= 0);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void SBC_F9(CPUStruct* CPU) { // 0xF9, sub with carry, absolute,y
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	byte value = read(address, CPU);
	int Sum = CPU->A - value - CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = (((CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum >= 0);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void SBC_E1(CPUStruct* CPU) { // 0xE1, sub with carry, (indirect,x)
	CPU->cycles = 6;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	initAddress = initAddress + CPU->X;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = (high * 256) + low;

	byte value = read(address, CPU);
	int Sum = CPU->A - value - CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = (((CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum >= 0);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void SBC_F1(CPUStruct* CPU) { // 0xF1, sub with carry, (indirect),y
	CPU->cycles = 6;

	byte initAddress = read(CPU->PC, CPU);
	CPU->PC++;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = low + CPU->Y;

	if (address > 0xFF) {
		CPU->cycles++;
	}

	address = address + (high * 256);

	byte value = read(address, CPU);
	int Sum = CPU->A - value - CPU->c;


	// essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
	CPU->v = (((CPU->A ^ value) & (CPU->A ^ Sum)) & 0x80) != 0;
	CPU->c = (Sum >= 0);

	CPU->A = (byte)(Sum);

	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}


//================================================ STS ================================================//
static void STA_85(CPUStruct * CPU) { // 0x85, store a, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
		CPU->PC++;

	write(address, CPU->A, CPU);
}
static void STX_86(CPUStruct * CPU) { // 0x86, store x, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	write(address, CPU->X, CPU);
}
static void STY_84(CPUStruct * CPU) { // 0x84, store y, zeropage
	CPU->cycles = 3;

	byte address = read(CPU->PC, CPU);
	CPU->PC++;

	write(address, CPU->Y, CPU);
}

static void STA_95(CPUStruct * CPU) { // 0x95, store a, zeropage,x
	CPU->cycles = 4;

	byte address = read(CPU->PC, CPU) + CPU->X;
		CPU->PC++;

	write(address, CPU->A, CPU);
}
static void STX_96(CPUStruct* CPU) { // 0x96, store x, zeropage,y
	CPU->cycles = 4;

	byte address = read(CPU->PC, CPU) + CPU->Y;
	CPU->PC++;

	write(address, CPU->X, CPU);
}

static void STY_94(CPUStruct* CPU) { // 0x94 store a, zeropage,x
	CPU->cycles = 4;

	byte address = read(CPU->PC, CPU) + CPU->X;
	CPU->PC++;

	write(address, CPU->Y, CPU);
}



static void STA_8D(CPUStruct * CPU) { // 0x8D, store a, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;
	write(address, CPU->A, CPU);
}
static void STX_8E(CPUStruct * CPU) { // 0x8E, store x, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;
	write(address, CPU->X, CPU);
}
static void STY_8C(CPUStruct * CPU) { // 0x8C, store y, absolute
	CPU->cycles = 4;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = (high * 256) + low;
	write(address, CPU->Y, CPU);
}
static void STA_9D(CPUStruct * CPU) { // 0x9D, store a, absolute,x
	CPU->cycles = 5;

	byte low = read(CPU->PC, CPU);
		CPU->PC++;
	byte high = read(CPU->PC, CPU);
		CPU->PC++;

	ushort address = low + CPU->X;

	address = address + (high * 256);
	write(address, CPU->A, CPU);
}
static void STA_99(CPUStruct* CPU) { // 0x99, store a, absolute,y
	CPU->cycles = 5;

	byte low = read(CPU->PC, CPU);
	CPU->PC++;
	byte high = read(CPU->PC, CPU);
	CPU->PC++;

	ushort address = low + CPU->Y;

	address = address + (high * 256);
	write(address, CPU->A, CPU);
}
static void STA_81(CPUStruct * CPU) { // 0x81, store a, (indirect,x)
	CPU->cycles = 6;

	byte initAddress = read(CPU->PC, CPU);
		CPU->PC++;

	initAddress = initAddress + CPU->X;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = (high * 256) + low;
	write(address, CPU->A, CPU);
}
static void STA_91(CPUStruct * CPU) { // 0x91, store a, (indirect),y
	CPU->cycles = 6;

	byte initAddress = read(CPU->PC, CPU);
		CPU->PC++;

	byte low = read(initAddress, CPU);
	byte high = read(initAddress + 1, CPU);

	ushort address = low + CPU->Y;

	address = address + (high * 256);
	write(address, CPU->A, CPU);
}

//================================================ TRN ================================================//

static void TAX_AA(CPUStruct* CPU) { // 0xAA, transfer a to x, implied
	CPU->cycles = 2;
	CPU->X = CPU->A;
	CPU->n = (CPU->X > 0x7F);
	CPU->z = (CPU->X == 0x00);
}
static void TAY_A8(CPUStruct* CPU) { // 0xA8, transfer a to y, implied
	CPU->cycles = 2;
	CPU->Y = CPU->A;
	CPU->n = (CPU->Y > 0x7F);
	CPU->z = (CPU->Y == 0x00);
}
static void TSX_BA(CPUStruct* CPU) { // 0xBA, transfer sp to x, implied
	CPU->cycles = 2;
	CPU->X = CPU->SP;
	CPU->n = (CPU->X > 0x7F);
	CPU->z = (CPU->X == 0x00);
}
static void TXA_8A(CPUStruct* CPU) { // 0x8A, transfer x to a, implied
	CPU->cycles = 2;
	CPU->A = CPU->X;
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}
static void TXS_9A(CPUStruct* CPU) { // 0x9A, transfer x to sp, implied
	CPU->cycles = 2;
	CPU->SP = CPU->X;
}
static void TYA_98(CPUStruct* CPU) { // 0x98, transfer y to a, implied
	CPU->cycles = 2;
	CPU->A = CPU->Y;
	CPU->n = (CPU->A > 0x7F);
	CPU->z = (CPU->A == 0x00);
}














//================================================ HLT =================================================//
static void HLT_02(CPUStruct * CPU) {

	CPU->halt = true;
}

static void __er__(CPUStruct * CPU) {
	printf("UKNOWN OPCODE\n");
	int ProgramCounter = CPU->PC - 1;
	printf("Mode: 0x%02X, Program counter: 0x%X\n", read(ProgramCounter, CPU), ProgramCounter);

	printf("BREAKPOINT HERE\n");
	exit(1);

}
const OperationFunc Operations[256] = {
	/*TABLE	   $0x00,   $0x01,   $0x02,   $0x03,   $0x04,   $0x05,   $0x06,   $0x07,   $0x08,   $0x09,   $0x0A,   $0x0B,   $0x0C,   $0x0D,   $0x0E,   $0x0F,*/
	/*0x00*/  BRK_00,  ORA_01,  HLT_02,  __er__,  __er__,  ORA_05,  ASL_06,  __er__,  PHP_08,  ORA_09,  ASL_0A,  __er__,  __er__,  ORA_0D,  ASL_0E,  __er__,
	/*0x10*/  BPL_10,  ORA_11,  __er__,  __er__,  __er__,  ORA_15,  ASL_16,  __er__,  CLC_18,  ORA_19,  __er__,  __er__,  __er__,  ORA_1D,  ASL_1E,  __er__,
	/*0x20*/  JSR_20,  AND_21,  __er__,  __er__,  BIT_24,  AND_25,  ROL_26,  __er__,  PLP_28,  AND_29,  ROL_2A,  __er__,  BIT_2C,  AND_2D,  ROL_2E,  __er__,
	/*0x30*/  BMI_30,  AND_31,  __er__,  __er__,  __er__,  AND_35,  ROL_36,  __er__,  SEC_38,  AND_39,  __er__,  __er__,  __er__,  AND_3D,  ROL_3E,  __er__,
	/*0x40*/  RTI_40,  EOR_41,  __er__,  __er__,  __er__,  EOR_45,  LSR_46,  __er__,  PHA_48,  EOR_49,  LSR_4A,  __er__,  JMP_4C,  EOR_4D,  LSR_4E,  __er__,
	/*0x50*/  BVC_50,  EOR_51,  __er__,  __er__,  __er__,  EOR_55,  LSR_56,  __er__,  CLI_58,  EOR_59,  __er__,  __er__,  __er__,  EOR_5D,  LSR_5E,  __er__,
	/*0x60*/  RTS_60,  ADC_61,  __er__,  __er__,  __er__,  ADC_65,  ROR_66,  __er__,  PLA_68,  ADC_69,  ROR_6A,  __er__,  JMP_6C,  ADC_6D,  ROR_6E,  __er__,
	/*0x70*/  BVS_70,  ADC_71,  __er__,  __er__,  __er__,  ADC_75,  ROR_76,  __er__,  SEI_78,  ADC_79,  __er__,  __er__,  __er__,  ADC_7D,  ROR_7E,  __er__,
	/*0x80*/  __er__,  STA_81,  __er__,  __er__,  STY_84,  STA_85,  STX_86,  __er__,  DEY_88,  __er__,  TXA_8A,  __er__,  STY_8C,  STA_8D,  STX_8E,  __er__,
	/*0x90*/  BCC_90,  STA_91,  __er__,  __er__,  STY_94,  STA_95,  STX_96,  __er__,  TYA_98,  STA_99,  TXS_9A,  __er__,  __er__,  STA_9D,  __er__,  __er__,
	/*0xA0*/  LDY_A0,  LDA_A1,  LDX_A2,  __er__,  LDY_A4,  LDA_A5,  LDX_A6,  __er__,  TAY_A8,  LDA_A9,  TAX_AA,  __er__,  LDY_AC,  LDA_AD,  LDX_AE,  __er__,
	/*0xB0*/  BCS_B0,  LDA_B1,  __er__,  __er__,  LDY_B4,  LDA_B5,  LDX_B6,  __er__,  CLV_B8,  LDA_B9,  TSX_BA,  __er__,  LDY_BC,  LDA_BD,  LDX_BE,  __er__,
	/*0xC0*/  CPY_C0,  CMP_C1,  __er__,  __er__,  CPY_C4,  CMP_C5,  DEC_C6,  __er__,  INY_C8,  CMP_C9,  DEX_CA,  __er__,  CPY_CC,  CMP_CD,  DEC_CE,  __er__,
	/*0xD0*/  BNE_D0,  CMP_D1,  __er__,  __er__,  __er__,  CMP_D5,  DEC_D6,  __er__,  CLD_D8,  CMP_D9,  __er__,  __er__,  __er__,  CMP_DD,  DEC_DE,  __er__,
	/*0xE0*/  CPX_E0,  SBC_E1,  __er__,  __er__,  CPX_E4,  SBC_E5,  INC_E6,  __er__,  INX_E8,  SBC_E9,  NOP_EA,  __er__,  CPX_EC,  SBC_ED,  INC_EE,  __er__,
	/*0xF0*/  BEQ_F0,  SBC_F1,  __er__,  __er__,  __er__,  SBC_F5,  INC_F6,  __er__,  SED_F8,  SBC_F9,  __er__,  __er__,  __er__,  SBC_FD,  INC_FE,  __er__,
};