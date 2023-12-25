`timescale 1 ns / 100 ps

module i2c_module_test;

    // simulation options
    parameter Tt     = 20;
    parameter Cycles = 50_000;

    // global signals
    reg         clk;
    reg         n_rst;

    // I2C bus
    wire        scl	;	
    wire        sda;
    // sensor value
    wire  [7:0] value;

    // DUT start
   i2c_manager dut
    (
     .clk  ( clk  ),
     .n_rst( n_rst),
     .sda  ( sda ),
     .scl  ( scl )
    );
    // DUT end

    //// peripheral device stub
    //pmod_als_stub stub
    //(
    //    .cs     ( cs    ),
    //    .sck    ( sck   ),
    //    .sdo    ( sdo   )
    //);

    // simulation init - clock
    initial begin
        clk = 0;
        forever clk = #(Tt/2) ~clk;
    end
	

    // simulation init - reset
    initial begin
        n_rst   = 0;
        repeat (1)  @(posedge clk);
        n_rst   = 1;
    end

    // simulation duration
    initial begin
       // repeat (Cycles)  @(posedge clk);
     //   $display ("Timeout");
        
    end

    // simulation output
    initial begin 
       // $monitor("value=%h", value);
   // $monitor("@time=%0t\t clk=%0d\t n_rst=0%d\t scl=0%d\t sda=0%d\t ",$time,clk,n_rst,sda,scl);

#1200000 $finish; 	 
 end
    initial 
        $dumpvars;

endmodule