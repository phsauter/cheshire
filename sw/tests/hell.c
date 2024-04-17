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
//#include "pepe.h"

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

uint32_t test_peripheral(uint32_t* err, uint32_t arr) {
    uint32_t pixtot = (1056<<16) + 628;
    uint32_t pixact = (800<<16) + 600;
    uint32_t front_porch = (40<<16) + 1;
    uint32_t sync_times = ((128<<16) + 4) | (1<<31) | (1<<15);

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

uint32_t arr = 0x81000000;
int main(void) {
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);
    uint64_t reset_freq = clint_get_core_freq(rtc_freq, 2500);
/*    
    char str[] = "Hell!\r\n";
    uart_init(&__base_uart, reset_freq, __BOOT_BAUDRATE);
    uart_write_str(&__base_uart, str, sizeof(str));
    uart_write_flush(&__base_uart);
*/
    uint32_t err;
    uint32_t ret = test_peripheral(&err, (uint32_t) arr);
    wts(3, ret);
    wts(4, err);
    wts(5, *reg32(AXI2HDMI_BASE, CURRENT_PTR));
    wts(6, arr);
    wts(6, ((uint32_t*)arr)[1]);

    for(int x = 0; x < 800; x++) {
        for(int y = 0; y < 600; y++) {
            volatile uint8_t * ptr = arr + 3 * (y * 800 + x);
            uint32_t rgb = 0;
            /*
            uint8_t mod = y % 30;
            if (mod <= 10) {
                rgb = 0xff0000;
            } else if (mod <= 20) {
                rgb = 0x00ff00;
            } else {
                rgb = 0x0000ff;
            }
            */
            if(y <= 1 && y >= 0) {
                rgb = 0xffffff;
            }
            ptr[0] = rgb;
            ptr[1] = rgb >> 8;
            ptr[2] = rgb >> 16;
        }
    }


    /*
    uint64_t* k = arr + 800 * 300 * 0;
    for(uint16_t i = 0; i < 800 * 600 / 8; i++) {
        uint64_t rgb = orig_rgb;
        uint8_t byteshift = (i / 800) % 3;
        rgb <<= 8 * byteshift;
        rgb |= orig_rgb >> (24 - 8 * byteshift);
        rgb &= 0xffffff;
        k[0] = rgb << 48 | rgb << 24 | rgb;
        k[1] = rgb << 56 | rgb << 32 | rgb << 8 | rgb >> 16;
        k[2] = rgb << 40 | rgb << 16 | rgb >> 8;
        
        k += 3;
    }
    */
    fence();
    for(uint32_t i = 0; i < 0x40000; i++) {
        wts(7, i);
    }

    //Necessary so that FPGA/sim does not stop
    //while (1) {}
    
    return 0;
}


//VSync period: 16'579'200 ns -> 60.3165412 Hz
//VSync duration: 105'600ns
//HSync period: 26'400 ns     -> 37.87 kHz
//HSync duration: 3'200 ns