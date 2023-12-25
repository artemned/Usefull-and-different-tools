module i2c_manager

#(
  parameter DELAY = 5,
  parameter VALUE = 8'b11001001 //169 //wrong send data with VALUE
 )
 
(
 input  clk,
 input  n_rst,
 inout tri sda,
 output scl	
);

localparam DELAY_SIZE = DELAY, // All date for count up number cycle  
           START_SIZE = 1,     // may be will be one clock or two clocks
		   ADDRES_SIZE = 7,
		   WRITE_SIZE  = 7,
		   READ_SIZE = WRITE_SIZE,
		   ACK_NACK_SIZE = 1,
           STOP_SIZE = START_SIZE;
		   
		   
localparam DELAY_ST = 0, 
           START_ST = 1,
		   ADDRESS_ST = 2,
           READ_ST = 3,		   
		   WRITE_ST = 4,
		   ACK_NACK_ST = 5,
		   STOP_ST = 6;
		   

wire sda_in;
wire strob_up;
wire strob_down;
integer flag_clk;
reg sda_out;

wire [7:0] p_data = 2'h77;     // data which will be send from master to slave 

//registers
reg [3:0] cnt=0;                 // our count 
reg [2:0] State;
reg [7:0] buffer =  8'b10110101;  //first data!
reg [7:0] address = 8'b10101001; // address slave device


i2c_fclk f_clk
(
  .clk(clk),
  .rst(n_rst),
  .s_clk(s_clk),
  .strob_up(strob_up),
  .strob_down(strob_down)  
);


assign sda = ((State == ACK_NACK_ST) || (State == READ_ST))
             ? 1'bz : sda_out; //when 0 work as output

assign sda_in = sda;//((State == ACK_NACK_ST) || (State == READ_ST)) ? sda : 1'bz;

assign scl = (flag_clk==1) ? s_clk : 1'b1; // give us press line to down

always@(posedge clk or negedge n_rst)
begin

 if(!n_rst)begin
 State <= DELAY_ST;
 cnt <= 0;
 sda_out <= 1'b1;
 flag_clk = 0; 
 end
 
 else begin
 if(strob_down)begin//fd
 
 case(State)
 
 DELAY_ST:begin
 if(cnt == DELAY_SIZE)State <= START_ST;
    cnt = (cnt == DELAY_SIZE) ? 0 : cnt + 1;
	
 end
 
 START_ST:begin          // one clock
	sda_out = 1'b0;      //break down line and sand data said slave be 
	State <= ADDRESS_ST;
	$display("Master Start!");
 end 
  
 ADDRESS_ST:begin // send address slave device
	flag_clk = 1;     //line clock now s_clk 	
	{sda_out,address} <= {address,1'b0}; //shift direct most significations bit
	cnt <= (cnt == ADDRES_SIZE)? 0 : cnt+1;
	$display("Master address=0%b\t",address);
    $display("Master repeat cnt=%d\t",cnt);
	if(cnt == 7 )begin 
	$display("Master all shift!!!",cnt);
    State <= ACK_NACK_ST;
    end
	
 end

 WRITE_ST:begin // writing data 
 $display("Master Write State.");
 if(cnt == WRITE_SIZE) State <= STOP_ST;
    cnt <= (cnt==WRITE_SIZE)? 0 : cnt + 1;
   {sda_out,buffer} <= {buffer,1'b0};
   
 end 
 
 
 STOP_ST: begin
 $display("Master StopState State.");
 sda_out <= 1'b1;
 State <= DELAY_ST;
 flag_clk = 1'b0;
 end
 
 
endcase
 
end 
  
 if(strob_up)begin
 case(State)
 ACK_NACK_ST:begin
 //if(cnt == 8)
 $display("Master AC State.");
  sda_out = sda;
 $display(sda_out);
 end
 
 endcase
 
 end//strob_up
  
  
end //else
end //always                                             

endmodule


 //{buffer[7:1],sda_out} <= (buffer >> 1); //shift less most significations bit
 //buffer <= buffer << 1; most significations bit 
 //sda_out <= buffer[7];
