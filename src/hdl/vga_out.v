`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Inipro, Inc.
// Engineer: Polo
// 
// Create Date: 2019/03/11 10:27:04
// Design Name: 
// Module Name: vga_out
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


module vga_out(
	input clk,
	output [7:0] pmod_a, pmod_b
    );

    wire pclk;
	reg [25:0] cnt;
	integer hcnt, vcnt, hc, vc, hd, vd;
	reg	[11:0]  vga_out;
	reg vga_hs, vga_vs;	
	
	assign pmod_b[3:0] = vga_out[11:8];
	assign pmod_a[3:0] = vga_out[7:4];
	assign pmod_b[7:4] = vga_out[3:0];
	assign pmod_a[4] = vga_hs;
	assign pmod_a[5] = vga_vs;

    clk_wiz_0 clk_wiz_0_inst
    (
        // Clock out ports
        .clk_out1(pclk),     // output clk_out1
        // Clock in ports
        .clk_in1(clk));      // input clk_in1
        
	always@(posedge pclk)
	begin
		
		hcnt <= hcnt + 1;
		if(hcnt == 799) begin
			hcnt <= 0;
			vcnt <= vcnt + 1;
			if(vcnt == 524)	vcnt <= 0;
		end
		
		if(hcnt>=656 && hcnt<752) vga_hs <= 1'b0;
		else                      vga_hs <= 1'b1;
		
		if(vcnt>=490 && vcnt<492) vga_vs <= 1'b0;
		else                      vga_vs <= 1'b1;
		
        if(hcnt>=0 && hcnt<640 && vcnt>=0 && vcnt<480) begin
            cnt <= cnt+1;		
            if(cnt == 500000) begin
                cnt <= 0;		
                hc = hc + hd;
                vc = vc + vd;
                if(hc == 500) hd = -1;
                if(hc == 0) hd = 1;
                if(vc == 440) vd = -1;
                if(vc == 0) vd = 1;
            end
                        
            vga_out <= 12'hfff;
                                    					
            if(hcnt>=hc && hcnt<hc+8 && vcnt>=vc && vcnt<vc+40) vga_out <= 12'hf00; // "I" Character
            
            if(hcnt>=hc+13 && hcnt<hc+21 && vcnt>=vc && vcnt<vc+40) vga_out <= 12'hf00; // "N" Character
            if(hcnt>=hc+13+(vcnt-vc)/2 && hcnt<hc+21+(vcnt-vc)/2 && vcnt>=vc && vcnt<vc+40) vga_out <= 12'hf00; // "N" Character
			if(hcnt>=hc+33 && hcnt<hc+41 && vcnt>=vc && vcnt<vc+40)  vga_out <= 12'hf00; // "N" Character
			
			if(hcnt>=hc+46 && hcnt<hc+54 && vcnt>=vc && vcnt<vc+40)  vga_out <= 12'hf00; // "I" Character            
			
			if(hcnt>=hc+59 && hcnt<hc+67 && vcnt>=vc && vcnt<vc+40)  vga_out <= 12'h00f; // "P" Character
			if(hcnt>=hc+67 && hcnt<hc+75 && vcnt>=vc && vcnt<vc+8)  vga_out <= 12'h00f; // "P" Character
			if(hcnt>=hc+67 && hcnt<hc+75 && vcnt>=vc+16 && vcnt<vc+24)  vga_out <= 12'h00f; // "P" Character
			if(hcnt>=hc+75 && hcnt<hc+83 && vcnt>=vc+0 && vcnt<vc+24)  vga_out <= 12'h00f; // "P" Character
			
			if(hcnt>=hc+88 && hcnt<hc+96 && vcnt>=vc && vcnt<vc+40)  vga_out <= 12'h00f; // "R" Character
			if(hcnt>=hc+96 && hcnt<hc+104 && vcnt>=vc && vcnt<vc+8)  vga_out <= 12'h00f; // "R" Character
			if(hcnt>=hc+96 && hcnt<hc+104 && vcnt>=vc+16 && vcnt<vc+24)  vga_out <= 12'h00f; // "R" Character
			if(hcnt>=hc+104 && hcnt<hc+112 && vcnt>=vc+0 && vcnt<vc+24)  vga_out <= 12'h00f; // "R" Character
			if(hcnt>=hc+88+(vcnt-vc)/2 && hcnt<hc+96+(vcnt-vc)/2 && vcnt>=vc+20 && vcnt<vc+40) vga_out <= 12'h00f; // "R" Character
			
			if(hcnt>=hc+117 && hcnt<hc+125 && vcnt>=vc && vcnt<vc+40)  vga_out <= 12'h00f; // "O" Character
			if(hcnt>=hc+133 && hcnt<hc+141 && vcnt>=vc && vcnt<vc+40)  vga_out <= 12'h00f; // "O" Character
			if(hcnt>=hc+117 && hcnt<hc+141 && vcnt>=vc && vcnt<vc+8)  vga_out <= 12'h00f; // "O" Character
			if(hcnt>=hc+117 && hcnt<hc+141 && vcnt>=vc+32 && vcnt<vc+40)  vga_out <= 12'h00f; // "O" Character			
		end
		else vga_out <= 12'h000;
		
	end 
	    
endmodule