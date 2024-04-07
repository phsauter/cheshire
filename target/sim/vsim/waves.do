add wave -label "clk" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/AXI_ACLK_CI
add wave -noupdate -divider AXI
add wave -group {AXI2HDMI_tb AXI Req} -label "Req" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/axi_req_o
add wave -group {AXI2HDMI_tb AXI Resp} -label "Resp" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/axi_resp_i
add wave -group {AXI2HDMI_tb AXI Lite Req} -label "Req" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/axi_lite_req_i
add wave -group {AXI2HDMI_tb AXI Lite Resp} -label "Resp" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/axi_lite_resp_o
add wave -noupdate -divider AXI2HDMI_Output
add wave -group {AXI2HDMI} -label "DOut_RGB_DO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/DOut_RGB_DO
add wave -group {AXI2HDMI} -label "DE_RGB_SO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/DE_RGB_SO
add wave -group {AXI2HDMI} -label "HSync_RGB_SO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/HSync_RGB_SO
add wave -group {AXI2HDMI} -label "VSync_RGB_SO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/VSync_RGB_SO
add wave -group {AXI2HDMI} -label "DOut_422_DO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/DOut_422_DO
add wave -group {AXI2HDMI} -label "DE_422_SO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/DE_422_SO
add wave -group {AXI2HDMI} -label "HSync_422_SO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/HSync_422_SO
add wave -group {AXI2HDMI} -label "VSync_422_SO" sim:/tb_cheshire_soc/fix/dut/gen_axi2hdmi/i_axi2hdmi/VSync_422_SO
add wave -noupdate -divider ScratchRegisters
add wave -label "0" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_0/q
add wave -label "1" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_1/q
add wave -label "2" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_2/q
add wave -label "3" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_3/q
add wave -label "4" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_4/q
add wave -label "5" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_5/q
add wave -label "6" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_6/q
add wave -label "7" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_7/q
add wave -label "8" sim:/tb_cheshire_soc/fix/dut/i_regs/u_scratch_8/q