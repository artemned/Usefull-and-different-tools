`timescale 1 ns / 100 ps

module test_i2c_fclk;

reg clk,n_rst;
wire strob_up,strob_down,s_clk;


i2c_fclk dut(
              .clk(clk),
			  .rst(n_rst),
			  .s_clk(s_clk),
			  .strob_up(strob_up),
              .strob_down(strob_down)			  
            );
			
initial begin	//initial work frequency		
clk = 1'b1;
forever
#10 clk = ~clk; 
end

task reset;
begin

#5 n_rst = 1'b0;
#12 n_rst = 1'b1;

end
endtask

initial begin

 reset;
 
 $monitor("@time=%0t\t clk=%0d\t n_rst=0%d\t strob_up=0%d\t strob_down=0%d\t ",$time,
          clk,n_rst,strob_up,strob_down,s_clk);

 
 #100000 $finish; 	 
 
 end
 
endmodule