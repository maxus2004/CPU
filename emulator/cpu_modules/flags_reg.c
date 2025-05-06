#include "wires.h"

void flags_reg_tick(){
    if(clk && !prev_clk){
        if(flags_store){
            flags_zf = alu_zf;
            flags_cf = alu_cf;
            flags_sf = alu_sf;
        }
    }
}