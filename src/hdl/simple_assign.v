`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Inipro, Inc.
// Engineer: Polo
// 
// Create Date: 2019/03/07 10:56:07
// Design Name: 
// Module Name: simple_assign
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module simple_assign(
    input [7:0] pmod_c,
    output [7:0] pmod_a
    );
    
    assign pmod_a = pmod_c;
    
endmodule
