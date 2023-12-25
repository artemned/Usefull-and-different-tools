module i2c_one

#(
  parameter DELAY = 1,
  parameter VALUE = 8'b11001001//169 //wrong send data with VALUE
 )
 
(
 input  clk,
 input  n_rst,
 output  sda,
 output s_clk	
 //output reg [7:0] value
);

localparam DELAY_SIZE = DELAY, // All date for count up number cycle  
           START_SIZE = 1,     // may be will be one clock or two clocks
		   ADDRESS_SIZE = 7,
		   WRITE_SIZE  = 7,
		   READ_SIZE = WRITE_SIZE,
		   ACK_SIZE = 1,
           STOP_SIZE = START_SIZE;
		   
		   
localparam DELAY_ST = 0, 
           START_ST = 1,
		   ADDRESS_ST = 2,
           READ_ST = 3,		   
		   WRITE_ST = 4,
		   ACK_ST = 5,
		   NACK_ST = 6,
		   STOP_ST = 7;

wire strob_up;
wire strob_down;

integer flag_clk;
integer sda_flag;
 
//registers

reg sda_out;
reg [3:0] cnt;          
reg [2:0] State;
reg [7:0] buffer = 8'b10110101;//send and recieve data
reg [7:0] address = 8'b10101001;//For slave address;


reg [3:0] Nextcnt;
reg [2:0] Nextstate;
reg [7:0] Nextbuffer ;//send and recieve data
reg [7:0] Nextaddress ;//For slave address; 

     
i2c_fclk f_clk
(
  .clk(clk),
  .rst(n_rst),
  .s_clk(s_clk),
  .strob_up(strob_up),
  .strob_down(strob_down)  
);


assign sda = sda_out;//(State==ACKNOWLADGE_ST) ? 1'bz : sda_out; //when flag is true sda work as input 
//assign scl = s_clk;(State==DELAY_ST)? 1'b1 : s_clk;       // give us press line to down

//Next state determining 
always @(s_clk) begin 

 if(strob_down) begin //Logic writing date to line SDA
  Nextstate = State;
  case(State)
  DELAY_ST  : if(cnt == DELAY_SIZE)  Nextstate = START_ST;
  START_ST  : if(cnt == START_SIZE)  Nextstate = ADDRESS_ST;
  ADDRESS_ST: if(cnt == ADDRESS_SIZE)Nextstate = ACK_ST;
  //WRITE_ST  : if(cnt == WRITE_SIZE)  Nextstate = ACK_ST;
  //STOP_ST   : Nextstate = DELAY_ST;
  endcase
 end //strob
 
 else if(strob_up)begin //Logic read date from line SDA
 case(State)
 ACK_ST: Nextstate = (~sda) ?
	     ((buffer[7]==0) ? WRITE_ST:READ_ST): NACK_ST;
 //NACK_ST:Nextstate =DELAY_ST;		
 //READ_ST:if(cnt == READ_SIZE) Nextstate = ACK; 
 endcase
 
end
end

//count determining
always@(s_clk)begin
Nextcnt = cnt;
if(strob_down)begin
 Nextcnt = cnt + 1;
 case(State)
     DELAY_ST     : if (cnt == DELAY_SIZE)    Nextcnt = 0;
	 ADDRESS_SIZE : if (cnt == ADDRESS_SIZE)  Nextcnt = 0;
     WRITE_ST     : if (cnt == WRITE_SIZE)    Nextcnt = 0;
     READ_ST      : if (cnt == READ_SIZE)     Nextcnt = 0;
endcase        
end	
end //always

always@(s_clk)begin //Logic wtiting and reading value
Nextbuffer = buffer;
Nextaddress = address;
if(strob_up)
    case(State)
        ADDRESS_ST    : Nextaddress = { Nextaddress[6:0], sda_out};
        
    endcase

end

		
always@(posedge clk,negedge n_rst)begin //Logic swich up state 
if(~n_rst)begin
State <= DELAY_ST;
cnt <= 0;
buffer <= 0;
sda_out <=1'b1;
//flag_clk = 1;
end

else begin
State <= Nextstate;
cnt <= Nextcnt;
buffer <= Nextbuffer;
address <= Nextaddress;
end

end

//task write_data();
//begin
//$display("Master address=0%b\t",address);
//$display("Master repeat cnt=%d\t",cnt);
//{sda_out,address} <= {address,1'b0};//shift most significations bit
////{buffer[7:1],sda_out} <= (buffer >> 1);//shift less most significations bit
////buffer <= buffer << 1; most significations bit 
////sda_out <= buffer[7];
//end
//endtask
//
//task read_data();
//begin
////if(cnt == ADDRESS_SIZE)State <= ACK;
////cnt <= (cnt == ADDRESS_SIZE)? 0 : cnt+1;
//buffer <= {buffer[6:0],sda}; // shift data and read address
//$display("Slave buffer=0%b\t",buffer);
//$display("Slave repeat cnt=%d\t",cnt);
//end
//endtask
//
endmodule