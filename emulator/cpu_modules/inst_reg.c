#include "wires.h"

void inst_reg_tick(){
    if(clk && !prev_clk){
        if(ir_read){
            ir_value = (uint8_t)system_bus;
        }
    }
}