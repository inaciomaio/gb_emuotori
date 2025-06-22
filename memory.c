#include "memory.h"

static uint8_t *rom = NULL;
static size_t rom_size = 0;

static uint8_t vram[0x2000]; // 8KB video ram
static uint8_t eram[0x2000]; // 8KB external ram
static uint8_t wram[0x2000]; // 8KB work ram
static uint8_t oam [0x00A0]; // sprite attributes memory
static uint8_t hram[0x007F]; // high(?) ram
static uint8_t io[0x0080];   // I/O registers
static uint8_t ie_reg = 0;	 // interrupt enable register

void mem_attach_rom(uint8_t *rom_ptr, size_t rom_len) {
	rom = rom_ptr;
	rom_size = rom_len;
}

#define FROM(buf, base) (buf[(addr) - (base)])

uint8_t mem_read(uint16_t addr) {
	if(addr <= 0x7FFF)
		return(addr < rom_size) ? rom[addr] : 0xFF;

	else if(addr <= 0x9FFF) return FROM(vram, 0x8000);
	else if(addr <= 0xBFFF) return FROM(eram, 0xA000);
	else if(addr <= 0xDFFF) return FROM(wram, 0xC000);
	else if(addr <= 0xFDFF) return FROM(wram, 0xE000);
	else if(addr <= 0xFE9F) return FROM(oam, 0xFE00);
	else if(addr <= 0xFEFF) return 0x00;
	else if(addr <= 0xFF7F) return FROM(io, 0xFF00);
	else if(addr <= 0xFFFE) return FROM(hram, 0xFF80);
	else return ie_reg;
	
}

void mem_write(uint16_t addr, uint8_t value) {
	if(addr <= 0x7FFF) { /* this is the rom, do nothing */ }
	else if(addr <= 0x9FFF) vram[addr - 0x8000] = value;
	else if(addr <= 0xBFFF) vram[addr - 0xA000] = value;
	else if(addr <= 0xDFFF) vram[addr - 0xC000] = value;
	else if(addr <= 0XFDFF) vram[addr - 0xE000] = value;
	else if(addr <= 0xFE9F) vram[addr - 0xFE00] = value;
	else if(addr <= 0xFEFF) { /* unusable */ }
	else if(addr <= 0XFF7F) vram[addr - 0xFF00] = value;
	else if(addr <= 0xFFFE) vram[addr - 0xFF80] = value;
	else ie_reg = value;
}
