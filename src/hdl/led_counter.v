`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Inipro, Inc.
// Engineer: Polo
// 
// Create Date: 2019/03/07 19:44:56
// Design Name: 
// Module Name: led_counter
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


module led_counter(
    input clk,
    output [7:0] pmod_a
    );
    
    reg [27:0] cnt;
    
    always @(posedge clk) begin
        cnt <= cnt + 1;
    end
    
    assign pmod_a = cnt[27:20];
    
endmodule
