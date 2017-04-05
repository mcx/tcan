#pragma once


// tcan
#include <tcan/EtherCatDevice.hpp>

// tcan example
#include <tcan_example/Dsp402.hpp>


namespace tcan_example {


struct ElmoTwitterIndata
{
    Dsp402Statusword statusword;

    int32_t position = 0;
    int32_t velocity = 0;

    int32_t digitalin = 0;

    int32_t busvoltage = 0;
    int16_t motorcurrent = 0;
};

struct ElmoTwitterOutdata
{
    Dsp402Controlword controlword;

    int32_t torque = 0;
};


inline ElmoTwitterOutdata createOutdata(Dsp402Command command, double torque)
{
    // Set controlword data
    static Dsp402Controlword controlword;
    switch(command)
    {
        case Dsp402Command::SWITCH_ON:
            controlword.bits.switch_on = 1;
            break;

        case Dsp402Command::SHUTDOWN:
            controlword.bits.switch_on = 0;
            break;

        case Dsp402Command::DISABLE_VOLTAGE:
            controlword.bits.enable_voltage = 0;
            break;

        case Dsp402Command::ENABLE_VOLTAGE:
            controlword.bits.enable_voltage = 1;
            break;

        case Dsp402Command::QUICK_STOP:
            controlword.bits.quick_stop = 1;
            break;

        case Dsp402Command::DISABLE_OPERATION:
            controlword.bits.enable_operation = 0;
            break;

        case Dsp402Command::ENABLE_OPERATION:
            controlword.bits.enable_operation = 1;
            break;

        case Dsp402Command::FAULT_RESET:
            controlword.bits.fault_reset = 1;
            break;

        case Dsp402Command::HALT:
            controlword.bits.halt = 1;
            break;

        case Dsp402Command::HALT_RESET:
            controlword.bits.halt = 0;
            break;

        case Dsp402Command::CLEAR_CONTROLWORD:
            controlword.all = 0;
            break;
    }

    // Covert torque data to INT16 from double
    double rated_current = 20000.0;
    double rated_torque = (20000.0*0.27)*0.001;
    int16_t torque_data = (int16_t)(torque/rated_torque*1000.0);

    // Copy to internal struct
    ElmoTwitterOutdata data;
    data.controlword.all = controlword.all;
    data.torque = torque_data;
    return data;
}

template <typename Value>
void readValue(uint8_t* data, const uint16_t pos, Value& value) {
//    value = 0;
//    uint64_t buffer = 0;
//    const uint16_t len = sizeof(Value);
//    for (uint16_t i = 0; i < len; i++) {
////        printf("%i    %i    ", pos+i,   data[pos+i]);
////        printf("Read %2i: 0x%8x\n", i, (uint64_t((data[pos+i]) << uint64_t(i*8)) & (uint64_t(0xff) <<  uint64_t(i*8))));
////        printf("Read %2i: 0x%8x\n", i, uint64_t(data[pos+i]));
////        printf("Read %2i: 0x%8x\n", i, (uint64_t((data[pos+i]) << uint64_t(i*8))));
////        printf("Read %2i: 0x%8x\n", i, (uint64_t(0xff) <<  uint64_t(i*8)));
//        buffer |= (uint64_t((data[pos+i]) << uint64_t(i*8)) & (uint64_t(0xff) <<  uint64_t(i*8)));
//    }
//    value = buffer;

    value = 0;
    const uint16_t len = sizeof(Value);
    for (uint16_t i = 0; i < len; i++) {
        value |= static_cast<Value>(data[pos+i] << i*8);
    }
}

inline ElmoTwitterIndata createIndata(const tcan::EtherCatDatagram& datagram)
{
    // Get data
    uint8_t databuffer[21];
    memcpy(&databuffer[0], datagram.getData(), datagram.getDataLength());


    ElmoTwitterIndata data2;
    readValue(&databuffer[0], 0, data2.position);
    readValue(&databuffer[0], 4, data2.digitalin);
    readValue(&databuffer[0], 8, data2.velocity);
    readValue(&databuffer[0], 12, data2.statusword.all);
    readValue(&databuffer[0], 15, data2.busvoltage);
    readValue(&databuffer[0], 19, data2.motorcurrent);

    // Store data
    ElmoTwitterIndata data;
    data.statusword.all = ((databuffer[13] << 8 ) & 0xff00) | (databuffer[12] & 0x00ff);
    data.position = ((databuffer[3] << 24) & 0xff000000) | ((databuffer[2] << 16) & 0x00ff0000) | ((databuffer[1] << 8) & 0x0000ff00) | ((databuffer[0] << 0) & 0x000000ff);
    data.digitalin = ((databuffer[7] << 24) & 0xff000000) | ((databuffer[6] << 16) & 0x00ff0000) | ((databuffer[5] << 8) & 0x0000ff00) | ((databuffer[4] << 0) & 0x000000ff);
    data.velocity = ((databuffer[11] << 24) & 0xff000000) | ((databuffer[10] << 16) & 0x00ff0000) | ((databuffer[9] << 8) & 0x0000ff00) | ((databuffer[8] << 0) & 0x000000ff);
    data.busvoltage = ((databuffer[18] << 24) & 0xff000000) | ((databuffer[17] << 16) & 0x00ff0000) | ((databuffer[16] << 8) & 0x0000ff00) | ((databuffer[15] << 0) & 0x000000ff);
    data.motorcurrent = ((databuffer[20] << 8) & 0xff00) | ((databuffer[19] << 0) & 0x00ff);


//    printf("position 0x%8x 0x%8x\n", data.position, data2.position);
//    printf("digitalin 0x%8x 0x%8x\n", data.digitalin, data2.digitalin);
//    printf("velocity 0x%8x 0x%8x\n", data.velocity, data2.velocity);
//    printf("statusword 0x%8x 0x%8x\n", data.statusword.all, data2.statusword.all);
//    printf("busvoltage 0x%8x 0x%8x\n", data.busvoltage, data2.busvoltage);
//    printf("motorcurrent 0x%8x 0x%8x\n", data.velocity, data2.motorcurrent);
//    printf("Position 0 0x%8x\n", ((databuffer[0] << 0) & 0x000000ff));
//    printf("Position 1 0x%8x\n", ((databuffer[1] << 8) & 0x0000ff00));
//    printf("Position 1 0x%8x\n", ((uint32_t(databuffer[1]) << 8) & 0x0000ff00));
//    printf("Position 2 0x%8x\n", ((databuffer[2] << 16) & 0x00ff0000));
//    printf("Position 3 0x%8x\n", ((databuffer[3] << 24) & 0xff000000));
//    printf("%i    \n", databuffer[1]);

    return data;
}



class ElmoTwitter : public tcan::EtherCatDevice {
 public:
    ElmoTwitter(const uint32_t address, const std::string& name);

    void dumpSlaveStatusInfo();
    void configureSlave();
};


} // tcan_example
