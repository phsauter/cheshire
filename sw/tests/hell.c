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

void wts(int idx, uint32_t val) {
    *reg32(&__base_regs, CHESHIRE_SCRATCH_0_REG_OFFSET + CHESHIRE_SCRATCH_1_REG_OFFSET * idx) = val;
}

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

uint32_t test_peripheral(uint32_t* err, uint32_t arr) {
    uint32_t pixtot = (1056<<16) + 628;
    uint32_t pixact = (800<<16) + 600;
    uint32_t front_porch = (40<<16) + 1;
    uint32_t sync_times = ((128<<16) + 1) | (1<<31) | (1<<15);

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
/*
    wts(15, 10);
    *reg32(AXI2HDMI_BASE, POINTERQ) = arr;
    *err = *reg32(AXI2HDMI_BASE, POINTERQ);
    wts(15, *err);
    wts(15, 20);
    *reg32(AXI2HDMI_BASE, POINTERQ) = arr;
    *err = *reg32(AXI2HDMI_BASE, POINTERQ);
    wts(15, *err);
    wts(15, 30);
    *reg32(AXI2HDMI_BASE, POINTERQ) = arr;
    *err = *reg32(AXI2HDMI_BASE, POINTERQ);
    wts(15, *err);
    wts(15, 40);
    *reg32(AXI2HDMI_BASE, POINTERQ) = arr;
    *err = *reg32(AXI2HDMI_BASE, POINTERQ);
    wts(15, *err);
*/
    wts(15, 50);
    //Bitmask to hold pointer!
    *reg32(AXI2HDMI_BASE, POINTERQ) = arr | 0b010;
    *err = *reg32(AXI2HDMI_BASE, POINTERQ);
    wts(15, *err);
    wts(15, 60);
    /*
    if(*err != 1) {
        return 3;
        //TODO why this fail? :(
    }
    */
    *reg32(AXI2HDMI_BASE, POWERREG) = 1;
    *err = *reg32(AXI2HDMI_BASE, POWERREG);
    if(*err != 1) {
        return 6;
    }

    return 0;
}

//uint64_t arr[800 * 600 * 3 / 8];
uint32_t arr = 0x81000000;
int main(void) {
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);
    uint64_t reset_freq = clint_get_core_freq(rtc_freq, 2500);

    uart_init(&__base_uart, reset_freq, __BOOT_BAUDRATE);    

    wts(3, 0xff);
    volatile uint64_t* k = arr;
    for(uint16_t i = 0; i < 200; i++) {
        uint64_t rgb = 0xff;
        uint8_t byteshift = (i / 50) % 3;
        rgb <<= 8 * byteshift;
        rgb |= rgb >> (24 - 8 * byteshift);
        rgb &= 0xffffff;
        k[0] = rgb << 48 | rgb << 24 | rgb;
        k[1] = rgb << 56 | rgb << 32 | rgb << 8 | rgb >> 16;
        k[2] = rgb << 40 | rgb << 16 | rgb >> 8;
        
        k += 3;
    }

    fence();
    wts(3, 0xa0);
    uint32_t err;
    uint32_t ret = test_peripheral(&err, (uint32_t) arr);
    wts(3, ret);
    wts(4, err);
    wts(5, *reg32(AXI2HDMI_BASE, CURRENT_PTR));
    wts(6, arr);
    wts(6, ((uint32_t*)arr)[1]);

    k = arr + 800 * 300 * 0;
    for(uint16_t i = 0; i < 800 * 600 / 8; i++) {
        uint64_t rgb = 0xff;
        uint8_t byteshift = (i / 50) % 3;
        rgb <<= 8 * byteshift;
        rgb |= rgb >> (24 - 8 * byteshift);
        rgb &= 0xffffff;
        k[0] = rgb << 48 | rgb << 24 | rgb;
        k[1] = rgb << 56 | rgb << 32 | rgb << 8 | rgb >> 16;
        k[2] = rgb << 40 | rgb << 16 | rgb >> 8;
        
        k += 3;
    }
    fence();

    //Necessary so that FPGA does not stop
    while (1) {}
    
    return 0;
}
