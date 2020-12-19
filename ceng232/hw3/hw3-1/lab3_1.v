`timescale 1ns / 1ps

module sf(
    input S,
    input F,
    input clk,
    output reg Q
    );

    initial begin
        Q = 0;
    end

    always @ (posedge clk)
		begin
			if (S == 0 && F == 0) Q = 0;
			else if (S == 0 && F == 1) Q = 1;
			else if (S == 1 && F == 1) Q = ~Q;
			else Q = Q;
		end
endmodule


module ic1500(
    input B,
    input A,
    input X,
	 input clk,
	 output D0,
	 output D1,
	 output Q,
    output Z
    );

	wire t1;
	wire t2;
	wire t3;

	assign t1 = A & ~B & ~X;
	assign t2 = ~A & B & ~X;

	assign D1 = t1 | t2;

	assign t3 = A & ~B;
	assign D0 = t3 | X;

	sf sf_1(D1, D0, clk, Q);

	assign Z = Q & X;
endmodule