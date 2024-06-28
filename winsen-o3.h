//
// Created by viciu on 6/28/24.
//

#ifndef WINSEN_O3_H
#define WINSEN_O3_H

#include <SoftwareSerial.h>

class WinsenO3 {
public:
    enum SensorState {
        IDLE,
        HDR1,
        HDR2,
        INSTR,
        DAT1,
        DAT2,
        DAT3,
        DAT4,
        CHECKSUM,
//        PRESS_RESP,
//        PRESS_RESP2,
//        PRESS_RESP3,
//        PRESS_RESP4,

    };
    WinsenO3();
    void begin(int8_t rx, int8_t tx);
    unsigned long currentValue();
    unsigned sampleCount();
    bool available();
    void process();
    void restart();

private:
    SoftwareSerial *sensor;
    bool initialized;
    unsigned long total;
    unsigned sample_count;
    byte stream[25];
    SensorState state;

    void readData();
    byte checksum();


};

#endif
