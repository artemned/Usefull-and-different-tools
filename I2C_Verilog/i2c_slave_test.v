`timescale 1 ns / 100 ps
module i2c_slave_test; 
reg clk;
reg n_rst;


wire scl; 
wire sda;

			
i2c_manager dut(

                .clk(clk),
                .n_rst(n_rst),
                .scl(scl),
				.sda(sda)
               );			
			
i2c_slave sdut(
                .scl(scl),
				.sda(sda)
			  );			
		

initial begin	//initial work frequency		
clk = 1'b1;
forever
#10 clk = ~clk;
//#5020 clk = ~clk; 
end

//
//task send_data; //send data
//begin
//repeat(7)begin
//@(negedge s_clk)begin
//if(cnt < 7){sda,address} <= {address,1'b0};
//cnt <= (cnt == 7)? 0 : cnt + 1;  
//end
//end
//end
//endtask


task reset;
begin

#5 n_rst = 1'b0;
#12 n_rst = 1'b1;

end
endtask

//initial begin
//repeat(3)begin
//@(negedge clk)
//cnt <= cnt + 1;
//{sda,address} <= {address,1'b0}; // read Address
//
//$display("address=0%b\t",address);
//$display("repeat address cnt=%d\t",cnt);
//end
//end
//always@(negedge clk)begin
//if(cnt <= 6)begin
//cnt <= cnt + 1;
//{sda,address} <= {address,1'b0}; //send address slave device on negative front scl
//end
//end



initial begin

 reset;
 //send_data;
 

//$monitor("@time=%0t\t s_clk=0%d\t sda=0%d\t ack=0%d\t ",$time,clk,n_rst,s_clk,sda,ack);

 
 #300000 $finish; 	 
 
 end

endmodule	