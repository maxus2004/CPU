#include "wires.h"

void a_tick(){
    if(wires.rst){
        next_wires.a_val = 0;
    }if(wires.a_shl && wires.a_shr && wires.a_alu){
        next_wires.a_val = wires.alu_result;
    }
    else if(wires.a_shl && wires.a_shr && !wires.a_alu){
        next_wires.a_val = wires.system_bus;
    }
    else if(!wires.a_shl && wires.a_shr){
        next_wires.a_val = (wires.a_val>>1) | (wires.alu_carry<<15);
    }
    else if(wires.a_shl && !wires.a_shr){
        next_wires.a_val = (wires.a_val<<1) | (wires.alu_carry);
    }
}

void a_between_ticks(){
    if(wires.a_oe){
        next_wires.system_bus = wires.a_val;
    }
}