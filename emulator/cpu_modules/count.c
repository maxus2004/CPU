#include "wires.h"

void count_antitick(){
    if(wires.count_reset){
        next_wires.count_value = 0;
    }else{
        next_wires.count_value = (wires.count_value+1)%16;
    }
}
