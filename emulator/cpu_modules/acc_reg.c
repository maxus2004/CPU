#include "wires.h"

uint16_t acc_reg_value;

void acc_reg_tick(){
    if(reg_read){
        system_bus = acc_reg_value;
    }

    if(clk && !prev_clk){
        if(reg_shl && reg_shr && reg_write){
            acc_reg_value = alu_result;
        }
        else if(reg_shl && reg_shr && !reg_write){
            acc_reg_value = system_bus;
        }
        else if(!reg_shl && reg_shr){
            acc_reg_value = acc_reg_value>>1;
        }
        else if(reg_shl && !reg_shr){
            acc_reg_value = acc_reg_value<<1;
        }
    }
}