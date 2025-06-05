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

void generate_one_argument_op(uint8_t opcode, uint8_t flags_mask, uint8_t flags, uint32_t *before_read_uops, int before_read_uops_count, uint32_t *after_read_uops, int after_read_uops_count){
    opcode = opcode<<3;

    //before argument read
    for(int src = 0b000; src <= 0b101; src++){
        for(int i = 0; i < before_read_uops_count; i++){
            add_uop(
                opcode|src, 2+i, flags_mask, flags, 
                before_read_uops[i]
            );
        }
    }

    //argument read
    // lit
    add_uop(opcode|0b000, 2+before_read_uops_count, flags_mask, flags, IP_OE | BASE_WE | OFFSET_CLR | IP_CNT);
    // reg
    // chilling 😎👌
    // [lit]
    add_uop(opcode|0b010, 2+before_read_uops_count, flags_mask, flags, IP_OE | BASE_WE | OFFSET_CLR | IP_CNT);
    add_uop(opcode|0b010, 3+before_read_uops_count, flags_mask, flags, MEM_OE | BASE_WE | OFFSET_CLR);
    // [reg]
    add_uop(opcode|0b011, 2+before_read_uops_count, flags_mask, flags, OE_IR0 | BASE_WE | OFFSET_CLR);
    // [r+l]
    add_uop(opcode|0b100, 2+before_read_uops_count, flags_mask, flags, IP_OE | BASE_WE | OFFSET_CLR | IP_CNT);
    add_uop(opcode|0b100, 3+before_read_uops_count, flags_mask, flags, MEM_OE | OFFSET_WE);
    add_uop(opcode|0b100, 4+before_read_uops_count, flags_mask, flags, OE_IR0 | BASE_WE);
    // [r+r]
    add_uop(opcode|0b101, 2+before_read_uops_count, flags_mask, flags, OE_IR4 | OFFSET_WE);
    add_uop(opcode|0b101, 3+before_read_uops_count, flags_mask, flags, OE_IR0 | BASE_WE);

    //after argument read
    int after_read_uop_start[6] = {
        2+before_read_uops_count+1,
        2+before_read_uops_count+0,
        2+before_read_uops_count+2,
        2+before_read_uops_count+1,
        2+before_read_uops_count+3,
        2+before_read_uops_count+2,
    };
    uint32_t data_read_signal[6] = {MEM_OE, OE_IR0, MEM_OE, MEM_OE, MEM_OE, MEM_OE};
        
    for(int src = 0b000; src <= 0b101; src++){
        for(int i = 0; i < after_read_uops_count; i++){
            add_uop(
                opcode|src, after_read_uop_start[src]+i, flags_mask, flags, 
                (i==0)?data_read_signal[src]:NONE | after_read_uops[i] | (i==after_read_uops_count-1)?CNT_CLR:NONE
            );
        }
    }
}

void generate_no_argument_ops(){
    //STOP
    uint32_t stop_uops[] = {STOP};
    generate_no_argument_op(0b00000000, NONE, NONE, stop_uops, 1);
    //RETURN
    uint32_t ret_uops[] = {
        SP_WE | BP_WE,                  //  ] MOV SP, BP
        SP_OE | BASE_WE | OFFSET_CLR,   //  ┐
        MEM_OE | BP_WE | SP_CNT,        //  ┘ POP BP
        SP_OE | BASE_WE | OFFSET_CLR,   //  ┐
        MEM_OE | IP_WE | SP_CNT         //  ┘ POP IP

    };
    generate_no_argument_op(0b00000001, NONE, NONE, ret_uops, 5);
    //SHR A
    uint32_t shr_a_uops[] = {A_SHR};
    generate_no_argument_op(0b00000010, NONE, NONE, shr_a_uops, 1);
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
    generate_one_argument_op(0b00001, NONE, NONE, NULL, 0, and_uops, 1);
    // OR
    uint32_t or_uops[] = {A_ALU | ALU_M | ALU_S3 | ALU_S2 | ALU_S1};
    generate_one_argument_op(0b00010, NONE, NONE, NULL, 0, or_uops, 1);
    // XOR
    uint32_t xor_uops[] = {A_ALU | ALU_M | ALU_S2 | ALU_S1};
    generate_one_argument_op(0b00011, NONE, NONE, NULL, 0, xor_uops, 1);
    // NAND
    uint32_t nand_uops[] = {A_ALU | ALU_M | ALU_S2};
    generate_one_argument_op(0b00100, NONE, NONE, NULL, 0, nand_uops, 1);
    // NOR
    uint32_t nor_uops[] = {A_ALU | ALU_M | ALU_S0};
    generate_one_argument_op(0b00101, NONE, NONE, NULL, 0, nor_uops, 1);
    // XNOR
    uint32_t xnor_uops[] = {A_ALU | ALU_M | ALU_S3 | ALU_S0};
    generate_one_argument_op(0b00110, NONE, NONE, NULL, 0, xnor_uops, 1);
    // ADD
    uint32_t add_uops[] = {FLAGS_WE | A_ALU | ALU_S3 | ALU_S0};
    generate_one_argument_op(0b00111, NONE, NONE, NULL, 0, add_uops, 1);
    // SUB
    uint32_t sub_uops[] = {FLAGS_WE | A_ALU | ALU_S2 | ALU_S1 | CIN};
    generate_one_argument_op(0b01000, NONE, NONE, NULL, 0, sub_uops, 1);
    // ADDC
    uint32_t addc1_uops[] = {FLAGS_WE | A_ALU | ALU_S3 | ALU_S0};
    generate_one_argument_op(0b01001, CF, NONE, NULL, 0, addc1_uops, 1);
    uint32_t addc2_uops[] = {FLAGS_WE | A_ALU | ALU_S3 | ALU_S0 | CIN};
    generate_one_argument_op(0b01001, CF, CF, NULL, 0, addc2_uops, 1);
    // SUBC
    uint32_t subc0_uops[] = {FLAGS_WE | A_ALU | ALU_S2 | ALU_S1};
    generate_one_argument_op(0b01010, CF, NONE, NULL, 0, subc0_uops, 1);
    uint32_t subc1_uops[] = {FLAGS_WE | A_ALU | ALU_S2 | ALU_S1 | CIN};
    generate_one_argument_op(0b01010, CF, CF, NULL, 0, subc1_uops, 1);
    // CMP
    uint32_t cmp_uops[] = {FLAGS_WE | ALU_S2 | ALU_S1 | CIN};
    generate_one_argument_op(0b01011, NONE, NONE, NULL, 0, cmp_uops, 1);
    // JMP
    uint32_t jmp_uops[] = {IP_WE};
    generate_one_argument_op(0b01100, NONE, NONE, NULL, 0, jmp_uops, 1);
    // JZ/JE
    uint32_t jz0_uops[] = {NONE};
    generate_one_argument_op(0b01101, ZF, NONE, NULL, 0, jz0_uops, 1);
    uint32_t jz1_uops[] = {IP_WE};
    generate_one_argument_op(0b01101, ZF, ZF, NULL, 0, jz1_uops, 1);
    // JNZ/JNE
    uint32_t jnz0_uops[] = {NONE};
    generate_one_argument_op(0b01110, ZF, ZF, NULL, 0, jnz0_uops, 1);
    uint32_t jnz1_uops[] = {IP_WE};
    generate_one_argument_op(0b01110, ZF, NONE, NULL, 0, jnz1_uops, 1);
    // JC/JB/JNAE
    uint32_t jc0_uops[] = {NONE};
    generate_one_argument_op(0b01111, CF, NONE, NULL, 0, jc0_uops, 1);
    uint32_t jc1_uops[] = {IP_WE};
    generate_one_argument_op(0b01111, CF, CF, NULL, 0, jc1_uops, 1);
    // JNC/JNB/JAE
    uint32_t jnc0_uops[] = {NONE};
    generate_one_argument_op(0b11000, CF, CF, NULL, 0, jnc0_uops, 1);
    uint32_t jnc1_uops[] = {IP_WE};
    generate_one_argument_op(0b11000, CF, NONE, NULL, 0, jnc1_uops, 1);
    // JS/JL/JNGE
    uint32_t js0_uops[] = {NONE};
    generate_one_argument_op(0b11001, SF, NONE, NULL, 0, js0_uops, 1);
    uint32_t js1_uops[] = {IP_WE};
    generate_one_argument_op(0b11001, SF, SF, NULL, 0, js1_uops, 1);
    // JNS/JNL/JGE
    uint32_t jns0_uops[] = {NONE};
    generate_one_argument_op(0b11010, SF, SF, NULL, 0, jns0_uops, 1);
    uint32_t jns1_uops[] = {IP_WE};
    generate_one_argument_op(0b11010, SF, NONE, NULL, 0, jns1_uops, 1);
    // JG/JNLE
    uint32_t jg0_uops[] = {NONE};
    generate_one_argument_op(0b11011, NONE, NONE, NULL, 0, jg0_uops, 1);
    uint32_t jg1_uops[] = {IP_WE};
    generate_one_argument_op(0b11011, SF|ZF, NONE, NULL, 0, jg1_uops, 1);
    // JNG/JLE
    uint32_t jng0_uops[] = {NONE};
    generate_one_argument_op(0b11100, SF|ZF, NONE, NULL, 0, jng0_uops, 1);
    uint32_t jng1_uops[] = {IP_WE};
    generate_one_argument_op(0b11100, NONE, NONE, NULL, 0, jng1_uops, 1);
    // CALL
    uint32_t call_uops_before_arg[] = {
        SP_CNT | SP_DOWN,               //  ┐
        SP_OE | BASE_WE | OFFSET_CLR,   //  │ PUSH IP
        MEM_WE | IP_OE                  //  ┘
    };
    uint32_t call_uops_after_arg[] = {
        IP_WE,                          //  ] MOV IP, <addr>
        SP_CNT | SP_DOWN,               //  ┐
        SP_OE | BASE_WE | OFFSET_CLR,   //  │ PUSH BP
        MEM_WE | BP_OE,                 //  ┘
        BP_WE | SP_OE,                  //  ] MOV BP, SP
    };
    generate_one_argument_op(0b11101, NONE, NONE, call_uops_before_arg, 3, call_uops_after_arg, 5);
    // PUSH
    uint32_t push_uops[] = {
        SP_CNT | SP_DOWN,
        SP_OE | BASE_WE | OFFSET_CLR,
        MEM_WE | OE_IR0
    };
    generate_no_argument_op(0b11110001, NONE, NONE, push_uops, 3);
    // POP
    uint32_t pop_uops[] = {
        SP_OE | BASE_WE | OFFSET_CLR,
        MEM_OE | WE_IR4 | SP_CNT
    };
    generate_no_argument_op(0b11111001, NONE, NONE, pop_uops, 2);
}

void generate_mov(){
    //MOV reg,lit / MOV reg,reg / MOV reg,[lit] / MOV reg,[reg] / MOV reg,[r+l]
    uint32_t mov_to_reg_uops[] = {WE_IR4};
    generate_one_argument_op(0b10001, NONE, NONE, NULL, 0, mov_to_reg_uops, 1);
    //MOV reg,[r+r]
    for(uint8_t dest_reg = 0;dest_reg<16;dest_reg++){
        uint32_t mov_to_reg2_uops[] = {
            OFFSET_WE | OE_IR0,
            BASE_WE | OE_IR4,
            WE | (dest_reg << WS_0to4) | MEM_OE
        }; 
        generate_no_argument_op((0b1010<<4)|dest_reg, NONE, NONE, mov_to_reg2_uops, 3);
    };
    //MOV [lit],reg
    uint32_t mov_from_reg0_uops[] = {
        IP_OE | BASE_WE | OFFSET_CLR | IP_CNT,
        MEM_OE | BASE_WE | OFFSET_CLR,
        MEM_WE | OE_IR0
    }; 
    generate_no_argument_op(0b10011010, NONE, NONE, mov_from_reg0_uops, 3);
    //MOV [reg],reg
    uint32_t mov_from_reg1_uops[] = {
        OE_IR4 | BASE_WE | OFFSET_CLR,
        MEM_WE | OE_IR0
    }; 
    generate_no_argument_op(0b10011011, NONE, NONE, mov_from_reg1_uops, 2);
    //MOV [r+l],reg
    uint32_t mov_from_reg2_uops[] = {
        IP_OE | BASE_WE | OFFSET_CLR | IP_CNT,
        MEM_OE | OFFSET_WE,
        OE_IR4 | BASE_WE,
        MEM_WE | OE_IR0
    }; 
    generate_no_argument_op(0b10011100, NONE, NONE, mov_from_reg2_uops, 4);
    //MOV [r+r],reg
    for(uint8_t base_reg = 0;base_reg<16;base_reg++){
        uint32_t mov_from_reg3_uops[] = {
            OFFSET_WE | OE_IR4,
            BASE_WE | OE | (base_reg << OS_0to4),
            MEM_WE | OE_IR0
        }; 
        generate_no_argument_op((0b1011<<4)|base_reg, NONE, NONE, mov_from_reg3_uops, 3);
    };
}

int main(){
    generate_fetch();
    generate_no_argument_ops();
    generate_one_argument_ops();
    generate_mov();

    //TODO: write microcode to files
}
