#include "wires.h"

void mem_addr_tick(){
    if(clk && !prev_clk){
        if(mem_addr_write){
            mem_addr = system_bus;
        }
    }
}