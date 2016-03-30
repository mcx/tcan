/*
 * SocketBusOptions.hpp
 *
 *  Created on: Mar 27, 2016
 *      Author: Philipp Leemann
 */

#ifndef SOCKETBUSOPTIONS_HPP_
#define SOCKETBUSOPTIONS_HPP_

#include <string>

#include "yalc/BusOptions.hpp"

class SocketBusOptions : public BusOptions {
public:

	SocketBusOptions():
		BusOptions(),
		interface(),
		loopback(false),
		baudrate(125),
		sndBufLength(0)
	{

	}

	SocketBusOptions(const std::string& interface_name, const unsigned int baud_rate):
		BusOptions(),
		interface(interface_name),
		loopback(false),
		baudrate(baud_rate),
		sndBufLength(0)
	{

	}

	virtual ~SocketBusOptions() { }

	//!  name of the network interface to bind to.
	std::string interface;

	//! loop back sent messages
	bool loopback;

	//! baud rate of the bus in kbps
	unsigned int baudrate;

	//! length of the socket buffer. 0=default. If the txqueuelen of the netdevice cannot be changed (default=10), set this value to prevent ENOBUFS errors when writing.
	// The minimum length is 1024, set 0 to keep the default
	unsigned int sndBufLength;

};

#endif /* SOCKETBUSOPTIONS_HPP_ */
