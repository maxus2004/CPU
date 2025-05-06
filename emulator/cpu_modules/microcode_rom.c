#include "wires.h"

uint32_t microcode_rom[65536];

void microcode_rom_tick(){
    if(!clk && prev_clk){
        uint16_t addr = ((uint16_t)ir_value) | 
                        (((uint16_t)count_value)<<8) | 
                        (((uint16_t)flags_zf)<<12) | 
                        (((uint16_t)flags_sf)<<13) | 
                        (((uint16_t)flags_cf)<<14);
        
        uint32_t control_word = microcode_rom[addr];

        mem_addr_write  = (control_word & (1<<0))  != 0;
        mem_transfer    = (control_word & (1<<1))  != 0;
        io_addr_write   = (control_word & (1<<2))  != 0;
        io_transfer     = (control_word & (1<<3))  != 0;
        io_mem_dir      = (control_word & (1<<4))  != 0;
        eight_bit       = (control_word & (1<<5))  != 0;
        reg_read        = (control_word & (1<<6))  != 0;
        reg_write       = (control_word & (1<<7))  != 0;
        reg_shr         = (control_word & (1<<8))  != 0;
        reg_shl         = (control_word & (1<<9))  != 0;
        flags_store     = (control_word & (1<<10)) != 0;
        alu_carry       = (control_word & (1<<11)) != 0;
        alu_mode        = (control_word & (1<<12)) != 0;
        alu_op0         = (control_word & (1<<13)) != 0;
        alu_op1         = (control_word & (1<<14)) != 0;
        alu_op2         = (control_word & (1<<15)) != 0;
        alu_op3         = (control_word & (1<<16)) != 0;
        ip_inc          = (control_word & (1<<17)) != 0;
        ip_write        = (control_word & (1<<18)) != 0;
        ip_read         = (control_word & (1<<19)) != 0;
        ir_read         = (control_word & (1<<20)) != 0;
        count_reset     = (control_word & (1<<21)) != 0;
        clock_stop      = (control_word & (1<<22)) != 0;
    }
}
