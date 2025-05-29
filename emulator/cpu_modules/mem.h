#pragma once
#include <stdint.h>

extern uint8_t ram[65536];
extern uint8_t rom[4096];

void mem_tick();
void mem_between_ticks();