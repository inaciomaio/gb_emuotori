#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "cpu.h"

// we define the maximum size for the input ROM (2MB)
#define ROM_MAX_SIZE 0x200000

// main entry point for the program
int main (int argc, char *argv[]) {

	// user did not provide filename, print how to use and exit program
	if(argc < 2) {
		printf("Usage: %s <romfile.gb>\n", argv[0]);
		return 1;
	}

	// allocate the 2MB as memory for the ROM
	// rom points to a chunk of RAM to load the file from
	uint8_t *rom = malloc(ROM_MAX_SIZE);
	if(!rom) {
		fprintf(stderr, "Failed to allocate memory.\n");
		return 1;
	}

	// open ROM file in read binary mode (rb)
	FILE *f = fopen(argv[1], "rb");
	if(!f) {
		perror("Error opening ROM file");
		free(rom);
		return 1;
	}

	size_t rom_size = fread(rom, 1, ROM_MAX_SIZE, f);
	fclose(f);
	printf("Loaded ROM: %s (%zu bytes)\n", argv[1], rom_size);

	// === SETUP CPU ===
	CPU cpu;
	cpu_reset(&cpu);

	printf("CPU reset --- PC = 0x%04X\n", cpu.pc);

	// Fetch first opcode @ 0x0100 (after bios startup)
	if(cpu.pc >= rom_size) {
		fprintf(stderr, "PC points beyond ROM size!\n");
		free(rom);
		return 1;
	}

	uint8_t opcode = rom[cpu.pc];
	printf("First opcode @0x0100 = 0x%02X\n", opcode);

	// start execution loop

	int cycles = 0;

	while(cpu.pc < rom_size && cycles < 10) {
		opcode = rom[cpu.pc];
		printf("PC=0x%04X  OPCODE=0x%02X\n", cpu.pc, opcode);

		switch(opcode) {
			case 0x00: // NOP (NO OPERATION)
				printf("	-> NOP\n");
				cpu.pc += 1;
				break;

			case 0xC3: {
				uint16_t addr = rom[cpu.pc + 1] | (rom[cpu.pc + 2] << 8);
				printf("	-> JP 0x%04X\n", addr);
				cpu.pc = addr;
				break;
			}

			case 0xF3:  // DI
			    printf("    -> DI (disable interrupts)\n");
				// TODO: Implement IME flag (interrupt master enable)
				cpu.pc += 1;
				break;

			case 0xAF: // XOR A
				printf("	-> XOR A\n");
				cpu.a ^= cpu.a;
				cpu.f = 0x80;
				cpu.pc += 1;
				break;

			default:
				printf("	!! Unknown opcode : 0x%02X\n", opcode);
				return 1;
		}

		cycles++;
	}

	free(rom);
	return 0;
}
