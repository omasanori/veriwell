/*****************************************************************************
 * Copyright 1994-2005, Elliot Mednick and Mark Hummel
 * This file is part of Veriwell.
 *
 * Veriwell is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Veriwell is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *****************************************************************************/

`timescale 1 ns /1 ps
module inv(o,i);
input i;
output o;

assign o = ~i;

endmodule

module test;
reg clk;
wire clk1;
wire clk2;
wire clk3;
integer count;
time	t1;
event xevent;
real  xyz;

initial begin
//	$lxt2_recordvars("space");
	$lxt_recordvars("speed");
	xyz = 0.0001;
	#1
	clk = 0;
	count = 0;
	#500000 $lxt_recordoff();
	#500000 $lxt_recordon();
	#10000000 $finish;
end

always begin
	clk = 0;
	clk <= #5 1;
	#10;
end

always @(posedge clk) begin
	t1 = $time;
	count = count+1;
	xyz = xyz + xyz;
	-> xevent;
end

inv i0(clk1,clk);
inv i1(clk2,clk1);
inv i2(clk3,clk2);

endmodule

