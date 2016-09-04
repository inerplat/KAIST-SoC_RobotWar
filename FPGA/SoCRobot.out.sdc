## Generated SDC file "SoCRobot.out.sdc"

## Copyright (C) 1991-2011 Altera Corporation
## Your use of Altera Corporation's design tools, logic functions 
## and other software and tools, and its AMPP partner logic 
## functions, and any output files from any of the foregoing 
## (including device programming or simulation files), and any 
## associated documentation or information are expressly subject 
## to the terms and conditions of the Altera Program License 
## Subscription Agreement, Altera MegaCore Function License 
## Agreement, or other applicable license agreement, including, 
## without limitation, that your use is for the sole purpose of 
## programming logic devices manufactured by Altera and sold by 
## Altera or its authorized distributors.  Please refer to the 
## applicable agreement for further details.


## VENDOR  "Altera"
## PROGRAM "Quartus II"
## VERSION "Version 11.0 Build 157 04/27/2011 SJ Web Edition"

## DATE    "Tue Jun 21 13:52:21 2011"

##
## DEVICE  "EP4CE75U19I7"
##


#**************************************************************
# Time Information
#**************************************************************

set_time_format -unit ns -decimal_places 3



#**************************************************************
# Create Clock
#**************************************************************

create_clock -name {vadr[14]} -period 1.000 -waveform { 0.000 0.500 } [get_registers {vadr[14]}]
create_clock -name {clk_div[1]} -period 1.000 -waveform { 0.000 0.500 } [get_registers {clk_div[1]}]
create_clock -name {clk_llc2} -period 1.000 -waveform { 0.000 0.500 } [get_ports {clk_llc2}]
create_clock -name {odd} -period 1.000 -waveform { 0.000 0.500 } [get_ports {odd}]
create_clock -name {href} -period 1.000 -waveform { 0.000 0.500 } [get_ports {href}]
create_clock -name {clk_lsdr} -period 1.000 -waveform { 0.000 0.500 } [get_ports {clk_lsdr}]
create_clock -name {clk_llc} -period 1.000 -waveform { 0.000 0.500 } [get_ports {clk_llc}]


#**************************************************************
# Create Generated Clock
#**************************************************************



#**************************************************************
# Set Clock Latency
#**************************************************************



#**************************************************************
# Set Clock Uncertainty
#**************************************************************

set_clock_uncertainty -rise_from [get_clocks {vadr[14]}] -rise_to [get_clocks {vadr[14]}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {vadr[14]}] -fall_to [get_clocks {vadr[14]}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {vadr[14]}] -rise_to [get_clocks {clk_div[1]}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {vadr[14]}] -fall_to [get_clocks {clk_div[1]}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {vadr[14]}] -rise_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {vadr[14]}] -fall_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {vadr[14]}] -rise_to [get_clocks {vadr[14]}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {vadr[14]}] -fall_to [get_clocks {vadr[14]}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {vadr[14]}] -rise_to [get_clocks {clk_div[1]}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {vadr[14]}] -fall_to [get_clocks {clk_div[1]}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {vadr[14]}] -rise_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {vadr[14]}] -fall_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {clk_div[1]}] -rise_to [get_clocks {vadr[14]}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {clk_div[1]}] -fall_to [get_clocks {vadr[14]}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {clk_div[1]}] -rise_to [get_clocks {clk_div[1]}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {clk_div[1]}] -fall_to [get_clocks {clk_div[1]}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {clk_div[1]}] -rise_to [get_clocks {clk_llc2}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {clk_div[1]}] -fall_to [get_clocks {clk_llc2}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {clk_div[1]}] -rise_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {clk_div[1]}] -fall_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {clk_div[1]}] -rise_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {clk_div[1]}] -fall_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {clk_div[1]}] -rise_to [get_clocks {vadr[14]}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {clk_div[1]}] -fall_to [get_clocks {vadr[14]}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {clk_div[1]}] -rise_to [get_clocks {clk_div[1]}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {clk_div[1]}] -fall_to [get_clocks {clk_div[1]}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {clk_div[1]}] -rise_to [get_clocks {clk_llc2}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {clk_div[1]}] -fall_to [get_clocks {clk_llc2}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {clk_div[1]}] -rise_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {clk_div[1]}] -fall_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {clk_div[1]}] -rise_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {clk_div[1]}] -fall_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {odd}] -rise_to [get_clocks {vadr[14]}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {odd}] -fall_to [get_clocks {vadr[14]}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {odd}] -rise_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {odd}] -fall_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {odd}] -rise_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {odd}] -fall_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {odd}] -rise_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {odd}] -fall_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {odd}] -rise_to [get_clocks {vadr[14]}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {odd}] -fall_to [get_clocks {vadr[14]}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {odd}] -rise_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {odd}] -fall_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {odd}] -rise_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {odd}] -fall_to [get_clocks {clk_lsdr}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {odd}] -rise_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {odd}] -fall_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {clk_llc2}] -rise_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {clk_llc2}] -fall_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {clk_llc2}] -rise_to [get_clocks {clk_llc2}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {clk_llc2}] -fall_to [get_clocks {clk_llc2}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {clk_llc2}] -rise_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {clk_llc2}] -fall_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {clk_llc2}] -rise_to [get_clocks {clk_llc2}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {clk_llc2}] -fall_to [get_clocks {clk_llc2}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {href}] -rise_to [get_clocks {vadr[14]}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {href}] -fall_to [get_clocks {vadr[14]}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {href}] -rise_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {href}] -fall_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {href}] -rise_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {href}] -fall_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {href}] -rise_to [get_clocks {vadr[14]}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {href}] -fall_to [get_clocks {vadr[14]}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {href}] -rise_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {href}] -fall_to [get_clocks {clk_div[1]}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {href}] -rise_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -fall_from [get_clocks {href}] -fall_to [get_clocks {clk_llc}]  0.030 
set_clock_uncertainty -rise_from [get_clocks {clk_lsdr}] -rise_to [get_clocks {clk_lsdr}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {clk_lsdr}] -fall_to [get_clocks {clk_lsdr}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {clk_lsdr}] -rise_to [get_clocks {clk_lsdr}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {clk_lsdr}] -fall_to [get_clocks {clk_lsdr}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {clk_llc}] -rise_to [get_clocks {clk_lsdr}]  0.040 
set_clock_uncertainty -rise_from [get_clocks {clk_llc}] -fall_to [get_clocks {clk_lsdr}]  0.040 
set_clock_uncertainty -rise_from [get_clocks {clk_llc}] -rise_to [get_clocks {clk_llc}]  0.020 
set_clock_uncertainty -rise_from [get_clocks {clk_llc}] -fall_to [get_clocks {clk_llc}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {clk_llc}] -rise_to [get_clocks {clk_lsdr}]  0.040 
set_clock_uncertainty -fall_from [get_clocks {clk_llc}] -fall_to [get_clocks {clk_lsdr}]  0.040 
set_clock_uncertainty -fall_from [get_clocks {clk_llc}] -rise_to [get_clocks {clk_llc}]  0.020 
set_clock_uncertainty -fall_from [get_clocks {clk_llc}] -fall_to [get_clocks {clk_llc}]  0.020 


#**************************************************************
# Set Input Delay
#**************************************************************



#**************************************************************
# Set Output Delay
#**************************************************************



#**************************************************************
# Set Clock Groups
#**************************************************************



#**************************************************************
# Set False Path
#**************************************************************



#**************************************************************
# Set Multicycle Path
#**************************************************************



#**************************************************************
# Set Maximum Delay
#**************************************************************



#**************************************************************
# Set Minimum Delay
#**************************************************************



#**************************************************************
# Set Input Transition
#**************************************************************

