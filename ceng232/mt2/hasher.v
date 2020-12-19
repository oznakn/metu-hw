module hasher (
	input clk,    // Clock
	input [15:0] cur_time,
	input [15:0] student_id,
	output [15:0] cur_hash
);
	reg [15:0] hash = 0;
	reg [31:0] tmp = 0;

	always @ (posedge clk) begin
		tmp = ((cur_time ^ student_id) - hash);
		hash = ((tmp * tmp) >> 8) & 16'hFFFF;
	end

	assign cur_hash = hash;

endmodule
