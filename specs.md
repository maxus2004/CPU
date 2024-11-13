## IO (контроллер ввода/вывода)

#### управляющие сигналы
- in read_addr 
- in write_addr 
- in read_data 
- in write_data 
- in data_size

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
- in read_value_from_bus 
- in write_value_to_bus 
- in read_value_from_alu

#### базовые сигналы
- in clk
- in/out x16 system_bus

#### связь с АЛУ
- in x16 new_value
- out x16 value

---

## FLAGS (регистр флагов)

#### управляющие сигналы
- in read_flags_from_bus 
- in read_flags_to_bus 
- in read_flags_from_alu

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

## alu (арифметическо-логическое устройство)

#### управляющие сигналы
- in add
- in nand
- in shr
- in shl

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
- in inc
- in write
- in read

#### базовые сигналы
- in clk
- in/out x16 system_bus

#### связь с IR
- out x16 ip

---

## IR (регистр инструкции)

#### управляющие сигналы
- in read

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
- in reset

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
- out x32 control_signals

---

## CLOCK (генератор тактового сигнала)

#### управляющие сигналы
- in stop
- in start (только ручная кнопка)
- in step (только ручная кнопка)

#### базовые сигналы
- out clk


