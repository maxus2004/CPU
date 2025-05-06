#include "wires.h"

uint16_t io_data = 0;

void io_port_tick(){
    if(io_transfer && io_mem_dir){
        system_bus = io_data;
    }void microcode_rom_tick();

    if(clk && !prev_clk){
        if(io_transfer && !io_mem_dir){
            io_data = system_bus;
        }
    }
}