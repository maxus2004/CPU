#include "wires.h"

uint32_t microcode_rom[65536];

void control_between_ticks(){
    uint16_t addr = (wires.ir_val) | 
                    (wires.count_value<<8) | 
                    (((uint16_t)wires.flags_zf)<<12) | 
                    (((uint16_t)wires.flags_sf)<<13) | 
                    (((uint16_t)wires.flags_cf)<<14);
    
    uint32_t control_word = microcode_rom[addr];

    next_wires.a_alu     = (control_word & (1<<0))  != 0;
    next_wires.a_alu     = (control_word & (1<<1))  != 0;
    next_wires.a_alu     = (control_word & (1<<2))  != 0;
    next_wires.a_alu     = (control_word & (1<<3))  != 0;
    next_wires.a_alu     = (control_word & (1<<4))  != 0;
    next_wires.a_alu     = (control_word & (1<<5))  != 0;
    next_wires.a_alu     = (control_word & (1<<6))  != 0;
    next_wires.a_alu     = (control_word & (1<<7))  != 0;
    next_wires.a_alu     = (control_word & (1<<8))  != 0;
    next_wires.a_alu     = (control_word & (1<<9))  != 0;
    next_wires.a_alu     = (control_word & (1<<10)) != 0;
    next_wires.a_alu     = (control_word & (1<<11)) != 0;
    next_wires.a_alu     = (control_word & (1<<12)) != 0;
    next_wires.a_alu     = (control_word & (1<<13)) != 0;
    next_wires.a_alu     = (control_word & (1<<14)) != 0;
    next_wires.a_alu     = (control_word & (1<<15)) != 0;
    next_wires.a_alu     = (control_word & (1<<16)) != 0;
    next_wires.a_alu     = (control_word & (1<<17)) != 0;
    next_wires.a_alu     = (control_word & (1<<18)) != 0;
    next_wires.a_alu     = (control_word & (1<<19)) != 0;
    next_wires.a_alu     = (control_word & (1<<20)) != 0;
    next_wires.a_alu     = (control_word & (1<<21)) != 0;
    next_wires.a_alu     = (control_word & (1<<22)) != 0;
}