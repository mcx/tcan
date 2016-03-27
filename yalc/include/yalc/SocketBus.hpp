/*
 * SocketBus.hpp
 *
 *  Created on: Mar 27, 2016
 *      Author: Philipp Leemann
 */

#ifndef SOCKETBUS_HPP_
#define SOCKETBUS_HPP_

#include <poll.h>
#include <string>

#include "yalc/Bus.hpp"
#include "yalc/SocketBusOptions.hpp"

class SocketBus : public Bus {
public:

	SocketBus() = delete;
	SocketBus(const std::string& interface, const unsigned int baudrate);
	SocketBus(const SocketBusOptions& options);

	virtual ~SocketBus();

	bool closeBus();

	virtual bool initializeBus();

protected:
	virtual bool readCanMessage();
	virtual bool writeCanMessage(std::unique_lock<std::mutex>& lock, const CANMsg& cmsg);

protected:
	std::string interface_;

	pollfd socket_;

	unsigned int baudRate_;
};

#endif /* SOCKETBUS_HPP_ */
