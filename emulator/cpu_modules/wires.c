#include "wires.h"

bool mem_addr_write;
bool mem_transfer;
bool io_addr_write;
bool io_transfer;
bool io_mem_dir;
bool eight_bit;
bool reg_read;
bool reg_write;
bool reg_shr;
bool reg_shl;
bool flags_store;
bool alu_carry;
bool alu_mode;
bool alu_op0;
bool alu_op1;
bool alu_op2;
bool alu_op3;
bool ip_inc;
bool ip_write;
bool ip_read;
bool ir_read;
bool count_reset;
bool clock_stop, prev_clock_stop;
bool clock_start, prev_clock_start;
bool clock_step, prev_clock_step;

bool alu_zf, alu_sf, alu_cf;
bool flags_zf, flags_sf, flags_cf;
uint16_t alu_result;
uint8_t count_value;
uint8_t ir_value;
uint8_t io_addr;
uint16_t mem_addr;

uint16_t system_bus;
bool clk, prev_clk;