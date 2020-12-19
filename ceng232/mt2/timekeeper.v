module timekeeper (
	input clk,    // Clock
	output[15:0] cur_time
);

	reg [15:0] counter = 0;

	always @ (posedge clk) begin
		if (counter == 16'hFFFF) counter = 0;
		else counter = counter + 1;
	end

	assign cur_time = counter;

endmodule
