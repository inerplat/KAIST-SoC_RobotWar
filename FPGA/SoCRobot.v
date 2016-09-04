// Copyright (C) 1991-2011 Altera Corporation
// Your use of Altera Corporation's design tools, logic functions 
// and other software and tools, and its AMPP partner logic 
// functions, and any output files from any of the foregoing 
// (including device programming or simulation files), and any 
// associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License 
// Subscription Agreement, Altera MegaCore Function License 
// Agreement, or other applicable license agreement, including, 
// without limitation, that your use is for the sole purpose of 
// programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the 
// applicable agreement for further details.

(
	clk_llc,
	clk_llc2,
	clk_lsdr,
	eamem_adr,
	eamem_csx,
	eamem_data,
	eamem_irq,
	eamem_rdx,
	eamem_waitx,
	eamem_wrx,
	href,
	led_test,
	odd,
	resetx,
	vpo,
	vref
);

input			clk_llc;
input			clk_llc2;
input			clk_lsdr;
input	[17:0]	eamem_adr;
input			eamem_csx;
inout	[15:0]	eamem_data;
output			eamem_irq;
input			eamem_rdx;
output			eamem_waitx;
input			eamem_wrx;
input			href;
output			led_test;
input			odd;
input			resetx;
input	[15:0]	vpo;
input			vref;

endmodule
