#include "wires.h"

void base_tick(){
    if(wires.rst){
        next_wires.base_val = 0;
    }else if(wires.base_we){
        next_wires.base_val = wires.system_bus;
    }
}