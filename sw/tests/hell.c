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
#include "pepe.h"

#define AXI2HDMI_BASE     ((void*)0x03009000)                           // Paper base address
#define CMD_IF_OFFSET     0x00000008                           // Paper's command interface's register size
#define POINTERQ          ( 0 * CMD_IF_OFFSET)
#define H_VTOT            ( 1 * CMD_IF_OFFSET)
#define H_VACTIVE         ( 2 * CMD_IF_OFFSET)
#define H_VFRONT          ( 3 * CMD_IF_OFFSET)
#define H_VSYNC           ( 4 * CMD_IF_OFFSET)
#define POWERREG          ( 5 * CMD_IF_OFFSET)
#define CURRENT_PTR       ( 6 * CMD_IF_OFFSET)
#define TEXT_BUFF_PARA    ( 7 * CMD_IF_OFFSET)
#define CURSOR_FONT_PARA  ( 8 * CMD_IF_OFFSET)

#define FG_COLOR_PALETTE  0x400
#define BG_COLOR_PALETTE  0x800

const uint8_t is_in_text_mode = 1;

uint32_t arr = 0x81000000;
uint32_t pixtot = (1056<<16) + 628;
uint32_t pixact = (800<<16) + 600;
uint32_t front_porch = (40<<16) + 1;
uint32_t sync_times = ((128<<16) + 4) | (1<<31) | (1<<15);
uint16_t cols = 90, rows = 35;

void wts(int idx, uint32_t val) {
    *reg32(&__base_regs, CHESHIRE_SCRATCH_0_REG_OFFSET + CHESHIRE_SCRATCH_1_REG_OFFSET * idx) = val;
}

uint32_t test_peripheral(uint32_t* const err, uint32_t ptr) {
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
    
    *reg32(AXI2HDMI_BASE, TEXT_BUFF_PARA) = (cols << 16) | rows;

    //TODO no init foreground/background?

    //Set text mode
    *reg32(AXI2HDMI_BASE, POWERREG) = (0 | (is_in_text_mode << 16));
    *err = *reg32(AXI2HDMI_BASE, POWERREG);
    if(*err != (0 | (is_in_text_mode << 16))) {
        return 12;
    }

    wts(15, 50);
    //Bitmask to hold pointer!
    *reg32(AXI2HDMI_BASE, POINTERQ) = ptr | 0b010;
    *err = *reg32(AXI2HDMI_BASE, POINTERQ);
    wts(15, *err);
    wts(15, 60);
    /*
    if(*err != 1) {
        return 3;
        //TODO why this fail? :(
    }
    */



    *reg32(AXI2HDMI_BASE, POWERREG) = (1 | (is_in_text_mode << 16));
    *err = *reg32(AXI2HDMI_BASE, POWERREG);
    if(*err != (1 | (is_in_text_mode << 16))) {
        return 6;
    }
    //Clear err
    *err = 0;
    return 0;
}

void pack_pixels(volatile uint32_t * const target, const uint32_t source[4]) {
    target[0] = ((source[1] & 0xff) << 24) | source[0];
    target[1] = ((source[2] & 0xffff) << 16) | ((source[1] >> 8) & 0xffff);
    target[2] = ((source[3] << 8)) & ((source[2] >> 16) & 0xff);
}

void init_memory(volatile uint8_t * const target) {
    if (is_in_text_mode) {
        volatile uint16_t * ptr = target;
        for(int i = 0; i < cols * rows; i++) {
            //Print every character I guess?
            //Bg and Fg use same color palette -> characters with same bg/fg color will not work
            *ptr = (((i / 16) % 0xff)<< 8) | (i % 0xff);
            ptr++;
        }
        wts(6, ((volatile uint32_t*)target)[1]);
    } else {
        for(int y = 0; y < 600; y++) {
            for(int x = 0; x < 800; x++) {
                volatile uint8_t * ptr = target + 3 * (x + y * 800);
                ptr[0] = x;
                ptr[1] = y;
                ptr[2] = x*x*y;

            }
        }
        /*
        for(int y = 0; y < 600; y++) {
            volatile uint32_t * line_start = target + y * (800 * 3);
            for(int x4 = 0; x4 < 800 / 4; x4++) {
                uint32_t pixels [4];
                for(int x = 0; x < 4; x++) {
                    uint32_t rgb = 0;
                    if(y == 0) {
                        rgb |= (x4 * 4 + x) / 10;
                    } else if (y == 1) {
                        rgb |= ((x4 * 4 + x) / 10) << 8;
                    } else if (y == 2) {
                        rgb |= ((x4 * 4 + x) / 10) << 16;
                    }
                    pixels[x] = rgb;
                }
                pack_pixels(line_start, pixels);
                //Wrote 4 pixels into 3 bytes, go on the next four
                line_start += 3;
            }
            fence();
            wts(7, y);
        }
        */
        wts(6, ((volatile uint32_t*)target)[1]);
    }

    //Make sure that RAM is updated
    fence();
}

void setpx(volatile uint8_t * ptr, uint32_t val) {
    ptr[0] = val;
    ptr[1] = val >> 8;
    ptr[2] = val >> 16;
}

float sin(float x) {
    return x - x*x*x / 6 + x*x*x*x*x / 120;
}

void make_video(volatile uint8_t * const target, int time) {
    const float PI = 3.1415;
    const int divider = 50;
    time %= (divider + 1);
    time -= (divider / 2);

    int radius = (int)(sin(2 * PI * time / divider) * 50) + 200;
    if(radius < 0) {
        radius = -radius;
    }

    int radius2 = radius * radius;
    int orad = (radius + 5) * (radius + 5);
    
    for(int y = 300 - (radius + 5 + 2); y < 300 + (radius + 5 + 3); y++) {
        for(int x = 400 - (radius + 5 + 2); x < 400 + (radius + 5 + 3); x++) {
            volatile uint8_t * ptr = target + 3 * (x + y * 800);
            int xm = x - 400;
            int ym = y - 300;
            int d = xm*xm + ym*ym;
            if(radius2 > d) {
                setpx(ptr, 0xff00ff);
            } else if (orad > d) {
                setpx(ptr, 0x00ff00);
            } else {
                setpx(ptr, 0);
            }
        }
    }
    fence();
}


int main(void) {
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);
    uint64_t reset_freq = clint_get_core_freq(rtc_freq, 2500);
/*    
    char str[] = "Hell!\r\n";
    uart_init(&__base_uart, reset_freq, __BOOT_BAUDRATE);
    uart_write_str(&__base_uart, str, sizeof(str));
    uart_write_flush(&__base_uart);
*/
    
    for(int i = 3; i < 16; i++)  {
        wts(i, -1);
    }
    init_memory(arr);

    uint32_t err;
    uint32_t ret = test_peripheral(&err, (uint32_t) arr);

    if(is_in_text_mode == 0) {
        for(int x = 0; x != -1; x++)
            make_video(arr, x);
    }
    
    wts(3, ret);
    wts(4, err);
    if(ret != 0) {
        return -1;
    }
    wts(5, *reg32(AXI2HDMI_BASE, CURRENT_PTR));
    //wts(6, arr);
    
    for(uint32_t i = 0; i < 0x80000; i++) {
        wts(7, i);
    }

    return 0;
}
