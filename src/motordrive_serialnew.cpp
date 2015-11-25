#include <serial/serial.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <termios.h>

#include "ros/ros.h"

using namespace std;
using namespace serial;

int main(int argc, char **argv  )
{
	//initialize ROS
	ros::init(argc, argv, "uart");

	//define a nodehandle and define the rate for the "ros while loop"
	ros::NodeHandle nh;
	ros::Rate rate(100);

	ROS_INFO("Ros is initialized");


     //
     // Do not skip whitespace characters while reading from the
     // serial port.
     //
     // serial_port.unsetf( std::ios_base::skipws ) ;
     //
     // Wait for some data to be available at the serial port.
     //
     //
     // Keep reading data from serial port and print it to the screen.
     //
     // Wait for some data to be available at the serial port.
   

  //
  //    while( serial_port.rdbuf()->in_avail() > 0 )

	char kees = 1;

	while(ros::ok())
     {
		char out_buf[8];
		
		//Define data
		out_buf[0] = 0x5a;
		out_buf[1] = 0xaa;
		out_buf[2] = 0x03;
		out_buf[3] = 0x30;
		out_buf[4] = 0x00;
		out_buf[5] = 0x00;
		out_buf[6] = 0x00;
		out_buf[7] = 0x00;
		
		//write data to serial port.
		//serial.write(0x5aaa033000000000);
		//serial.write("0x90, 0x170, 0x03, 0x48, 0x00, 0x00, 0x00, 0x00");

	    rate.sleep();
	 }

//     while(1)
//     {
//         char next_byte;
//         serial_port.get(next_byte);  HERE I RECEIVE THE FIRST ANSWER
//         std::cerr << next_byte;
//
//     }

     return EXIT_SUCCESS ;

}
