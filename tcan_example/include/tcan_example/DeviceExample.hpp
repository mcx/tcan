/*!
 * @file 	Device.hpp
 * @brief
 * @author 	Christian Gehring
 * @date 	Jan, 2012
 * @version 1.0
 * @ingroup robotCAN, device
 *
 */

#pragma once

#include <stdint.h>
#include <atomic>

#include "tcan/DeviceCanOpen.hpp"

namespace tcan {

namespace example_can {
//! An example device that is connected via CAN.

class DeviceExampleOptions : public DeviceCanOpenOptions {
public:
	DeviceExampleOptions() = delete;
	DeviceExampleOptions(const uint32_t nodeId, const std::string& name):
		DeviceCanOpenOptions(nodeId, name),
		someParameter(0)
	{

	}

	unsigned int someParameter;
};


class DeviceExample : public DeviceCanOpen {
public:

	/*! Constructors
	 * @param nodeId	ID of CAN node
	 * @param name		name of the device
	 */
	DeviceExample() = delete;
	DeviceExample(const uint32_t nodeId, const std::string& name);
	DeviceExample(DeviceExampleOptions* options);

	//! Destructor
	virtual ~DeviceExample();

	virtual bool initDevice();

	virtual void configureDevice();

	void setCommand(const float value);

	bool parsePdo1(const CanMsg& cmsg);

	float getMeasurement() const { return myMeasurement_; }

	/*! Handle a SDO answer
	 * this function is automatically called by parseSDO(..) and provides the possibility to save data from read SDO requests
	 * @param index		index of the SDO
	 * @param subIndex	subIndex of the SDO
	 * @param data		data of the answer to the read request (4 bytes)
	 */
	virtual void handleReadSDOAnswer(const uint16_t index, const uint8_t subIndex, const uint8_t *data);

protected:
	std::atomic<float> myMeasurement_;
};

} /* namespace example_can */

} /* namespace tcan */