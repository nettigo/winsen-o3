//
// Created by viciu on 6/28/24.
//
#include "winsen-o3.h"


WinsenO3::WinsenO3() {
    initialized = false;
    total = 0;
    sample_count = 0;
}

void WinsenO3::begin(int8_t rx, int8_t tx) {
    sensor = new SoftwareSerial;
    sensor->begin(9600, SWSERIAL_8N1, rx, tx, false);
//    sensor->enableIntTx(false);
    initialized = true;
}

bool WinsenO3::available() {
    if (!initialized) return false;
    if (sample_count) return true;
    return false;
}

void WinsenO3::process() {
    if (!initialized) {
        Serial.println(F("not inited"));
        delay(20);
        return;

    }
    readData();
}

void WinsenO3::readData() {
    yield();
    if (sensor->available()) {
        byte r = sensor->read();
        static byte idx = 0;
        static unsigned value = 0;
        static unsigned value2 = 0;
        switch (state) {
            case IDLE:
                if (r == 0xFF) {
                    idx = 0;
                    stream[idx] = r;
                    state = HDR1;
                }
                break;
            case HDR1:
                if (r == 0x2a) { //03 code
                    stream[++idx] = r;
                    state = HDR2;
                } else state = IDLE;
                break;
            case HDR2:
                if (r == 0x04) { //ppb as unit
                    stream[++idx] = r;
                    state = INSTR;
                } else state = IDLE;
                break;
            case INSTR:
                if (r == 0x0) { // to be explained
                    stream[++idx] = r;
                    state = DAT1;
                } else state = IDLE;
                break;
            case DAT1:
                stream[++idx] = r;
                state = DAT2;
                value = r * 255;
                break;
            case DAT2:
                stream[++idx] = r;
                state = DAT3;
                value += r;
//            Serial.print(F("Value:"));
//            Serial.println(value);
                break;
            case DAT3:
                stream[++idx] = r;
                state = DAT4;
                value2 = r * 256;
                break;
            case DAT4:
                stream[++idx] = r;
                state = CHECKSUM;
                value2 += r;
//            Serial.print(F("Value:"));
//            Serial.println(value2);
                break;
            case CHECKSUM:
                byte sum = checksum();
//            Serial.println();
//            Serial.print(millis()/1000);
//            Serial.print(F(" "));
                if (sum == r) {
                    total += value;
                    sample_count++;
                } else {
//                    Serial.print(F("CHECKSUM ERROR: "));
//                    Serial.print(sum, 16);
//                    Serial.print(F("/"));
//                    Serial.println(r, 16);
                }
                state = IDLE;

                break;
        }
    }
}



byte WinsenO3::checksum() {
    unsigned char j, tempq = 0;
    for (j = 0; j <= 7; j++) {
        tempq += stream[j];
    }
    tempq = (~tempq);
    return (tempq);
}

unsigned long WinsenO3::currentValue() {
    if (!available()) return 0;
    return total / sample_count;
}

void WinsenO3::restart() {
    sample_count = total = 0;
}

unsigned WinsenO3::sampleCount() {
    return sample_count;
}