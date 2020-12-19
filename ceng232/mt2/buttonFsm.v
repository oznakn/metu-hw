module buttonFsm (
	input clk,
	input button,
	output stateful_button
);

	reg [1:0] state = 0;
	reg [1:0] nextState = 0;
	reg out = 0;

	always @ (posedge clk) begin
		state = nextState;
	end

	always @ (button) begin
		if (state == 0) begin
			if (button == 1) begin
				nextState = 1;
				out = 1;
			end
			else if (button == 0) begin
				nextState = 0;
				out = 0;
			end
		end
		else if (state == 1) begin
			if (button == 1) begin
				nextState = 1;
				out = 1;
			end
			else if (button == 0) begin
				nextState = 2;
				out = 1;
			end
		end
		else if (state == 2) begin
			if (button == 1) begin
				nextState = 3;
				out = 0;
			end
			else if (button == 0) begin
				nextState = 2;
				out = 1;
			end
		end
		else if (state == 3) begin
			if (button == 1) begin
				nextState = 3;
				out = 0;
			end
			else if (button == 0) begin
				nextState = 0;
				out = 0;
			end
		end
	end

	assign stateful_button = out;

endmodule
