# ----------------------------------------------------------------------------  
# 
#  Inipro Inc.          www.inipro.net
#
# ----------------------------------------------------------------------------
#
#     IO standard for Bank 26 Vcco supply is fixed at 1.8V
#     IO standard for Bank 65 Vcco supply is fixed at 1.2V
#
# ----------------------------------------------------------------------------
 
 
# ----------------------------------------------------------------------------
# High-speed expansion connector
# ---------------------------------------------------------------------------- 
# Bank 65
set_property PACKAGE_PIN F1   [get_ports {pmod_c[1]               }];  # "F1.DSI_D0_N"
set_property PACKAGE_PIN G1   [get_ports {pmod_c[0]               }];  # "G1.DSI_D0_P"
set_property PACKAGE_PIN E3   [get_ports {pmod_c[3]               }];  # "E3.DSI_D1_N"
set_property PACKAGE_PIN E4   [get_ports {pmod_c[2]               }];  # "E4.DSI_D1_P"
set_property PACKAGE_PIN D1   [get_ports {pmod_c[5]               }];  # "D1.DSI_D2_N"
set_property PACKAGE_PIN E1   [get_ports {pmod_c[4]               }];  # "E1.DSI_D2_P"
set_property PACKAGE_PIN C3   [get_ports {pmod_c[7]               }];  # "C3.DSI_D3_N"
set_property PACKAGE_PIN D3   [get_ports {pmod_c[6]               }];  # "D3.DSI_D3_P"

set_property PACKAGE_PIN L2   [get_ports {clk	                  }];  # "L2.CSI0_D3_P"


# ----------------------------------------------------------------------------
# Low-speed expansion connector
# ---------------------------------------------------------------------------- 
# Bank 26
set_property PACKAGE_PIN D7   [get_ports {pmod_a[0]               }];  # "D7.HD_GPIO_0"
set_property PACKAGE_PIN F8   [get_ports {pmod_a[1]               }];  # "F8.HD_GPIO_1"
set_property PACKAGE_PIN F7   [get_ports {pmod_a[2]               }];  # "F7.HD_GPIO_2"
set_property PACKAGE_PIN G7   [get_ports {pmod_a[3]               }];  # "G7.HD_GPIO_3"
set_property PACKAGE_PIN F6   [get_ports {pmod_a[4]               }];  # "F6.HD_GPIO_4"
set_property PACKAGE_PIN G5   [get_ports {pmod_a[5]               }];  # "G5.HD_GPIO_5"
set_property PACKAGE_PIN A6   [get_ports {pmod_a[6]               }];  # "A6.HD_GPIO_6"
set_property PACKAGE_PIN A7   [get_ports {pmod_a[7]               }];  # "A7.HD_GPIO_7"

set_property PACKAGE_PIN G6   [get_ports {pmod_b[0]               }];  # "G6.HD_GPIO_8"
set_property PACKAGE_PIN E6   [get_ports {pmod_b[1]               }];  # "E6.HD_GPIO_9"
set_property PACKAGE_PIN E5   [get_ports {pmod_b[2]               }];  # "E5.HD_GPIO_10"
set_property PACKAGE_PIN D6   [get_ports {pmod_b[3]               }];  # "D6.HD_GPIO_11"
set_property PACKAGE_PIN D5   [get_ports {pmod_b[4]               }];  # "D5.HD_GPIO_12"
set_property PACKAGE_PIN C7   [get_ports {pmod_b[5]               }];  # "C7.HD_GPIO_13"
set_property PACKAGE_PIN B6   [get_ports {pmod_b[6]               }];  # "B6.HD_GPIO_14"
set_property PACKAGE_PIN C5   [get_ports {pmod_b[7]               }];  # "C5.HD_GPIO_15"

#BT_HCI_RTS on FPGA /  emio_uart0_ctsn connect to 
set_property PACKAGE_PIN B7 [get_ports BT_ctsn]
#BT_HCI_CTS on FPGA / emio_uart0_rtsn
set_property PACKAGE_PIN B5 [get_ports BT_rtsn]

# ----------------------------------------------------------------------------
#
# IOSTANDARD Constraints
#
# ---------------------------------------------------------------------------- 

# Set the bank voltage for IO Bank 26 to 1.8V
set_property IOSTANDARD LVCMOS18 [get_ports -of_objects [get_iobanks 26]];

# Set the bank voltage for IO Bank 65 to 1.2V
set_property IOSTANDARD LVCMOS12 [get_ports -of_objects [get_iobanks 65]];

# Set the voltage for BT* to 1.8V
set_property IOSTANDARD LVCMOS18 [get_ports BT*]