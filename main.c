#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "cpu.h"
#include "memory.h"

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
	mem_attach_rom(rom, rom_size);
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

	// start execution loop
	int cycles = 0;

	while(cpu.pc < 0x10000 && cycles < 10) {
			cpu_step(&cpu);
			cycles++;
	}

	free(rom);
	return 0;
}
