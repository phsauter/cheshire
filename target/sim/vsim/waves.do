add wave -label "clk" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/AXI_ACLK_CI
add wave -noupdate -divider AXI
add wave -group {AXI2HDMI_tb AXI Req} -label "Req" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/axi_req_o
add wave -group {AXI2HDMI_tb AXI Resp} -label "Resp" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/axi_resp_i
add wave -group {AXI2HDMI_tb AXI Lite Req} -label "Req" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/axi_lite_req_i
add wave -group {AXI2HDMI_tb AXI Lite Resp} -label "Resp" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/axi_lite_resp_o
add wave -noupdate -divider AXI2HDMI_Output
add wave -group {AXI2HDMI} -label "r" sim:/tb_cheshire_soc/fix/dut/vga_red_o
add wave -group {AXI2HDMI} -label "g" sim:/tb_cheshire_soc/fix/dut/vga_green_o
add wave -group {AXI2HDMI} -label "b" sim:/tb_cheshire_soc/fix/dut/vga_blue_o
add wave -group {AXI2HDMI} -label "hsync" sim:/tb_cheshire_soc/fix/dut/vga_hsync_o
add wave -group {AXI2HDMI} -label "vsync" sim:/tb_cheshire_soc/fix/dut/vga_vsync_o

add wave -group {AXI2HDMI} -label "DOut_RGB_DO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/DOut_RGB_DO
add wave -group {AXI2HDMI} -label "DE_RGB_SO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/DE_RGB_SO
add wave -group {AXI2HDMI} -label "HSync_RGB_SO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/HSync_RGB_SO
add wave -group {AXI2HDMI} -label "VSync_RGB_SO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/VSync_RGB_SO


add wave -noupdate -divider ScratchRegisters
add wave -label "0"  sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_0/q
add wave -label "1"  sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_1/q
add wave -label "2"  sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_2/q
add wave -label "3"  sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_3/q
add wave -label "4"  sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_4/q
add wave -label "5"  sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_5/q
add wave -label "6"  sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_6/q
add wave -label "7"  sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_7/q
add wave -label "8"  sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_8/q
add wave -label "9"  sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_9/q
add wave -label "10" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_10/q
add wave -label "11" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_11/q
add wave -label "12" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_12/q
add wave -label "13" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_13/q
add wave -label "14" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_14/q
add wave -label "15" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_15/q
add wave -noupdate -divider CmdIf
add wave -label "PowerReg_DP"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/PowerReg_DP
add wave -label "TextPntr_S"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/TextPntr_SO
add wave -label "TextCols_D"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/TextCols_DO
add wave -label "TextRows_D"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/TextRows_DO
add wave -label "WrAddrReg_DP"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/WrAddrReg_DP
add wave -label "FramePntr_DO"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/FramePntr_DO
add wave -label "HTot_DO"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/HTot_DO
add wave -label "VTot_DO"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/VTot_DO
add wave -label "HAct_DO"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/HAct_DO
add wave -label "VAct_DO"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/VAct_DO
add wave -label "HFront_DO"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/HFront_DO
add wave -label "VFront_DO"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/VFront_DO
add wave -label "VSync_DO"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/VSync_DO
add wave -label "HSync_DO"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/HSync_DO
add wave -label "HSyncPol_SO"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/HSyncPol_SO
add wave -label "VSyncPol_SO"  sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/CmdInterface_i/VSyncPol_SO