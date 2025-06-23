#ifndef CPU_H
#define CPU_H

#include <stdint.h>

// simulation of Gameboy CPU (Sharp LR35902)

typedef struct {
	// pc = program counter, holds the next memory address of the next instruction to execute
	uint16_t pc;

	uint16_t sp;

	uint8_t a, f;
	uint8_t b, c;
	uint8_t d, e;
	uint8_t h, l;

	
} CPU;

#define HL(cpu) ((uint16_t)(((cpu)->h << 8 | (cpu)->1))

void cpu_reset(CPU *cpu);
void cpu_step(CPU *cpu);

#endif
