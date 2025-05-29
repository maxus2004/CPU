#include "wires.h"

void offset_tick(){
    if(wires.offset_clr){
        next_wires.offset_val = 0;
    }else if(!wires.offset_we){
        next_wires.offset_val = wires.system_bus;
    }
}
