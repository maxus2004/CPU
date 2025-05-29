void ip_tick(){
    if(wires.b_we){
        next_wires.b_val = wires.system_bus;
    }
}
void ip_between_ticks(){
    if(wires.b_oe){
        next_wires.system_bus = wires.b_val;
    }
}