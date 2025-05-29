#include "wires.h"

void ir_tick(){
    if(wires.ir_we){
        next_wires.ir_val = wires.system_bus;
    }
}
