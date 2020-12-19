`timescale 1ns / 1ps
module PharmacyMem(
	input [4:0] studentID ,
    input clk,
    input [1:0] mode , // 0: list, 2: checkIn, 3: delete
    input [7:0] checkInTime,
    output reg [4:0] listOutput,
    output reg listBusy ,
    output reg ready
	);

    reg [4:0] students [1:10];
    reg [7:0] studentTimes [1:10];

    integer studentCount;
    integer cursor;
    integer listCursor;

    integer flag;
    integer childCursor;
    integer currentStudent;
    integer currentStudentTime;
    integer temp;

    initial begin
        ready = 1;
        listBusy = 0;
        listOutput = 0;

        for (cursor = 1; cursor <= 10; cursor = cursor + 1) begin
            students[cursor] = 0;
            studentTimes[cursor] = 0;
        end

        studentCount = 0;
        cursor = 1;
        listCursor = 1;

        flag = 1;
        childCursor = 1;
        currentStudent = 0;
        currentStudentTime = 0;
    end

    always @(posedge clk) begin
        if (mode == 2'b10) begin // check in mode
            listBusy = 0;
            ready = 1;

            listCursor = 1;

            cursor = studentCount + 1;

            if (cursor >= 1 && cursor <= 10) begin
                if (cursor > 1) begin
                    temp = cursor / 2;

                    while (checkInTime < studentTimes[temp]) begin
                        students[cursor] = students[temp];
                        studentTimes[cursor] = studentTimes[temp];

                        cursor = temp;
                        temp = cursor / 2;
                    end
                end

                students[cursor] = studentID;
                studentTimes[cursor] = checkInTime;

                studentCount = studentCount + 1;
            end
        end
        else if (mode == 2'b11) begin // delete mode
            listBusy = 0;
            ready = 1;

            listCursor = 1;

            if (studentCount != 0) begin
                students[1] = students[studentCount];
                studentTimes[1] = studentTimes[studentCount];

                studentCount = studentCount - 1;

                flag = 1;
                cursor = 1;

                currentStudent = students[cursor];
                currentStudentTime = studentTimes[cursor];

                while (flag == 1 && cursor * 2 <= studentCount) begin
                    childCursor = cursor * 2;

                    if (childCursor != studentCount && studentTimes[childCursor + 1] < studentTimes[childCursor]) begin
                        childCursor = childCursor + 1;
                    end

                    if (studentTimes[childCursor] < currentStudentTime) begin
                        students[cursor] = students[childCursor];
                        studentTimes[cursor] = studentTimes[childCursor];

                        cursor = childCursor;
                    end
                    else begin
                        flag = 0;
                    end
                end

                students[cursor] = currentStudent;
                studentTimes[cursor] = currentStudentTime;
            end
        end
        else if (mode == 2'b00) begin // list mode
            if (ready == 1 && listBusy == 0) begin
                ready = 0;
                listBusy = 1;

                listCursor = 1;
            end

            if (listCursor <= studentCount) begin
                listOutput = students[listCursor];
                listCursor = listCursor + 1;
            end
            else begin
                ready = 1;
                listBusy = 0;
            end
        end
    end

endmodule