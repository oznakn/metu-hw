module rtcClkDivider (
	input sys_clk,    // 1 MHz
	output clk_500Hz, // 500 Hz
	output clk_5s  // 0.2 Hz
);

	parameter KEYCHANGE_PERIOD = 5;  // DO NOT CHANGE THE NAME OF THIS PARAMETER
	// AND MAKE SURE TO USE THIS PARAMETER INSTEAD OF HARDCODING OTHER VALUES

	integer clk1_thresold;
	integer clk2_thresold;

	integer counter1;
	integer counter2;

	reg out1 = 0;
	reg out2 = 0;

	initial begin
		clk1_thresold = 2000 / 2;
		clk2_thresold = (KEYCHANGE_PERIOD * 1000000) / 2;

		counter1 = 0;
		counter2 = 0;

		out1 = 0;
		out2 = 0;
	end

	always @ (posedge sys_clk) begin
		counter1 = counter1 + 1;
		if (counter1 == clk1_thresold) begin
			out1 = ~out1;
			counter1 = 0;
		end

		counter2 = counter2 + 1;
		if (counter2 == clk2_thresold) begin
			out2 = ~out2;
			counter2 = 0;
		end
	end

	assign clk_500Hz = out1;
	assign clk_5s = out2;
endmodule
