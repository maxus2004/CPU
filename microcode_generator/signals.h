#define CONTINUE    1<<0    //set microoperation counter to 0, execute next command
#define CLK_STOP    1<<1    //stop system clock

#define ALU_CIN     1<<2    //clear carry_in for ALU and set carry_in for A_SHR/A_SHL 
#define ALU_MODE    1<<3    //0 - MATH, 1 - LOGIC
#define ALU_OP0     1<<4    //
#define ALU_OP1     1<<5    //bits for operation selection
#define ALU_OP2     1<<6    //see datasheet for details: https://www.ti.com/lit/ds/symlink/sn54s181.pdf
#define ALU_OP3     1<<7    //

#define A_OE      1<<16   //output A value to BUS
#define A_SHR     1<<8    //shift A value right, if both SHR and AHL then write input value to A
#define A_SHL     1<<9    //shift A value left, if both SHR and AHL then write input value to A
#define A_ALU     1<<10   //0 - input from BUS, 1 - input from ALU

#define FLAGS_WE    11<<20  //write input value to FLAGS
#define FLAGS_OE    11<<16  //output FLAGS value to BUS
#define FLAGS_ALU   1<<11   //0 - input from BUS, 1 - input from ALU

#define B_WE        2<<20   //write BUS value to B
#define B_OE        2<<16   //output B value to BUS
#define C_WE        3<<20   //write BUS value to C
#define C_OE        3<<16   //output C value to BUS
#define D_WE        4<<20   //write BUS value to D
#define D_OE        4<<16   //output D value to BUS
#define E_WE        5<<20   //write BUS value to E
#define E_OE        5<<16   //output E value to BUS
#define F_WE        6<<20   //write BUS value to F
#define F_OE        6<<16   //output E value to BUS
#define G_WE        7<<20   //write BUS value to G
#define G_OE        7<<16   //output G value to BUS
#define H_WE        8<<20   //write BUS value to H
#define H_OE        8<<16   //output H value to BUS

#define BP_WE       9<<20   //write BUS value to BP
#define BP_OE       9<<16   //output BP value to BUS

#define SP_WE       10<<20  //write BUS value to SP
#define SP_OE       10<<16  //output SP value to BUS
#define SP_CNT      1<<12   //increment/decrement SP        !!!!INVERTED, need to add inverted to schematic!!!!
#define SP_DOWN     1<<13   //0 - increment on CNT, 1 - decrement on CNT

#define IP_WE       12<<20  //write BUS value to IP
#define IP_OE       12<<16  //output IP value to BUS
#define IP_INC      1<<14   //increment IP
  
#define IR_WE       13<<20  //write BUS value to IR
#define IR_BYTE_OE  13<<16  //output extra byte to BUS

#define BASE_WE     1<<20   //write BUS value to BASE
#define OFFSET_WE   15<<20  //write BUS value to OFFSET
#define OFFSET_CLR  1<<15   //set OFFSET to 0               !!!!INVERTED, need to add inverted to schematic!!!!
#define MEM_WE      14<<20  //write BUS value to memory at BASE+OFFSET address
#define MEM_OE      14<<16  //output value from memory at BASE+OFFSET address to BUS