#include "wires.h"

void io_addr_reg_tick(){
    if(clk && !prev_clk){
        if(io_addr_write){
            io_addr = (uint8_t)system_bus;
        }
    }
}