/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'nios2_FFT' in SOPC Builder design 'main_system'
 * SOPC Builder design path: ../../main_system.sopcinfo
 *
 * Generated: Thu Jun 11 20:12:38 CEST 2020
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_gen2"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x00040820
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 50000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x13
#define ALT_CPU_DCACHE_BYPASS_MASK 0x80000000
#define ALT_CPU_DCACHE_LINE_SIZE 32
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_DCACHE_SIZE 2048
#define ALT_CPU_EXCEPTION_ADDR 0x00020020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 50000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_EXTRA_EXCEPTION_INFO
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 4096
#define ALT_CPU_INITDA_SUPPORTED
#define ALT_CPU_INST_ADDR_WIDTH 0x13
#define ALT_CPU_NAME "nios2_FFT"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x00020000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x00040820
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 50000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x13
#define NIOS2_DCACHE_BYPASS_MASK 0x80000000
#define NIOS2_DCACHE_LINE_SIZE 32
#define NIOS2_DCACHE_LINE_SIZE_LOG2 5
#define NIOS2_DCACHE_SIZE 2048
#define NIOS2_EXCEPTION_ADDR 0x00020020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_EXTRA_EXCEPTION_INFO
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 4096
#define NIOS2_INITDA_SUPPORTED
#define NIOS2_INST_ADDR_WIDTH 0x13
#define NIOS2_NUM_OF_SHADOW_REG_SETS 0
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x00020000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_MAILBOX_SIMPLE
#define __ALTERA_AVALON_MUTEX
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SYSID_QSYS
#define __ALTERA_AVALON_UART
#define __ALTERA_NIOS2_GEN2


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone V"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart_FFT"
#define ALT_STDERR_BASE 0x41060
#define ALT_STDERR_DEV jtag_uart_FFT
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart_FFT"
#define ALT_STDIN_BASE 0x41060
#define ALT_STDIN_DEV jtag_uart_FFT
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart_FFT"
#define ALT_STDOUT_BASE 0x41060
#define ALT_STDOUT_DEV jtag_uart_FFT
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "main_system"


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 32
#define ALT_SYS_CLK none
#define ALT_TIMESTAMP_CLK none


/*
 * jtag_uart_FFT configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart_FFT altera_avalon_jtag_uart
#define JTAG_UART_FFT_BASE 0x41060
#define JTAG_UART_FFT_IRQ 2
#define JTAG_UART_FFT_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_FFT_NAME "/dev/jtag_uart_FFT"
#define JTAG_UART_FFT_READ_DEPTH 64
#define JTAG_UART_FFT_READ_THRESHOLD 8
#define JTAG_UART_FFT_SPAN 8
#define JTAG_UART_FFT_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_FFT_WRITE_DEPTH 64
#define JTAG_UART_FFT_WRITE_THRESHOLD 8


/*
 * mailbox_to_FFT configuration
 *
 */

#define ALT_MODULE_CLASS_mailbox_to_FFT altera_avalon_mailbox_simple
#define MAILBOX_TO_FFT_BASE 0x41040
#define MAILBOX_TO_FFT_IRQ 0
#define MAILBOX_TO_FFT_IRQ_INTERRUPT_CONTROLLER_ID 0
#define MAILBOX_TO_FFT_NAME "/dev/mailbox_to_FFT"
#define MAILBOX_TO_FFT_SPAN 16
#define MAILBOX_TO_FFT_TYPE "altera_avalon_mailbox_simple"


/*
 * mailbox_to_Sound_Acquisition configuration
 *
 */

#define ALT_MODULE_CLASS_mailbox_to_Sound_Acquisition altera_avalon_mailbox_simple
#define MAILBOX_TO_SOUND_ACQUISITION_BASE 0x41020
#define MAILBOX_TO_SOUND_ACQUISITION_IRQ 4
#define MAILBOX_TO_SOUND_ACQUISITION_IRQ_INTERRUPT_CONTROLLER_ID 0
#define MAILBOX_TO_SOUND_ACQUISITION_NAME "/dev/mailbox_to_Sound_Acquisition"
#define MAILBOX_TO_SOUND_ACQUISITION_SPAN 16
#define MAILBOX_TO_SOUND_ACQUISITION_TYPE "altera_avalon_mailbox_simple"


/*
 * mutex_SDRAM configuration
 *
 */

#define ALT_MODULE_CLASS_mutex_SDRAM altera_avalon_mutex
#define MUTEX_SDRAM_BASE 0x41058
#define MUTEX_SDRAM_IRQ -1
#define MUTEX_SDRAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MUTEX_SDRAM_NAME "/dev/mutex_SDRAM"
#define MUTEX_SDRAM_OWNER_INIT 0
#define MUTEX_SDRAM_OWNER_WIDTH 16
#define MUTEX_SDRAM_SPAN 8
#define MUTEX_SDRAM_TYPE "altera_avalon_mutex"
#define MUTEX_SDRAM_VALUE_INIT 0
#define MUTEX_SDRAM_VALUE_WIDTH 16


/*
 * onchip_memory_nios2_FFT configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memory_nios2_FFT altera_avalon_onchip_memory2
#define ONCHIP_MEMORY_NIOS2_FFT_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY_NIOS2_FFT_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMORY_NIOS2_FFT_BASE 0x20000
#define ONCHIP_MEMORY_NIOS2_FFT_CONTENTS_INFO ""
#define ONCHIP_MEMORY_NIOS2_FFT_DUAL_PORT 0
#define ONCHIP_MEMORY_NIOS2_FFT_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY_NIOS2_FFT_INIT_CONTENTS_FILE "main_system_onchip_memory_nios2_FFT"
#define ONCHIP_MEMORY_NIOS2_FFT_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY_NIOS2_FFT_INSTANCE_ID "NONE"
#define ONCHIP_MEMORY_NIOS2_FFT_IRQ -1
#define ONCHIP_MEMORY_NIOS2_FFT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMORY_NIOS2_FFT_NAME "/dev/onchip_memory_nios2_FFT"
#define ONCHIP_MEMORY_NIOS2_FFT_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMORY_NIOS2_FFT_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY_NIOS2_FFT_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMORY_NIOS2_FFT_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY_NIOS2_FFT_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY_NIOS2_FFT_SIZE_VALUE 131072
#define ONCHIP_MEMORY_NIOS2_FFT_SPAN 131072
#define ONCHIP_MEMORY_NIOS2_FFT_TYPE "altera_avalon_onchip_memory2"
#define ONCHIP_MEMORY_NIOS2_FFT_WRITABLE 1


/*
 * pio_switches configuration
 *
 */

#define ALT_MODULE_CLASS_pio_switches altera_avalon_pio
#define PIO_SWITCHES_BASE 0x41030
#define PIO_SWITCHES_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_SWITCHES_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_SWITCHES_CAPTURE 1
#define PIO_SWITCHES_DATA_WIDTH 10
#define PIO_SWITCHES_DO_TEST_BENCH_WIRING 0
#define PIO_SWITCHES_DRIVEN_SIM_VALUE 0
#define PIO_SWITCHES_EDGE_TYPE "ANY"
#define PIO_SWITCHES_FREQ 50000000
#define PIO_SWITCHES_HAS_IN 1
#define PIO_SWITCHES_HAS_OUT 0
#define PIO_SWITCHES_HAS_TRI 0
#define PIO_SWITCHES_IRQ 3
#define PIO_SWITCHES_IRQ_INTERRUPT_CONTROLLER_ID 0
#define PIO_SWITCHES_IRQ_TYPE "EDGE"
#define PIO_SWITCHES_NAME "/dev/pio_switches"
#define PIO_SWITCHES_RESET_VALUE 0
#define PIO_SWITCHES_SPAN 16
#define PIO_SWITCHES_TYPE "altera_avalon_pio"


/*
 * sysid_qsys_0 configuration
 *
 */

#define ALT_MODULE_CLASS_sysid_qsys_0 altera_avalon_sysid_qsys
#define SYSID_QSYS_0_BASE 0x41050
#define SYSID_QSYS_0_ID 0
#define SYSID_QSYS_0_IRQ -1
#define SYSID_QSYS_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSID_QSYS_0_NAME "/dev/sysid_qsys_0"
#define SYSID_QSYS_0_SPAN 8
#define SYSID_QSYS_0_TIMESTAMP 1591897483
#define SYSID_QSYS_0_TYPE "altera_avalon_sysid_qsys"


/*
 * uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_uart_0 altera_avalon_uart
#define UART_0_BASE 0x41000
#define UART_0_BAUD 115200
#define UART_0_DATA_BITS 8
#define UART_0_FIXED_BAUD 1
#define UART_0_FREQ 50000000
#define UART_0_IRQ 1
#define UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define UART_0_NAME "/dev/uart_0"
#define UART_0_PARITY 'N'
#define UART_0_SIM_CHAR_STREAM ""
#define UART_0_SIM_TRUE_BAUD 0
#define UART_0_SPAN 32
#define UART_0_STOP_BITS 1
#define UART_0_SYNC_REG_DEPTH 2
#define UART_0_TYPE "altera_avalon_uart"
#define UART_0_USE_CTS_RTS 0
#define UART_0_USE_EOP_REGISTER 0

#endif /* __SYSTEM_H_ */
