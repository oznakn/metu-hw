module top (
	input sysclk,    // 1 MHz
	input button_in,
	input [15:0] student_id,
	output[3:0] D5_out,
	output[3:0] D4_out,
	output[3:0] D3_out,
	output[3:0] D2_out,
	output[3:0] D1_out
);
	wire clk_500hz;
	wire clk_5s;
	wire enable;
	wire [15:0] cur_time;
	wire [15:0] cur_hash;

	rtcClkDivider rtcClkDivider_1(sysclk, clk_500hz, clk_5s);
	buttonFsm button_1(clk_500hz, button_in, enable);
	timekeeper timekeeper_1(clk_5s, cur_time);
	hasher hasher_1(clk_5s, cur_time, student_id, cur_hash);
	b16toBCD b16toBCD_1(cur_hash, enable, D5_out, D4_out, D3_out, D2_out, D1_out);

endmodule
