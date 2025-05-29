#include "wires.h"

void ip_tick(){
    if(wires.rst){
        next_wires.ip_val = 0;
    }else if(wires.ip_we){
        next_wires.ip_val = wires.system_bus;
    }else if(wires.ip_inc){
        next_wires.ip_val = wires.ip_val+1;
    }
}

void ip_between_ticks(){
    if(wires.ip_oe){
        next_wires.system_bus = wires.ip_val;
    }
}