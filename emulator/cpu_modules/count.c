#include "wires.h"

void count_tick(){
    if(clk && !prev_clk){
        if(count_reset){
            count_value = 0;
        }else{
            count_value = (count_value+1)%16;
        }
    }
}