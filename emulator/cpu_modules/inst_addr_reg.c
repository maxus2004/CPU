#include "wires.h"

uint16_t inst_addr_value;

void inst_addr_reg_tick(){
    if(ip_read){
        system_bus = inst_addr_value;
    }

    if(clk && !prev_clk){
        if(ip_write){
            inst_addr_value = system_bus;
        }
        if(ip_inc){
            inst_addr_value++;
        }
    }
}