#include "main.h"

u16 pc = 0;

u16 r0 = 0;
u16 r1 = 0;
u16 r2 = 0;
u16 r3 = 0;

u8* emu_ram = NULL;
u8* emu_systemRam = NULL;
u8* emu_programRam = NULL;
u8* emu_gpRam = NULL;

void emu_log(const CString errorLevel, const CString msg) {
	printf("%s : %s\n", errorLevel, msg);
}

void debug_dump() {
	printf("debug | pc: %i | r0: %i | r1: %i | r2: %i | r3: %i\n", pc, r0, r1, r2, r3);
}

u32 readFile(const CString filename, u8* buffer, u32 maxSize) {
	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		return -1;
	}
	u32 result = fread(buffer, sizeof(u8), maxSize, f);
	fclose(f);
	return result;
}

u16 getShort(u8* buffer) {
	u16 value = 0;
	memcpy(&value, buffer, sizeof(u16));
	return value;
}

void setShort(u8* buffer, u16 value) {
	memcpy(buffer, &value, sizeof(u16));
}

u16 readRegister(u32 reg) {
	switch (reg) {
		case 0: return r0;
		case 1: return r1;
		case 2: return r2;
		case 3: return r3;
		default: return 0;
	}
}

void writeRegister(u32 reg, u16 val) {
	switch (reg) {
		case 0: r0 = val; break;
		case 1: r1 = val; break;
		case 2: r2 = val; break;
		case 3: r3 = val; break;
		default: return;
	}
}

void emu_init() {
	pc = SYSTEM_RAM_SIZE; // start at program start, in the future the system will have a bootloader
	emu_ram = (u8*) malloc(sizeof(u8) * MEMORY_SIZE);
	emu_systemRam = emu_ram; // size 128bytes
	emu_programRam = emu_ram + SYSTEM_RAM_SIZE;
	emu_gpRam = emu_ram + SYSTEM_RAM_SIZE + PROGRAM_RAM_SIZE; // size 786bytes
}

bool emu_step() {
	printf("step | pc: %i | ram[pc]: %i \n", pc, emu_ram[pc]);
	switch (emu_ram[pc]) {
		case INS_INVALID:
			emu_log("error", "invalid instruction");
			return false;
		case INS_NOOP:
			pc++;
			return true;
		case INS_HALT:
			return false;
		case INS_BREAK:
			debug_dump();
			return false;
		case INS_MOV:
			writeRegister(emu_ram[pc + 3], getShort(&emu_ram[pc + 1]));
			pc += 4;
			return true;
		case INS_MOVR:
			writeRegister(emu_ram[pc + 2], readRegister(emu_ram[pc + 1]));
			pc += 3;
			return true;
		case INS_ADD:
			writeRegister(emu_ram[pc + 1], readRegister(emu_ram[pc + 1]) + readRegister(emu_ram[pc + 2]));
			pc += 3;
			return true;
		case INS_SUB:
			writeRegister(emu_ram[pc + 1], readRegister(emu_ram[pc + 1]) - readRegister(emu_ram[pc + 2]));
			pc += 3;
			return true;
		default:
			emu_log("error", "invalid instruction");
			return false;
	}
}

void emu_dumpPhysicalMemory() {
	FILE* f = fopen("memory.bin", "w");
	fwrite(emu_ram, sizeof(u8), MEMORY_SIZE, f);
	fclose(f);
}

void emu_destory() {
	if (emu_ram != NULL) {
		free(emu_ram);
		emu_systemRam = NULL;
		emu_programRam = NULL;
		emu_gpRam = NULL;
	}
}

int main(int argc, char** argv) {
	if (argc != 2) {
		emu_log("error", "usage : emu [binary]");
		return -1;
	}
	emu_init();
	u32 programSize = readFile(argv[1], emu_programRam, PROGRAM_RAM_SIZE);
	if (programSize == -1) {
		emu_log("error", "failed to load input file");
		return -1;
	}
	while (emu_step()) {};
	emu_dumpPhysicalMemory();
	emu_destory();
	return 0;
}