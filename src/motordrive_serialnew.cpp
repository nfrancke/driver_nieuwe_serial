//#include <serial/serial.h>

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <termios.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

#include "ros/ros.h"


//#define O_RDWR 0x0002
//#define O_NOCTTY 0

using namespace std;

int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                ROS_ERROR("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        //tty.c_cflag &= ~CSTOPB;
        tty.c_cflag |= CSTOPB;			//2 stop bits
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                ROS_ERROR ("error %d from tcsetattr", errno);
                return 1;
        }
        return 0;
}

void set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                ROS_ERROR ("error %d from tggetattr", errno);
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                ROS_ERROR ("error %d setting term attributes", errno);
}


int main(int argc, char **argv  )
{
	//initialize ROS
	ros::init(argc, argv, "uart");

	//define a nodehandle and define the rate for the "ros while loop"
	ros::NodeHandle nh;
	ros::Rate rate(100);

	ROS_INFO("Ros is initialized");

//	char *portname = "/dev/ttyS8";

	int fd = open("/dev/ttyS8", O_RDWR | O_NOCTTY | O_SYNC);
	ROS_INFO("fd = %i", fd);

	if (fd < 0){
	        ROS_ERROR ("error %d opening %s: %s", errno, "/dev/ttyS8", strerror (errno));
	        return -1;
	}
	ROS_INFO("opened port");

	set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
	set_blocking (fd, 0);                // set no blocking


	char kees = 1;

	while(ros::ok())
     {
     	ROS_INFO("we draaien");
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


//		write(fd, "hello!\n", 7);           // send 7 character greeting
		write(fd, out_buf, 8);
		char buf [100];
		int n = read(fd, buf, sizeof buf);  // read up to 100 characters if ready to read
		
		ROS_INFO("%c", buf[0]);

	    rate.sleep();
	 }

	 close(fd);
	 ROS_INFO("port is closed");

//     while(1)
//     {
//         char next_byte;
//         serial_port.get(next_byte);  HERE I RECEIVE THE FIRST ANSWER
//         std::cerr << next_byte;
//
//     }

     return EXIT_SUCCESS ;

}
