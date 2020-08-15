`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Inipro, Inc.
// Engineer: Polo
// 
// Create Date: 2019/03/07 20:23:47
// Design Name: 
// Module Name: digital_watch
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


module digital_watch(
	input clk,
	output reg [7:0] pmod_a,pmod_b
    );
	 
	reg [25:0] cnt;
	reg [2:0] seg_state;
	reg [3:0] sec0,sec1,min0,min1,ssd_a,ssd_b;

	always@(posedge clk)
	begin
		cnt <= cnt+1;		
		if(cnt == 49999999) begin
			cnt <= 0;
			sec0 <= sec0 + 1;
			if (sec0 == 9)	begin
				sec0 <= 0;
				sec1 <= sec1 + 1;
				if (sec1 == 5)	begin
					sec1 <= 0;
					min0 <= min0 + 1;
					if (min0 == 9)	begin
						min0 <= 0;
						min1 <= min1 + 1'b1;
						if (min1 == 5)	begin
							min1 <= 0;
						end
					end
				end
			end		
		end
	end
	
	always@(posedge cnt[15])
	begin
		seg_state <= seg_state + 1;
	end
               
	always@(seg_state[0], sec0, sec1, min0, min1)
	begin
		case(seg_state[0])
			1'b0 : begin
				ssd_b <= sec0;
				ssd_a <= min0;
			end
			default : begin	
				ssd_b <= sec1;
                ssd_a <= min1;
			end
		endcase 
	end
	
	always@(ssd_a, seg_state)
	begin
	   pmod_a[7] <= seg_state[0];
	   case(ssd_a)
		    4'h0 : pmod_a[6:0] = 7'b0111111;
			4'h1 : pmod_a[6:0] = 7'b0000110;
			4'h2 : pmod_a[6:0] = 7'b1011011;
			4'h3 : pmod_a[6:0] = 7'b1001111;
			4'h4 : pmod_a[6:0] = 7'b1100110;
			4'h5 : pmod_a[6:0] = 7'b1101101;
			4'h6 : pmod_a[6:0] = 7'b1111101;
			4'h7 : pmod_a[6:0] = 7'b0100111;
			4'h8 : pmod_a[6:0] = 7'b1111111;
			4'h9 : pmod_a[6:0] = 7'b1101111;		
			default : pmod_a[6:0] = 7'b0000000;
		endcase
	end
	
	always@(ssd_b,seg_state)
    begin
       pmod_b[7] = seg_state[0];
        case(ssd_b)
            4'h0 : pmod_b[6:0] = 7'b0111111;
            4'h1 : pmod_b[6:0] = 7'b0000110;
            4'h2 : pmod_b[6:0] = 7'b1011011;
            4'h3 : pmod_b[6:0] = 7'b1001111;
            4'h4 : pmod_b[6:0] = 7'b1100110;
            4'h5 : pmod_b[6:0] = 7'b1101101;
            4'h6 : pmod_b[6:0] = 7'b1111101;
            4'h7 : pmod_b[6:0] = 7'b0100111;
            4'h8 : pmod_b[6:0] = 7'b1111111;
            4'h9 : pmod_b[6:0] = 7'b1101111;        
            default : pmod_b[6:0] = 7'b0000000;
        endcase
    end    
	
endmodule