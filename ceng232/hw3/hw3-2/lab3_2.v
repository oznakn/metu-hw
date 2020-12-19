`timescale 1ns / 1ps
module lab3_2(
        input[3:0] command,
        input CLK,
        input mode,
        output reg [7:0] total_time1,
        output reg [7:0] total_time0,
        output reg [7:0] total_cost1,
        output reg [7:0] total_cost0,
        output reg [3:2] coord_x,
        output reg [1:0] coord_y,
        output reg warning
    );

    initial
        begin
            total_cost0 = 0;
            total_cost1 = 0;

            total_time0 = 0;
            total_time1 = 0;

            coord_x = 1;
            coord_y = 1;
        end

    always @ (mode or posedge CLK)
        begin
            if (mode == 1)
                begin
                    warning = 1;

                    total_cost0 = 0;
                    total_cost1 = 0;

                    total_time0 = 0;
                    total_time1 = 0;

                    coord_x = 0;
                    coord_y = 0;
                end

            else if (mode == 0)
                begin
                    warning = 0;

                    case (command)
                        0:
                            begin
                                total_cost0 += 1;
                            end

                        1:
                            begin
                                if (coord_y != 2 && coord_x != 0)
                                    begin
                                        coord_y += 1;
                                        coord_x -= 1;
                                        total_cost0 += 3;
                                    end
                                else
                                    total_cost0 += 1;
                            end

                        2:
                            begin
                                if (coord_y == 2)
                                    coord_y = 0;
                                else
                                    coord_y += 1;

                                total_cost0 += 2;
                            end

                        3:
                            begin
                                if (coord_y != 2 && coord_x != 2)
                                    begin
                                        coord_y += 1;
                                        coord_x += 1;
                                        total_cost0 += 3;
                                    end
                                else
                                    total_cost0 += 1;
                            end

                        4:
                            begin
                                if (coord_x == 2)
                                    coord_x = 0;
                                else
                                    coord_x += 1;

                                total_cost0 += 2;
                            end

                        5:
                            begin
                                if (coord_y != 0 && coord_x != 2)
                                    begin
                                        coord_y -= 1;
                                        coord_x += 1;
                                        total_cost0 += 3;
                                    end
                                else
                                    total_cost0 += 1;
                            end


                        6:
                            begin
                                if (coord_y == 0)
                                    coord_y = 2;
                                else
                                    coord_y -= 1;

                                total_cost0 += 2;
                            end

                        7:
                            begin
                                if (coord_y != 0 && coord_x != 0)
                                    begin
                                        coord_y -= 1;
                                        coord_x -= 1;
                                        total_cost0 += 3;
                                    end
                                else
                                    total_cost0 += 1;
                            end

                        8:
                            begin
                                if (coord_x == 0)
                                    coord_x = 2;
                                else
                                    coord_x -= 1;

                                total_cost0 += 2;
                            end
                    endcase

                    if (total_cost0 > 9)
                        begin
                            total_cost1 += 1;
                            total_cost0 -= 10;
                        end

                    if (total_cost1 > 1)
                        begin
                            total_cost1 = 0;
                        end

                    total_time0 += 1;

                    if (total_time0 > 9)
                        begin
                            total_time1 += 1;
                            total_time0 -= 10;
                        end

                    if (total_time1 > 1)
                        begin
                            total_time1 = 0;
                        end
                end
        end
endmodule