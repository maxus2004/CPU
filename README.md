# Описание процессора

Архитектура процессора - Фон Неймановская, 16 разрядная, предназначение всех регистов строго определено, данные должны храниться в памяти.

Процессор будет собран из отдельных модулей, которые будут состоять из печатной платы с SMD микросхемами логики серии 74HC и микросхем памяти с параллельным вводом/выводом.

Все линии данных и управляющих сигналов будут подключены к светодиодам для обозначения их текущего значения. Светодиоды будут запитаны от отдельной линии питания, чтобы они не влияли на работу процессора, и их можно было выключить.

Записывать и читать значения в оперативной памяти можно будет вручную с помощью переключателей и кнопок для программирования и отладки. Также с помощью кнопок можно будет выполнять инструкции по тактам, нажимаешь кнопку - выполняется 1 такт.

# Модули процессора

## IO (контроллер ввода/вывода)

#### управляющие сигналы
- in io_read_addr - если 1, вывод адреса в system_bus
- in io_write_addr - если 1, ввод адреса с system_bus при переходе clk 0->1
- in io_read_data - если 1, вывод данных в system_bus
- in io_write_data - если 1, ввод данных с system_bus при переходе clk 0->1
- in io_data_size - выбор размера данных (0 - 16 бит, 1 - 8 бит)

#### базовые сигналы
- in clk
- in/out x16 system_bus

#### связь с MEM
- out mem_read - 1, если io_read_data=1 и адрес < 65280
- out mem_write - 1, если io_write_data=1 и адрес < 65280
- out x16 mem_addr - постоянный вывод значения регистра адреса
- in/out x16 mem_data - ввод данных и вывод в system_bus, если mem_read=1, вывод данных из system_bus если mem_write=1
- out mem_data_size - 1, если io_data_size=1

#### связь с портом ввода/вывода
- out port_read - 1, если io_read_data=1 и адрес >= 65280
- out port_write - 1, если io_write_data=1 и адрес >= 65280
- out x8 port_addr - постоянный вывод первых 8 бит значения регистра адреса
- in/out x8 port_data - ввод данных и вывод первых 8 бит в system_bus, если port_read=1, вывод первых 8 бит данных из system_bus если port_write=1

## MEM (память)

#### базовые сигналы
- in clk

#### связь с IO
- in mem_read - если 1, вывод данных по адресу mem_addr в mem_data
- in mem_write - если 1, ввод данных по адресу mem_addr из mem_data при переходе clk 0->1
- in x16 mem_addr - работа описана выше
- in/out x16 mem_data - работа описана выше
- in mem_data_size - если 0 - данные 16 бит, если 1 - данные 8 бит

## REG (регистр)

#### управляющие сигналы
- in reg_read_from_bus - если 1, ввод данных с system_bus при переходе clk 0->1 
- in reg_write_to_bus - если 1, вывод данных в system_bus
- in reg_read_from_alu - если 1, ввод данных с alu_result при переходе clk 0->1 

#### базовые сигналы
- in clk
- in/out x16 system_bus

#### связь с АЛУ
- in x16 alu_result - работа описана выше
- out x16 reg_value - постоянный вывод данных

## FLAGS (регистр флагов)

#### управляющие сигналы
- in flags_read_from_bus - если 1, ввод значений с system_bus (бит 0 - zf, бит 1 - sf, бит 2 - cf) при переходе clk 0->1 
- in flags_read_to_bus - если 1, вывод значений в system_bus (бит 0 - zf, бит 1 - sf, бит 2 - cf)
- in flags_read_from_alu - если 1, ввод значений с alu_x при переходе clk 0->1 

#### базовые сигналы
- in clk
- in/out x16 system_bus

#### связь с АЛУ и УУ
- in alu_zf - работа описана выше
- in alu_sf - работа описана выше
- in alu_cf - работа описана выше
- out flags_zf - постоянный вывод zf
- out flags_sf - постоянный вывод sf
- out flags_cf - постоянный вывод cf

## ALU (арифметическо-логическое устройство)

#### управляющие сигналы
- in alu_add - если 1, выолнять "reg_value + system_bus + flags_cf" и выводить в alu_result
- in alu_nand - если 1, выолнять "reg_value nand system_bus" и выводить в alu_result
- in alu_shr - если 1, выолнять сдвиг system_bus вправо и выводить в alu_result
- in alu_shl - если 1, выолнять сдвиг system_bus влево и выводить в alu_result

#### базовые сигналы
- in x16 system_bus

#### связь с REG
- in x16 reg_value - работа описана выше
- out x16 alu_result - работа описана выше

#### связь с FLAGS
- in flags_cf - работа описана выше
- out alu_zf - 1, если результат=0
- out alu_sf - 1, если результат<0 (>32767)
- out alu_cf - 1, если результат не влез в reg_value

## IP (регистр адреса инструкции)

#### управляющие сигналы
- in ip_inc - если 1, инкрементировать значение при переходе clk 0->1 
- in ip_write - если 1, записать значение из system_bus при переходе clk 0->1
- in ip_read - если 1, выводить значение в system_bus

#### базовые сигналы
- in clk
- in/out x16 system_bus

#### связь с IR
- out x16 ip_value - постоянный вывод значения

## IR (регистр инструкции)

#### управляющие сигналы
- in ir_read

#### базовые сигналы
- in clk
- in x16 system_bus

#### связь с IP
- in x16 ip_value

#### связь с ROM
- out x8 ir_value

## COUNT (счётчик микрооперации)

#### управляющие сигналы
- in count_reset

#### базовые сигналы
- in clk

#### связь с ROM
- out x4 count_value

## MICROCODE ROM (память микрокода / комбинационная схема управления)

#### связь с IR
- in x8 ir_value

#### связь с COUNT
- in x4 count_value

#### связь с FLAGS
- in flags_zf
- in flags_sf
- in flags_cf

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

## CLOCK (генератор тактового сигнала)

#### управляющие сигналы
- in clock_stop
- in clock_start (только ручная кнопка)
- in clock_step (только ручная кнопка)

#### базовые сигналы
- out clk
