#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

// give rom buffer to memory system
void mem_attach_rom (uint8_t *rom_ptr, size_t rom_len);

// read & write byte (64KB limit)
uint8_t mem_read (uint16_t addr);
void mem_write(uint16_t addr, uint8_t value);

#endif
