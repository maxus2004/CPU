## IO (контроллер ввода/вывода)

#### управляющие сигналы
- in io_read_addr 
- in io_write_addr 
- in io_read_data 
- in io_write_data 
- in io_data_size

#### базовые сигналы
- in clk
- in/out x16 system_bus

#### связь с MEM
- out mem_read
- out mem_write
- out x16 mem_addr
- in/out x16 mem_data
- out data_size

#### связь с портом ввода/вывода
- out io_read
- out io_write
- out x8 io_addr
- in/out x8 io_data

---

## MEM (память)

#### базовые сигналы
- in clk

#### связь с IO
- in mem_read
- in mem_write
- in data_size
- in x16 mem_addr
- in/out x16 mem_data

---

## REG (регистр)

#### управляющие сигналы
- in reg_read_from_bus 
- in reg_write_to_bus 
- in reg_read_from_alu

#### базовые сигналы
- in clk
- in/out x16 system_bus

#### связь с АЛУ
- in x16 new_value
- out x16 value

---

## FLAGS (регистр флагов)

#### управляющие сигналы
- in flags_read_from_bus 
- in flags_read_to_bus 
- in flags_read_from_alu

#### базовые сигналы
- in clk
- in/out x16 system_bus

#### связь с АЛУ и УУ
- in zf
- in sf
- in cf
- out zf
- out sf
- out cf

---

## ALU (арифметическо-логическое устройство)

#### управляющие сигналы
- in alu_add
- in alu_nand
- in alu_shr
- in alu_shl

#### базовые сигналы
- in clk
- in x16 system_bus

#### связь с REG
- in x16 reg_value
- out x16 result

#### связь с FLAGS
- in zf
- in sf
- in cf
- out zf
- out sf
- out cf

---

## IP (регистр адреса инструкции)

#### управляющие сигналы
- in ip_inc
- in ip_write
- in ip_read

#### базовые сигналы
- in clk
- in/out x16 system_bus

#### связь с IR
- out x16 ip

---

## IR (регистр инструкции)

#### управляющие сигналы
- in ir_read

#### базовые сигналы
- in clk
- in x16 system_bus

#### связь с IP
- in x16 address

#### связь с ROM
- out x8 value

---

## COUNT (счётчик микрооперации)

#### управляющие сигналы
- in count_reset

#### базовые сигналы
- in clk

#### связь с ROM
- out x4 value

---

## microcode ROM (память микрокода / комбинационная схема управления)

#### связь с IR
- in x8 instruction

#### связь с COUNT
- in x4 count

#### связь с FLAGS
- in x3 flags

#### базовые сигналы
- in clk

#### управляющие сигналы
- out io_read_addr
- out io_write_addr
- out io_read_data
- out io_write_data
- out io_data_size
- out reg_read_from_bus 
- out reg_write_to_bus 
- out reg_read_from_alu
- out flags_read_from_bus 
- out flags_read_to_bus 
- out flags_read_from_alu
- out alu_add
- out alu_nand
- out alu_shr
- out alu_shl
- out ip_inc
- out ip_write
- out ip_read
- out ir_read
- out count_reset
- out clock_stop

---

## CLOCK (генератор тактового сигнала)

#### управляющие сигналы
- in clock_stop
- in clock_start (только ручная кнопка)
- in clock_step (только ручная кнопка)

#### базовые сигналы
- out clk
