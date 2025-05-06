#include "wires.h"
#include "stdio.h"

int CLOCK_INTERVAL = 2;

int clock_tick_count = 0;

bool clock_running = 0;

void clock_tick(){
    if (clock_stop && !prev_clock_stop){
        clock_running = 0;
        clock_tick_count = 0;
    }
    if (clock_start && !prev_clock_start){
        clock_running = 1;
        clock_tick_count = 0;
    }

    if(clock_running || (clock_step && !prev_clock_step)){
        clock_tick_count++;
        if(clock_tick_count >= CLOCK_INTERVAL){
            clock_tick_count = 0;
            prev_clk = clk;
            clk = !clk;
            if(clk) puts("CLK ↑");
            else puts("CLK ↓");
        }
    }
}