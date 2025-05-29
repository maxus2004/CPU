#include "wires.h"

void alu_between_ticks(){
    int32_t f;
    uint16_t a = wires.a_val;
    uint16_t b = wires.system_bus;
    bool c = wires.alu_carry;
    uint8_t alu_op = (wires.alu_op0) | (wires.alu_op1<<1) | (wires.alu_op2<<2) | (wires.alu_op3<3);

    if(wires.alu_mode){
        switch (alu_op){
            case  0: f = ~a;       break;
            case  1: f = ~(a & b); break;
            case  2: f = ~a | b;   break;
            case  3: f = 1;        break;
            case  4: f = ~(a | b); break;
            case  5: f = ~b;       break;
            case  6: f = ~(a ^ b); break;
            case  7: f = a | ~b;   break;
            case  8: f = ~a & b;   break;
            case  9: f = a ^ b;    break;
            case 10: f = b;        break;
            case 11: f = a | b;    break;
            case 12: f = 0;        break;
            case 13: f = a & ~b;   break;
            case 14: f = a & b;    break;
            case 15: f = a;        break;
        }
    }else{
        switch (alu_op){
            case  0: f = a - 1 + c;              break;
            case  1: f = (a & b) - 1 + c;        break;
            case  2: f = (a & ~b) - 1 + c;       break;
            case  3: f = -1 + c;                 break;
            case  4: f = a + (a | ~b) + c;       break;
            case  5: f = (a & b) + (a | ~b) + c; break;
            case  6: f = a - b - 1 + c;          break;
            case  7: f = (a | ~b) + c;           break;
            case  8: f = a + (a | b) + c;        break;
            case  9: f = a + b + c;              break;
            case 10: f = (a & ~b) + (a | b) + c; break;
            case 11: f = (a | b) + c;            break;
            case 12: f = a + a + c;              break;
            case 13: f = (a & b) + a + c;        break;
            case 14: f = (a & ~b) + a + c;       break;
            case 15: f = a + c;                  break;
        }
    }

    next_wires.alu_result = (uint16_t)f;
    next_wires.alu_zf = next_wires.alu_result == 0;
    next_wires.alu_sf = (int16_t)next_wires.alu_result < 0;
    next_wires.alu_cf = next_wires.alu_result != f;
}