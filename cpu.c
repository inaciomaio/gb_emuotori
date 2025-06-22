#include "cpu.h"

void cpu_reset(CPU *cpu) {
	cpu->pc = 0x0100;
	cpu->sp = 0xFFFE;

	cpu->a = 0x01;
	cpu->f = 0xB0;
	cpu->b = 0x00;
	cpu->c = 0x13;
	cpu->d = 0x00;
	cpu->e = 0xD8;
	cpu->h = 0x01;
	cpu->l = 0x4D;
}
