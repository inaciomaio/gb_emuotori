#include "cpu.h"
#include "memory.h"
#include <stdio.h>

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

void cpu_step(CPU *cpu) {
	uint8_t opcode = mem_read(cpu->pc);
	printf("PC=0x%04X || OPCODE=0x%^02X", cpu->pc, opcode);

	switch (opcode) {
	
		case 0x00: // no operation
			printf("  -> NOP\n");
			cpu->pc += 1;
			break;

		case 0x01:
			
			break;

		case 0x02:
			break;

		case 0x03:
			break;
			
		case 0x04:
			break;
			
		case 0x05:
			break;
			
		case 0x06: //LD B, n8
			printf("	-> LOAD INTO B THE VALUE n8");
			uint8_t n = mem_read(cpu->pc + 1);
			cpu->b = n;
			cpu->pc += 2;
			break;

		case 0x07:
			break;

		case 0x08:
			break;

		case 0x09:
			break;

		case 0x0A:
			break;
			
		case 0xC3: { // jump
			uint16_t lo = mem_read(cpu->pc + 1);
			uint16_t hi = mem_read(cpu->pc + 2);
			uint16_t addr = lo | (hi << 8);

			printf("  -> JUMP 0x%04X\n", addr);
			cpu->pc = addr;

			break;
		}

		case 0x31: { // load stack pointer
			uint16_t lo = mem_read(cpu->pc + 1);
			uint16_t hi = mem_read(cpu->pc + 2);
			cpu->sp = lo | (hi << 8);
			printf("  -> LD SP, 0x%04X\n", cpu->sp);
			cpu->pc += 3;
			break;
		}

		case 0x21: {  // LD HL, nn
			uint16_t lo = mem_read(cpu->pc + 1);
		    uint16_t hi = mem_read(cpu->pc + 2);
		    cpu->l = lo;
		    cpu->h = hi;
		    printf("  -> LD HL, 0x%02X%02X\n", cpu->h, cpu->l);
		    cpu->pc += 3;
		    break;
		}

		case 0xF3: // DI
			// TODO!
			printf("  -> DI\n");
			cpu->pc += 1;
			break;

		case 0xAF: // XOR A
			printf("  -> XOR A\n");
			cpu->a ^= cpu->a;
			cpu->f = 0x80;
			cpu->pc += 1;
			break;

		case 0xEA: {
			uint16_t lo = mem_read(cpu->pc + 1);
			uint16_t hi = mem_read(cpu->pc + 2);
			uint16_t addr = lo | (hi << 8);
			mem_write(addr, cpu->a);
			cpu->pc += 3;
			break;
		}

		case 0xFA: { // LD A, (16a)
			uint16_t lo = mem_read(cpu->pc + 1);
			uint16_t hi = mem_read(cpu->pc + 2);		
			uint16_t addr = lo | (hi << 8);
			cpu->a = mem_read(addr);
			cpu->pc +=3;
			break;
		}

			
		default:
			printf("  !! Unknown opcode: 0x%02X\n", opcode);
			exit(1);
	}
}
