#include "wires.h"

void flags_tick(){
    if(wires.flags_we){
        next_wires.flags_zf = wires.alu_zf;
        next_wires.flags_cf = wires.alu_cf;
        next_wires.flags_sf = wires.alu_sf;
    }
}
