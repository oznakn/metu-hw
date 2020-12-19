`timescale 1ns / 1ps

module AddParity(
    input [1:8] dataIn,
    output reg [1:12] dataOut
    );

    //Write your code below
    //
    //

    always @(dataIn) begin
        dataOut[1] = dataIn[1] ^ dataIn[2] ^ dataIn[4] ^ dataIn[5] ^ dataIn[7];
        dataOut[2] = dataIn[1] ^ dataIn[3] ^ dataIn[4] ^ dataIn[6] ^ dataIn[7];
        dataOut[3] = dataIn[1];
        dataOut[4] = dataIn[2] ^ dataIn[3] ^ dataIn[4] ^ dataIn[8];
        dataOut[5] = dataIn[2];
        dataOut[6] = dataIn[3];
        dataOut[7] = dataIn[4];
        dataOut[8] = dataIn[5] ^ dataIn[6] ^ dataIn[7] ^ dataIn[8];
        dataOut[9] = dataIn[5];
        dataOut[10] = dataIn[6];
        dataOut[11] = dataIn[7];
        dataOut[12] = dataIn[8];
    end
endmodule

module CheckParity(
    input [1:12] dataIn,
    output reg [1:8] dataOut
    );

    //Write your code below
    //
    //

    reg [3:0] C;
    reg [1:12] temp;

    always @(dataIn) begin
        C[0] = dataIn[1] ^ dataIn[3] ^ dataIn[5]  ^ dataIn[7]  ^ dataIn[9]  ^ dataIn[11];
        C[1] = dataIn[2] ^ dataIn[3] ^ dataIn[6]  ^ dataIn[7]  ^ dataIn[10] ^ dataIn[11];
        C[2] = dataIn[4] ^ dataIn[5] ^ dataIn[6]  ^ dataIn[7]  ^ dataIn[12];
        C[3] = dataIn[8] ^ dataIn[9] ^ dataIn[10] ^ dataIn[11] ^ dataIn[12];

        temp = dataIn;
        if (C > 0 && C <= 12) temp[C] = ~temp[C];

        dataOut[1] = temp[3];
        dataOut[2] = temp[5];
        dataOut[3] = temp[6];
        dataOut[4] = temp[7];
        dataOut[5] = temp[9];
        dataOut[6] = temp[10];
        dataOut[7] = temp[11];
        dataOut[8] = temp[12];
    end
endmodule

module RAM(
    input [7:0] dataIn, //0:read, 1:write
    input clk,
    input mode,
    input [3:0] addr,
    output reg [7:0] dataOut);

    //Write your code below
    //
    //

    integer i;
    reg [7:0] ram [0:16];

    initial begin
        for (i = 0; i < 16; i++) ram[i] = 0;
    end

    always @(posedge clk) begin
        if (mode == 0) begin
            dataOut = ram[addr];
        end
        else begin
            ram[addr] = dataIn;
        end
    end
endmodule

module SEC_MEM(
input [1:12] dataIn,
input clk,
input mode, //0:read, 1:write
input [3:0] addr,
output  [1:12] dataOut);

//DO NOT EDIT THIS MODULE
wire  [7:0]  ramDataIn;
wire  [7:0]  ramDataOut;
CheckParity CP(dataIn,ramDataIn);
RAM RM(ramDataIn, clk, mode, addr, ramDataOut);
AddParity AP(ramDataOut,dataOut);

endmodule
