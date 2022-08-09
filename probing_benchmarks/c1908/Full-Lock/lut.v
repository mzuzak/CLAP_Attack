module lut1 #(parameter N = 1) (
    input [N-1:0] inputs,
    output reg [N-1:0] outputs
);
    always @(*)
        case(inputs)
            1'b0 : outputs = 1'b0;
            1'b1 : outputs = 1'b1;
        endcase
endmodule

module lut2 #(parameter N = 2) (
    input [N-1:0] inputs,
    output reg [N-1:0] outputs
);
    always @(*)
        case(inputs)
            2'b00 : outputs = 2'b00;
            2'b01 : outputs = 2'b01;
            2'b10 : outputs = 2'b10;
            2'b11 : outputs = 2'b11;
        endcase
endmodule


module lut3 #(parameter N = 3) (
    input [N-1:0] inputs,
    output reg [N-1:0] outputs
);
    always @(*)
        case(inputs)
            3'b000 : outputs = 3'b000;
            3'b001 : outputs = 3'b001;
            3'b010 : outputs = 3'b010;
            3'b011 : outputs = 3'b011;
            3'b100 : outputs = 3'b100;
            3'b101 : outputs = 3'b101;
            3'b110 : outputs = 3'b110;
            3'b111 : outputs = 3'b111;
        endcase
endmodule

module lut4 #(parameter N = 4) (
    input [N-1:0] inputs,
    output reg [N-1:0] outputs
);
    always @(*)
        case(inputs)
            4'b0000 : outputs = 4'b0000;
            4'b0001 : outputs = 4'b0001;
            4'b0010 : outputs = 4'b0010;
            4'b0011 : outputs = 4'b0011;
            4'b0100 : outputs = 4'b0100;
            4'b0101 : outputs = 4'b0101;
            4'b0110 : outputs = 4'b0110;
            4'b0111 : outputs = 4'b0111;
            4'b1000 : outputs = 4'b1000;
            4'b1001 : outputs = 4'b1001;
            4'b1010 : outputs = 4'b1010;
            4'b1011 : outputs = 4'b1011;
            4'b1100 : outputs = 4'b1100;
            4'b1101 : outputs = 4'b1101;
            4'b1110 : outputs = 4'b1110;
            4'b1111 : outputs = 4'b1111;
        endcase
endmodule

module lut5 #(parameter N = 5) (
    input [N-1:0] inputs,
    output reg [N-1:0] outputs
);
    always @(*)
        case(inputs)
            5'b00000 : outputs = 5'b00000;
            5'b00001 : outputs = 5'b00001;
            5'b00010 : outputs = 5'b00010;
            5'b00011 : outputs = 5'b00011;
            5'b00100 : outputs = 5'b00100;
            5'b00101 : outputs = 5'b00101;
            5'b00110 : outputs = 5'b00110;
            5'b00111 : outputs = 5'b00111;
            5'b01000 : outputs = 5'b01000;
            5'b01001 : outputs = 5'b01001;
            5'b01010 : outputs = 5'b01010;
            5'b01011 : outputs = 5'b01011;
            5'b01100 : outputs = 5'b01100;
            5'b01101 : outputs = 5'b01101;
            5'b01110 : outputs = 5'b01110;
            5'b01111 : outputs = 5'b01111;
            5'b10000 : outputs = 5'b10000;
            5'b10001 : outputs = 5'b10001;
            5'b10010 : outputs = 5'b10010;
            5'b10011 : outputs = 5'b10011;
            5'b10100 : outputs = 5'b10100;
            5'b10101 : outputs = 5'b10101;
            5'b10110 : outputs = 5'b10110;
            5'b10111 : outputs = 5'b10111;
            5'b11000 : outputs = 5'b11000;
            5'b11001 : outputs = 5'b11001;
            5'b11010 : outputs = 5'b11010;
            5'b11011 : outputs = 5'b11011;
            5'b11100 : outputs = 5'b11100;
            5'b11101 : outputs = 5'b11101;
            5'b11110 : outputs = 5'b11110;
            5'b11111 : outputs = 5'b11111;
        endcase
endmodule
