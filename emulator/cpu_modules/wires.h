#include <stdbool.h>
#include <stdint.h>

extern bool mem_addr_write;
extern bool mem_transfer;
extern bool io_addr_write;
extern bool io_transfer;
extern bool io_mem_dir;
extern bool eight_bit;
extern bool reg_read;
extern bool reg_write;
extern bool reg_shr;
extern bool reg_shl;
extern bool flags_store;
extern bool alu_carry;
extern bool alu_mode;
extern bool alu_op0;
extern bool alu_op1;
extern bool alu_op2;
extern bool alu_op3;
extern bool ip_inc;
extern bool ip_write;
extern bool ip_read;
extern bool ir_read;
extern bool count_reset;
extern bool clock_stop, prev_clock_stop;
extern bool clock_start, prev_clock_start;
extern bool clock_step, prev_clock_step;

extern bool alu_zf, alu_sf, alu_cf;
extern bool flags_zf, flags_sf, flags_cf;
extern uint16_t alu_result;
extern uint16_t reg_value;
extern uint8_t count_value;
extern uint8_t ir_value;
extern uint8_t io_addr;
extern uint16_t mem_addr;

extern uint16_t system_bus;
extern bool clk, prev_clk;