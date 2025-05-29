#pragma once
#include "common.h"

typedef struct {
    bool a_oe;
    bool a_shl;
    bool a_shr;
    bool a_alu;
    uint16_t a_val;

    bool alu_carry;
    bool alu_op0;
    bool alu_op1;
    bool alu_op2;
    bool alu_op3;
    bool alu_mode;
    bool alu_zf;
    bool alu_sf;
    bool alu_cf;
    uint16_t alu_result;

    bool flags_we;
    bool flags_zf;
    bool flags_sf;
    bool flags_cf;

    bool ip_val;
    bool ip_we;
    bool ip_oe;
    bool ip_inc;

    bool ir_we;
    uint16_t ir_val;

    bool base_we;
    uint16_t base_val;

    bool offset_we;
    bool offset_clr;
    uint16_t offset_val;

    bool mem_we;
    bool mem_oe;
    uint16_t io_data;
    uint16_t io_addr;
    bool io_we;
    bool io_oe;

    bool count_reset;
    uint8_t count_value;

    uint16_t system_bus;
    bool rst;
}wires_t;

extern wires_t wires;
extern wires_t next_wires;