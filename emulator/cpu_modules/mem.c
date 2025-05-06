#include "wires.h"

uint8_t ram[65536];
uint8_t rom[4096];

void mem_tick(){
    if(mem_transfer && !io_mem_dir){
        if(mem_addr > 4095){
            if(eight_bit){
                system_bus = (uint16_t)ram[mem_addr];
            }else{
                system_bus = ((uint16_t)ram[mem_addr]) | (((uint16_t)ram[mem_addr+1])<<8);
            }
        }else{
            if(eight_bit){
                system_bus = (uint16_t)rom[mem_addr];
            }else{
                system_bus = ((uint16_t)rom[mem_addr]) | (((uint16_t)rom[mem_addr+1])<<8);
            }
        }
    }

    if(clk && !prev_clk){
        if(mem_transfer && io_mem_dir && mem_addr > 4095){
            if(eight_bit){
                ram[mem_addr] = (uint8_t)system_bus;
            }else{
                ram[mem_addr] = (uint8_t)system_bus;
                ram[mem_addr+1] = (uint8_t)(system_bus>>8);
            }
        }
    }
}