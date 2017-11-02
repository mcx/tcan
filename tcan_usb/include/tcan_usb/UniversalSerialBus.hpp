/*
 * Bus.hpp
 *
 *  Created on: Mar 27, 2016
 *      Author: Philipp Leemann
 */

#pragma once

#include <termios.h> // tcgettatr
#include <memory>

#include "tcan/Bus.hpp"
#include "tcan_usb/UniversalSerialBusOptions.hpp"
#include "tcan_usb/UsbMsg.hpp"

namespace tcan_usb {

class UniversalSerialBus : public tcan::Bus<UsbMsg> {
 public:
    UniversalSerialBus() = delete;
    UniversalSerialBus(std::unique_ptr<UniversalSerialBusOptions>&& options);

    virtual ~UniversalSerialBus();

    void sanityCheck();

    virtual int getPollableFileDescriptor() { return fileDescriptor_; }

protected:
    virtual bool initializeInterface();
    virtual bool readData();
    virtual bool writeData(std::unique_lock<std::mutex>* lock);

 private:
    void configureInterface();

 private:
    int fileDescriptor_;

    termios savedAttributes_;

    unsigned int deviceTimeoutCounter_;
};

} /* namespace tcan_usb */
