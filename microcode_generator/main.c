#include <stdio.h>
#include <stdint.h>
#include "signals.h"

uint32_t ucode[65536] = {0};

void add_uop(uint8_t opcode, uint8_t cnt, uint8_t flags_mask, uint8_t flags, uint32_t uop){
    for(uint8_t f = 0b000; f<=0b111; f++){
        if((flags&flags_mask) != (f&flags_mask)) continue;
        ucode[((uint16_t)opcode) | (((uint16_t)cnt)<<8) | (((uint16_t)f)<<12)] = uop;
    }
}

void generate_fetch(){
    for(int opcode = 0;opcode<256;opcode++){
        add_uop(opcode, 0, NONE, NONE, IP_OE | BASE_WE | OFFSET_CLR);
        add_uop(opcode, 1, NONE, NONE, MEM_OE | IR_WE | IP_CNT);
    }
}

void generate_no_argument_op(uint8_t opcode, uint8_t flags_mask, uint8_t flags, uint32_t *uops, int uops_count){
    for(int i = 0; i < uops_count; i++){
        add_uop(
            opcode, 2+i, flags_mask, flags, 
            uops[i] | (i==uops_count-1)?CNT_CLR:NONE
        );
    }
}

void generate_one_argument_op(uint8_t opcode, uint8_t flags_mask, uint8_t flags, uint32_t *uops, int uops_count, int uop_with_data){
    // lit
    add_uop(opcode|0b000, 2, flags_mask, flags, IP_OE | BASE_WE | OFFSET_CLR | IP_CNT);
    for(int i = 0; i < uops_count; i++){
        add_uop(
            opcode|0b000, 3+i, flags_mask, flags, 
            (uop_with_data==i)?MEM_OE:NONE | uops[i] | (i==uops_count-1)?CNT_CLR:NONE
        );
    }
    // reg
    for(int i = 0; i < uops_count; i++){
        add_uop(
            opcode|0b001, 2+i, flags_mask, flags,
            (uop_with_data==i)?OE_IR0:NONE | uops[i] | (i==uops_count-1)?CNT_CLR:NONE
        );
    }
    // [lit]
    add_uop(opcode|0b010, 2, flags_mask, flags, IP_OE | BASE_WE | OFFSET_CLR | IP_CNT);
    add_uop(opcode|0b010, 3, flags_mask, flags, MEM_OE | BASE_WE | OFFSET_CLR);
    for(int i = 0; i < uops_count; i++){
        add_uop(
            opcode|0b010, 4+i, flags_mask, flags, 
            (uop_with_data==i)?MEM_OE:NONE | uops[i] | (i==uops_count-1)?CNT_CLR:NONE
        );
    }
    // [reg]
    add_uop(opcode|0b011, 2, flags_mask, flags, OE_IR0 | BASE_WE | OFFSET_CLR);
    for(int i = 0; i < uops_count; i++){
        add_uop(
            opcode|0b011, 3+i, flags_mask, flags, 
            (uop_with_data==i)?MEM_OE:NONE | uops[i] | (i==uops_count-1)?CNT_CLR:NONE
        );
    }
    // [r+l]
    add_uop(opcode|0b100, 2, flags_mask, flags, IP_OE | BASE_WE | OFFSET_CLR | IP_CNT);
    add_uop(opcode|0b100, 3, flags_mask, flags, MEM_OE | OFFSET_WE);
    add_uop(opcode|0b100, 4, flags_mask, flags, OE_IR0 | BASE_WE);
    for(int i = 0; i < uops_count; i++){
        add_uop(
            opcode|0b100, 5+i, flags_mask, flags, 
            (uop_with_data==i)?MEM_OE:NONE | uops[i] | (i==uops_count-1)?CNT_CLR:NONE
        );
    }
    // [r+r]
    add_uop(opcode|0b101, 2, flags_mask, flags, OE_IR4 | OFFSET_WE);
    add_uop(opcode|0b101, 3, flags_mask, flags, OE_IR0 | BASE_WE);
    for(int i = 0; i < uops_count; i++){
        add_uop(
            opcode|0b101, 4+i, flags_mask, flags, 
            (uop_with_data==i)?MEM_OE:NONE | uops[i] | (i==uops_count-1)?CNT_CLR:NONE
        );
    }
}

void generate_no_argument_ops(){
    //STOP
    uint32_t stop_uops[] = {STOP};
    generate_no_argument_op(0b00000000, NONE, NONE, stop_uops, 1);
    //RETURN
    //TODO: return instruction
    //SHR A
    uint32_t shr_a_uops[] = {A_SHR};
    generate_no_argument_op(0b00000001, NONE, NONE, shr_a_uops, 1);
    //SHL A
    uint32_t shl_a_uops[] = {A_SHL};
    generate_no_argument_op(0b00000011, NONE, NONE, shl_a_uops, 1);
    //INC A
    uint32_t inc_a_uops[] = {A_ALU | CIN};
    generate_no_argument_op(0b00000100, NONE, NONE, inc_a_uops, 1);
    //INC C
    uint32_t inc_c_uops[] = {C_CNT};
    generate_no_argument_op(0b00000101, NONE, NONE, inc_c_uops, 1);
    //DEC A
    uint32_t dec_a_uops[] = {A_ALU | ALU_S3 | ALU_S2 | ALU_S1 | ALU_S0};
    generate_no_argument_op(0b00000110, NONE, NONE, dec_a_uops, 1);
    //DEC C
    uint32_t dec_c_uops[] = {C_CNT | C_DOWN};
    generate_no_argument_op(0b00000111, NONE, NONE, dec_c_uops, 1);
}

void generate_one_argument_ops(){
    // AND
    uint32_t and_uops[] = {A_ALU | ALU_M | ALU_S3 | ALU_S1 | ALU_S0};
    generate_one_argument_op(0b00001<<3, NONE, NONE, and_uops, 1, 0);
    // OR
    uint32_t or_uops[] = {A_ALU | ALU_M | ALU_S3 | ALU_S2 | ALU_S1};
    generate_one_argument_op(0b00010<<3, NONE, NONE, or_uops, 1, 0);
    // XOR
    uint32_t xor_uops[] = {A_ALU | ALU_M | ALU_S2 | ALU_S1};
    generate_one_argument_op(0b00011<<3, NONE, NONE, xor_uops, 1, 0);
    // NAND
    uint32_t nand_uops[] = {A_ALU | ALU_M | ALU_S2};
    generate_one_argument_op(0b00100<<3, NONE, NONE, nand_uops, 1, 0);
    // NOR
    uint32_t nor_uops[] = {A_ALU | ALU_M | ALU_S0};
    generate_one_argument_op(0b00101<<3, NONE, NONE, nor_uops, 1, 0);
    // XNOR
    uint32_t xnor_uops[] = {A_ALU | ALU_M | ALU_S3 | ALU_S0};
    generate_one_argument_oo(0b00110<<3, NONE, NONE, xnor_uops, 1, 0);
    // ADD
    uint32_t add_uops[] = {FLAGS_WE | A_ALU | ALU_S3 | ALU_S0};
    generate_one_argument_op(0b00111<<3, NONE, NONE, add_uops, 1, 0);
    // SUB
    uint32_t sub_uops[] = {FLAGS_WE | A_ALU | ALU_S2 | ALU_S1 | CIN};
    generate_one_argument_op(0b01000<<3, NONE, NONE, sub_uops, 1, 0);
    // ADDC
    uint32_t addc1_uops[] = {FLAGS_WE | A_ALU | ALU_S3 | ALU_S0};
    generate_one_argument_op(0b01001<<3, CF, NONE, addc1_uops, 1, 0);
    uint32_t addc2_uops[] = {FLAGS_WE | A_ALU | ALU_S3 | ALU_S0 | CIN};
    generate_one_argument_op(0b01001<<3, CF, CF, addc2_uops, 1, 0);
    // SUBC
    uint32_t subc0_uops[] = {FLAGS_WE | A_ALU | ALU_S2 | ALU_S1};
    generate_one_argument_op(0b01010<<3, CF, NONE, subc0_uops, 1, 0);
    uint32_t subc1_uops[] = {FLAGS_WE | A_ALU | ALU_S2 | ALU_S1 | CIN};
    generate_one_argument_op(0b01010<<3, CF, CF, subc1_uops, 1, 0);
    // CMP
    uint32_t cmp_uops[] = {FLAGS_WE | ALU_S2 | ALU_S1 | CIN};
    generate_one_argument_op(0b01011<<3, NONE, NONE, cmp_uops, 1, 0);
    // JMP
    uint32_t jmp_uops[] = {IP_WE};
    generate_one_argument_op(0b01100<<3, NONE, NONE, jmp_uops, 1, 0);
    // JZ/JE
    uint32_t jz0_uops[] = {NONE};
    generate_one_argument_op(0b01101<<3, ZF, NONE, jz0_uops, 1, 0);
    uint32_t jz1_uops[] = {IP_WE};
    generate_one_argument_op(0b01101<<3, ZF, ZF, jz1_uops, 1, 0);
    // JNZ/JNE
    uint32_t jz0_uops[] = {NONE};
    generate_one_argument_op(0b01101<<3, ZF, ZF, jz0_uops, 1, 0);
    uint32_t jz1_uops[] = {IP_WE};
    generate_one_argument_op(0b01101<<3, ZF, NONE, jz1_uops, 1, 0);
    // JC/JB/JNAE
    uint32_t jz0_uops[] = {NONE};
    generate_one_argument_op(0b01110<<3, CF, NONE, jz0_uops, 1, 0);
    uint32_t jz1_uops[] = {IP_WE};
    generate_one_argument_op(0b01110<<3, CF, CF, jz1_uops, 1, 0);
    // JNC/JNB/JAE
    uint32_t jz0_uops[] = {NONE};
    generate_one_argument_op(0b01110<<3, CF, CF, jz0_uops, 1, 0);
    uint32_t jz1_uops[] = {IP_WE};
    generate_one_argument_op(0b01110<<3, CF, NONE, jz1_uops, 1, 0);
    // JS/JL/JNGE
    uint32_t jz0_uops[] = {NONE};
    generate_one_argument_op(0b01110<<3, SF, NONE, jz0_uops, 1, 0);
    uint32_t jz1_uops[] = {IP_WE};
    generate_one_argument_op(0b01110<<3, SF, SF, jz1_uops, 1, 0);
    // JNS/JNL/JGE
    uint32_t jz0_uops[] = {NONE};
    generate_one_argument_op(0b01110<<3, SF, SF, jz0_uops, 1, 0);
    uint32_t jz1_uops[] = {IP_WE};
    generate_one_argument_op(0b01110<<3, SF, NONE, jz1_uops, 1, 0);
    // JG/JNLE
    uint32_t jz0_uops[] = {NONE};
    generate_one_argument_op(0b01110<<3, NONE, NONE, jz0_uops, 1, 0);
    uint32_t jz1_uops[] = {IP_WE};
    generate_one_argument_op(0b01110<<3, SF|ZF, NONE, jz1_uops, 1, 0);
    // JNG/JLE
    uint32_t jz0_uops[] = {NONE};
    generate_one_argument_op(0b01110<<3, SF|ZF, NONE, jz0_uops, 1, 0);
    uint32_t jz1_uops[] = {IP_WE};
    generate_one_argument_op(0b01110<<3, NONE, NONE, jz1_uops, 1, 0);
    // CALL
    //TODO: CALL instruction
    // PUSH
    uint32_t push_ops[] = {
        SP_CNT | SP_DOWN,
        SP_OE | BASE_WE | OFFSET_CLR,
        MEM_WE | OE_IR0
    };
    generate_no_argument_op(0b11110001, NONE, NONE, push_ops, 3);
    // POP
    uint32_t pop_ops[] = {
        SP_OE | BASE_WE | OFFSET_CLR,
        MEM_OE | WE_IR4 | SP_CNT
    };
    generate_no_argument_op(0b11110001, NONE, NONE, pop_ops, 2);
}

void generate_mov(){
    //TODO: mov instruction
}

int main(){
    generate_fetch();
    generate_no_argument_ops();
    generate_one_argument_ops();
    generate_mov();

    //TODO: write microcode to files
}
