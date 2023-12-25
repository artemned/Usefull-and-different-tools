`define CPU_CLK 50_000_000 //Current chip frequency
`define DIV_CLK 100_000 // Frequency need for s_clk

module i2c_fclk
(
 input clk,
 input rst,
 output reg s_clk,
 output reg strob_up,
 output reg strob_down 
);

localparam PERIOD_SIZE = (`CPU_CLK / `DIV_CLK); // Period s_clk 
localparam COUNT_SIZE = $clog2(PERIOD_SIZE); 

localparam RISE_SIZE = (PERIOD_SIZE / 2)- 1, //RISE_SIZE - 1  cycle for str_up		   
           FALLING_SIZE = (PERIOD_SIZE / 2)- 1;// FALLING_SIZE -1 cycle for str_down

localparam STR_STATE_UP = 0,
           STR_STATE_DOWN =1,
		   DOWN_STATE = 2,
		   UP_STATE   = 3;

reg[1:0] State;
reg[COUNT_SIZE-1:0] cnt; // Size T/2


always@(posedge clk or negedge rst)
begin
 if(~rst)
 begin
 State <= STR_STATE_DOWN;         // DOWN_STATE;
 cnt <= {RISE_SIZE{1'b0}}; //{FALLING_SIZE{1'b0}}; 
 end
 
 else begin 
  case(State)
  
  STR_STATE_DOWN: begin
  
    State <= DOWN_STATE;//UP_STATE;   
	cnt <= 0;
    strob_down <= 1'b1;//1'b0;
	
 end

  DOWN_STATE: begin
  strob_down <= 1'b0;
  if(cnt == FALLING_SIZE)
  begin
   State <= STR_STATE_UP;//STR_STATE_UP;
   s_clk <= 1'b1; 
  end
  cnt <= (cnt == FALLING_SIZE)? 1'b0 : cnt + 1; // be count up to next
  end
  
  STR_STATE_UP:begin
  State <= UP_STATE;
  cnt <= 0;
  strob_up <= 1'b1;
  end

  UP_STATE: begin
  strob_up = 1'b0;
  if(cnt == RISE_SIZE)
  begin
  State <= STR_STATE_DOWN;//DOWN_STATE;
  s_clk <= 1'b0;  
 end
 cnt <= (cnt == RISE_SIZE)? 1'b0 : cnt + 1;
 end
 
 endcase
 
 end
 
end// always end


endmodule