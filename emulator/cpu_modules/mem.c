#include "wires.h"

uint16_t ram[32768];
uint16_t rom[2048];

void mem_tick(){
    uint16_t address = wires.base_val+wires.offset_val;
    if(wires.mem_we){
        if(address < 2048){
            rom[address] = wires.system_bus;
        }else if(address < 2048+256){
            wires.io_addr = address;
            wires.io_data = wires.system_bus;
        }else if(address >= 32768){
            ram[address%32768] = wires.system_bus;
        }
    }
}

void mem_between_ticks(){
    uint16_t address = wires.base_val+wires.offset_val;
    if(wires.mem_oe){
        if(address < 2048){
            next_wires.system_bus = rom[address];
        }else if(address < 2048+256){
            next_wires.io_addr = address;
            next_wires.system_bus = wires.io_data;
        }else if(address >= 32768){
            next_wires.system_bus = ram[address%32768];
        }
    }
}