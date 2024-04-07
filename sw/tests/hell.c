// Copyright 2023 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Nicole Narr <narrn@student.ethz.ch>
// Christopher Reinwardt <creinwar@student.ethz.ch>
//
// Simple payload to test bootmodes

#include "regs/cheshire.h"
#include "dif/clint.h"
#include "dif/uart.h"
#include "params.h"
#include "util.h"

#define AXI2HDMI_BASE     ((void*)0x03009000)                           // Paper base address
#define CMD_IF_OFFSET     0x00000008                           // Paper's command interface's register size
#define POINTERQ          ( 0 * CMD_IF_OFFSET)
#define H_VTOT            ( 1 * CMD_IF_OFFSET)
#define H_VACTIVE         ( 2 * CMD_IF_OFFSET)
#define H_VFRONT          ( 3 * CMD_IF_OFFSET)
#define H_VSYNC           ( 4 * CMD_IF_OFFSET)
#define POWERREG          ( 5 * CMD_IF_OFFSET)
#define CURRENT_PTR       ( 6 * CMD_IF_OFFSET)
#define TXT_BUFF_PARA     ( 7 * CMD_IF_OFFSET)
#define CURSOR_FONT_PARA  ( 8 * CMD_IF_OFFSET)

uint64_t* address;

void int_to_hex(uint32_t src, char* dest) {
    for(int b = 0; b < 8; b++) {
        uint32_t bits = (src >> (7 - b) * 4) & 0xf;
        if (bits < 10) {
            *dest = '0' + bits;
        } else {
            *dest = 'A' + (bits - 10);
        }
        dest++;
    }
}

uint32_t test_peripheral(uint32_t* err) {
    uint32_t pixtot = (106<<16) + 62;
    uint32_t pixact = (80<<16) + 60;
    uint32_t front_porch = (40<<16) + 1;
    uint32_t sync_times = ((128<<16) + 4) | (1<<31) | (1<<15);

    //10 pix vert and hor
    *reg32(AXI2HDMI_BASE, H_VTOT) = pixtot;
    *err = *reg32(AXI2HDMI_BASE, H_VTOT);
    if(*err != pixtot) {
        return 1;
    }

    *reg32(AXI2HDMI_BASE, H_VACTIVE) = pixact;
    *err = *reg32(AXI2HDMI_BASE, H_VACTIVE);
    if(*err != pixact) {
        return 2;
    }

    *reg32(AXI2HDMI_BASE, H_VFRONT) = front_porch;
    *err = *reg32(AXI2HDMI_BASE, H_VFRONT);
    if(*err != front_porch) {
        return 4;
    }

    *reg32(AXI2HDMI_BASE, H_VSYNC) = sync_times;
    *err = *reg32(AXI2HDMI_BASE, H_VSYNC);
    if(*err != sync_times) {
        return 5;
    }

    *reg32(AXI2HDMI_BASE, POINTERQ) = address;
    *err = *reg32(AXI2HDMI_BASE, POINTERQ);
    if(*err != 1) {
        //return 3; TODO why this fail? :(
    }

    *reg32(AXI2HDMI_BASE, POWERREG) = 1;
    *err = *reg32(AXI2HDMI_BASE, POWERREG);
    if(*err != 1) {
        return 6;
    }

    return 0;
}


int main(void) {
    uint64_t arr[80 * 6 / 2];
    address = arr;
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);
    uint64_t reset_freq = clint_get_core_freq(rtc_freq, 2500);
    uart_init(&__base_uart, reset_freq, __BOOT_BAUDRATE);    

    for(long unsigned int i = 0; i < sizeof(arr); i++) {
        *(((volatile uint8_t*)arr) + i) = i; 
    }

    char val[] = "uuuuoooo-\r\n";
    uint32_t err;
    uint32_t ret = test_peripheral(&err);

    int_to_hex(*reg32(AXI2HDMI_BASE, CURRENT_PTR), val);
    *(val + 8) = 'a' + ret;
    volatile uint32_t *scratch = reg32(&__base_regs, CHESHIRE_SCRATCH_3_REG_OFFSET);

    uart_write_str(&__base_uart, val, sizeof(val));
    uart_write_flush(&__base_uart);

    int_to_hex(((uint64_t)arr), val);
    uart_write_str(&__base_uart, val, sizeof(val));
    uart_write_flush(&__base_uart);
    return 0;
}
