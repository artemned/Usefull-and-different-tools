module i2c_slave
#(parameter DEVICE_ADDRESS = 8'b10101001) //10101001	
(
 input scl, //debug ware
 inout sda  //debug ware
);

parameter START_ST = 0,
          ADDRESS_ST = 1,
		  ACK_NACK_ST = 2,
          READ_ST = 3,
          WRITE_ST = 4,
		  STOP_ST = 5;
		  
parameter ADDRESS_SIZE = 7, // 6 bit address + 1 bit W/R
          READ_SIZE = 7,
		  WRITE_SIZE = 7;		  

wire sda_in;
reg  sda_out;
wire scl_in;
reg  scl_out;


wire[7:0] data;// data for master
reg [7:0] buffer=0; // ouput data from sda_in
reg [2:0] State;
reg [3:0] cnt=0;
wire[7:0] d_address = 8'b10101001;//DEVICE_ADDRESS;
reg [7:0] value=0;


initial State <= START_ST; // begin with start state

 
assign sda = ((State == ACK_NACK_ST)||(State == READ_ST)) 
             ? sda_out :1'bz; //when flag is true sda work as input 
 
assign sda_in =((State != ACK_NACK_ST)||(State != READ_ST)) ? sda : 1'bz;
always@* begin

case(State)
START_ST:begin
case({scl,sda}) //waiting for before master initial start
2'b10:begin
State = ADDRESS_ST;
$display("Slave Start!");
end
default:begin
State = START_ST;
$display("Cycle Start");  
end

endcase

end//START_ST
endcase
end //always


always@(posedge scl)begin
 case(State)
 ADDRESS_ST:begin 
 buffer <= {buffer[6:0],sda_in}; // shift data and read address
 cnt <= (cnt == ADDRESS_SIZE)? 0 : cnt+1;
 $display("Slave buffer=0%b\t",buffer);
 $display("Slave repeat cnt=%d\t",cnt);
 if(cnt == ADDRESS_SIZE)begin
 if(buffer === 7'b1010100)begin
  State <= ACK_NACK_ST;
  $display("Address slave equal");
  //sda_out <= 1;  
 end
end
 
 end // ADRRESS_ST
 
 
 WRITE_ST:begin
 $display("Slave write state!");
 //if(cnt == WRITE_SIZE)State <= STOP_ST;
 cnt <= (cnt == WRITE_SIZE)? 0 : cnt+1;
 //buffer <= {buffer[6:0],sda}; // read Address
 end
 
ACK_NACK_ST:begin
$display("Slave ACK_NACK_ST state!");
buffer <= {buffer[6:0],sda};
$display("Slave buffer=0%b\t",buffer);
sda_out =1;
end
 
 

endcase

end //always
endmodule
 