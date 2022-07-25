
// module mux21 (
//     input D0,
//     input D1,
//     input S,
//     output reg Y
// );
//     always @(*) begin
//         if(S == 0)
//             Y <= D0;
//         else 
//             Y <= D1;
//     end
// endmodule

module mux21 (
    input D0,
    input D1,
    input S,
    output Y
);
    wire y1, y2, sel_n;
    not A1 (sel_n, S);
    and A2 (y1, D0, sel_n);
    and A3 (y2, D1, S);
    or A4 (Y, y1, y2);
endmodule


module SwB (
    input I0,
    input I1,
    input K0,
    input K1,
    input K2, 
    // input K3,  // unused
    // input K4,  // unused
    // input EXL, // unused
    
    output O0,
    output O1
);
    wire M0, M1, invout0, invout1;

    mux21 mux0(
        .D0(I0),
        .D1(I1),
        .S(K0),
        .Y(M0)
    );

    mux21 mux1(
        .D0(I1),
        .D1(I0),
        .S(K0),
        .Y(M1)
    );

    not (invout0, M0), (invout1, M1);

    mux21 mux2(
        .D0(M0),
        .D1(invout0),
        .S(K1),
        .Y(O0)
    );

    mux21 mux3(
        .D0(M1),
        .D1(invout1),
        .S(K2),
        .Y(O1)
    );

endmodule

module stg #(parameter N = 16) (
    input [N-1:0] IN,
    input [N/2-1:0] K0,
    input [N/2-1:0] K1,
    input [N/2-1:0] K2,
    output [N-1:0] OUT
);

    generate
        genvar i;
        //name of ith switchbox is all_swb[i].switchbox
        for (i = 0; i < N; i = i+2) begin: all_swb 
            SwB switchbox(
                .I0(IN[i]),
                .I1(IN[i+1]),
                .K0(K0[i/2]),
                .K1(K1[i/2]),
                .K2(K2[i/2]),
                .O0(OUT[i]),
                .O1(OUT[i+1])
            );
        end
    endgenerate

endmodule

// from https://ieeexplore-ieee-org.proxy-um.researchport.umd.edu/document/1671798
// can replace this with different network types
module shuffle_interconnect #(parameter N = 16) (
    input [N-1:0] IN,
    output [N-1:0] OUT
);
    generate
    genvar i;
    for (i = 0; i < N; i = i+1) begin: shuffleblk
        // outi = 2*i%N + 2*i/N;
        // assign OUT[2*i%N + 2*i/N;] <= IN[i];
        if(i < N/2) begin: test
            assign OUT[2*i] = IN[i];
        end else begin: test
            assign OUT[2*i+1-N] = IN[i];
    end end
    endgenerate
endmodule



//stages should be lg(N) + M
//in this case M = 1
module cln #(parameter N = 16, parameter stages = 6) (
    input [N-1:0] IN,
    input [N*stages/2-1:0] K0,
    input [N*stages/2-1:0] K1,
    input [N*stages/2-1:0] K2,
    output [N-1:0] OUT
);
    wire [N-1:0] ic [2*(stages-1)-1:0];
    generate
        genvar i;
        for(i = 0; i < stages; i = i+1) begin: all_stg
            if(i == 0) begin: all_stg //first stage, tie in inputs
                stg #(.N(N)) stage (
                    .IN(IN),  // INPUT HERE
                    .K0(K0[N*(i+1)/2-1:N*i/2]),
                    .K1(K1[N*(i+1)/2-1:N*i/2]),
                    .K2(K2[N*(i+1)/2-1:N*i/2]),
                    .OUT(ic[2*i])
                );

                shuffle_interconnect #(.N(N)) si (
                    .IN(ic[2*i]),
                    .OUT(ic[2*i + 1])
                );
            end
            else if(i == stages - 1) begin: all_stg  //last stage, tie in outputs
                stg #(.N(N)) stage(
                    .IN(ic[2*i - 1]),
                    .K0(K0[N*(i+1)/2-1:N*i/2]),
                    .K1(K1[N*(i+1)/2-1:N*i/2]),
                    .K2(K2[N*(i+1)/2-1:N*i/2]),
                    .OUT(OUT)  // OUTPUT HERE
                );
            end
            else begin: all_stg  //intermediate stage
                stg #(.N(N)) stage (
                    .IN(ic[2*i-1]),
                    .K0(K0[N*(i+1)/2-1:N*i/2]),
                    .K1(K1[N*(i+1)/2-1:N*i/2]),
                    .K2(K2[N*(i+1)/2-1:N*i/2]),
                    .OUT(ic[2*i])
                );

                shuffle_interconnect #(.N(N)) si (
                    .IN(ic[2*i]),
                    .OUT(ic[2*i + 1])
                );
            end
            
        end
    endgenerate

endmodule
