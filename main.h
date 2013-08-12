#ifndef MAIN_H
#define MAIN_H

#include "emu_types.h"

// I was going to go for the -nostdlib but that's a whole other project for another day
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SYSTEM_RAM_SIZE 128
#define PROGRAM_RAM_SIZE 256
#define MEMORY_SIZE 1024

enum instructionTypes {
	INS_INVALID = 0x00,
	INS_NOOP = 0x01,
	INS_HALT = 0x02,
	INS_BREAK = 0x03,

	INS_MOV = 0x10,
	INS_MOVR = 0x11,

	INS_ADD = 0x20,
	INS_SUB = 0x21
};

void emu_log(const CString errorLevel, const CString msg);

void emu_init();
bool emu_step();
void emu_destroy();

#endif