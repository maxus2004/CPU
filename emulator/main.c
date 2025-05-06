#include <stdio.h>
#include <stdbool.h>
#include "acc_reg.h"
#include "alu.h"
#include "clock.h"
#include "count.h"
#include "flags_reg.h"
#include "inst_addr_reg.h"
#include "inst_reg.h"
#include "io_addr_reg.h"
#include "io_port.h"
#include "mem_addr_reg.h"
#include "mem.h"
#include "microcode_rom.h"
#include "wires.h"

typedef void (*void_func)();

uint64_t simulation_tick = 0;

void load_roms(){
    FILE *microcode_file = fopen("microcode.bin", "rb");
    fread(microcode_rom, sizeof(microcode_rom), 1, microcode_file);
    fclose(microcode_file);

    FILE *rom_file = fopen("rom.bin", "rb");
    fread(rom, sizeof(rom), 1, rom_file);
    fclose(rom_file);
}

void print_wires(){
    printf("mem_addr_write: %i\n",mem_addr_write);
    printf("mem_transfer: %i\n",mem_transfer);
    printf("io_addr_write: %i\n",io_addr_write);
    printf("io_transfer: %i\n",io_transfer);
    printf("io_mem_dir: %i\n",io_mem_dir);
    printf("eight_bit: %i\n",eight_bit);
    printf("reg_read: %i\n",reg_read);
    printf("reg_write: %i\n",reg_write);
    printf("reg_shr: %i\n",reg_shr);
    printf("reg_shl: %i\n",reg_shl);
    printf("flags_store: %i\n",flags_store);
    printf("alu_carry: %i\n",alu_carry);
    printf("alu_mode: %i\n",alu_mode);
    printf("alu_op0: %i\n",alu_op0);
    printf("alu_op1: %i\n",alu_op1);
    printf("alu_op2: %i\n",alu_op2);
    printf("alu_op3: %i\n",alu_op3);
    printf("ip_inc: %i\n",ip_inc);
    printf("ip_write: %i\n",ip_write);
    printf("ip_read: %i\n",ip_read);
    printf("ir_read: %i\n",ir_read);
    printf("count_reset: %i\n",count_reset);
    printf("clock_stop: %i\n",clock_stop);
    printf("clock_start: %i\n",clock_start);
    printf("clock_step: %i\n",clock_step);
    printf("alu_zf: %i\n",alu_zf);
    printf("alu_sf: %i\n",alu_sf);
    printf("alu_cf: %i\n",alu_cf);
    printf("flags_zf: %i\n",flags_zf);
    printf("flags_sf: %i\n",flags_sf);
    printf("flags_cf: %i\n",flags_cf);
    printf("alu_result: %i\n",alu_result);
    printf("acc_reg_value: %i\n",acc_reg_value);
    printf("count_value: %i\n",count_value);
    printf("ir_value: %i\n",ir_value);
    printf("io_addr: %i\n",io_addr);
    printf("mem_addr: %i\n",mem_addr);
    printf("inst_addr_value: %i\n",inst_addr_value);
    printf("io_data: %i\n",io_data);
    printf("system_bus: %i\n",system_bus);
    printf("clock_running: %i\n",clock_running);
    printf("clk: %i\n",clk);
    printf("simulation_tick: %lld\n",simulation_tick);
}

int main(){
    void_func component_ticks[] = {
        acc_reg_tick, 
        alu_tick, 
        clock_tick, 
        count_tick, 
        flags_reg_tick, 
        inst_addr_reg_tick,
        inst_reg_tick,
        io_addr_reg_tick,
        io_port_tick,
        mem_addr_tick,
        mem_tick,
        microcode_rom_tick
    };
    int components_count = sizeof(component_ticks)/sizeof(void_func);

    load_roms();

    while (true){
        for(int i = 0;i<components_count;i++){
            component_ticks[i]();
        }
        print_wires();
        puts("press for next simulation tick");
        getchar();
        simulation_tick++;
    }
}
