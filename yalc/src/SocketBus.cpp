/*!
 * @file 	Bus.cpp
 * @brief	Type definitions
 * @author 	Christian Gehring
 * @date 	Jan, 2012
 * @version 1.0
 * @ingroup robotCAN, bus
 *
 */

#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#include "yalc/SocketBus.hpp"

SocketBus::SocketBus(const std::string& interface):
	Bus(),
	interface_(interface),
	socket_(),
	baudRate_(125)
{
}

SocketBus::~SocketBus()
{
	closeBus();
}

bool SocketBus::initializeBus()
{
	/* **************************
	 * CAN DRIVER SETUP
	 ***************************/
	/* open channel */
	int fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if(fd < 0) {
		printf("Opening CAN channel %s failed: %d\n", interface_.c_str(), fd);
		return false;
	}

	//Configure the socket:
	struct ifreq ifr;
	strcpy(ifr.ifr_name, interface_.c_str());
	ioctl(fd, SIOCGIFINDEX, &ifr);

	int loopback = 0; /* 0 = disabled, 1 = enabled (default) */
	setsockopt(fd, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));
	int recv_own_msgs = 0; /* 0 = disabled (default), 1 = enabled */
	setsockopt(fd, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS,&recv_own_msgs, sizeof(recv_own_msgs));

	// CAN error handling
	can_err_mask_t err_mask = CAN_ERR_MASK; //( CAN_ERR_TX_TIMEOUT | CAN_ERR_BUSOFF );
	setsockopt(fd, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));

	//Bind
	struct sockaddr_can addr;
	memset(&addr, 0, sizeof(struct sockaddr_can));
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	//printf("%s at index %d\n", channelname, ifr.ifr_ifindex);
	if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		printf("Error in socket %s bind.\n", interface_.c_str());
		return false;
	}

	//Set nonblocking:
	int flags;
	if (-1 == (flags = fcntl(fd, F_GETFL, 0))) flags = 0;
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	socket_ = {fd, POLLIN, 0};

	return true;
}

bool SocketBus::closeBus() {
	close(socket_.fd);
	return true;
}


bool SocketBus::readCanMessage() {

	const int ret = poll( &socket_, 1, 1000 );

	if ( ret == -1 ) {
		printf("poll failed");
		perror("poll()");
		return false;
	}else if ( ret == 0 || !(socket_.revents & POLLIN) ) {
		/*******************************************************
		 * no data received
		 *******************************************************/
	}else{
		/*******************************************************
		 * READ INCOMING CAN MESSAGES
		 *******************************************************/
		struct can_frame frame;
		socket_.revents = 0;
		bool dataAvailable=true;
		do {
			int bytes_read = read( socket_.fd, &frame, sizeof(struct can_frame));
			// printf("CanManager_ bytes read: %i\n", bytes_read);

			if(bytes_read<=0) {
				//  printf("CanManager:bus_routine: Data buffer empty or error\n");
				dataAvailable=false;
			} else {
				// printf("CanManager:bus_routine: Data received from iBus %i, n. Bytes: %i \n", iBus, bytes_read);

				handleMessage( CANMsg(frame.can_id, frame.can_dlc, frame.data) );
			}
		} while(dataAvailable);

	}


	return true;
}


bool SocketBus::writeCanMessage(std::unique_lock<std::mutex>& lock, const CANMsg& cmsg) {

	usleep((cmsg.getLength()*8+44)/baudRate_*1000); // sleep the amount of time the message needs to be transmitted
	return true;
}
