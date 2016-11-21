/*
 * XbeeUsb.cpp
 *
 *  Created on: Mar 27, 2016
 *      Author: Philipp Leemann
 */


#include "tcan_example/XbeeUsb.hpp"
#include "message_logger/message_logger.hpp"

namespace tcan_example {

XbeeUsb::XbeeUsb(tcan::UniversalSerialBusOptions* options):
    tcan::UniversalSerialBus(options)
{

}

XbeeUsb::~XbeeUsb()
{
}

void XbeeUsb::handleMessage(const tcan::UsbMsg& msg) {
    std::cout << " got data: " << msg.getData() << std::endl;
}

} /* namespace tcan */
