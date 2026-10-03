#include "CanReporter.h"
#include <SPI.h>

#define CAN_REPORT_ID 0x100 // vlastni ID pro tento status ramec

CanReporter::CanReporter(uint8_t csPin, unsigned long reportIntervalMs)
    : can(csPin), reportIntervalMs(reportIntervalMs)
{
}

bool CanReporter::begin()
{
    if (can.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
        return false;
    }
    // Zatim loopback - zadny druhy uzel na sbernici, testujeme jen ze
    // se ramce spravne sestavuji a odesilaji. Az bude k dispozici druhy
    // uzel/analyzator, prepnout na MCP_NORMAL.
    can.setMode(MCP_LOOPBACK);
    return true;
}

void CanReporter::setData(uint8_t newState, bool newFaultFlag, double newTemperature)
{
    state = newState;
    faultFlag = newFaultFlag;
    temperature = newTemperature;
}

void CanReporter::update()
{
    unsigned long now = millis();
    if (now - lastSendMillis < reportIntervalMs) {
        return;
    }
    lastSendMillis = now;

    int16_t tempScaled = (int16_t)(temperature * 10); // napr. 21.6 C -> 216

    uint8_t buf[8];
    buf[0] = state;
    buf[1] = faultFlag ? 1 : 0;
    buf[2] = (uint8_t)(tempScaled >> 8);   // horni bajt
    buf[3] = (uint8_t)(tempScaled & 0xFF); // dolni bajt
    buf[4] = 0;
    buf[5] = 0;
    buf[6] = 0;
    buf[7] = 0;

    can.sendMsgBuf(CAN_REPORT_ID, 0, 8, buf); // 0 = standardni (11-bit) ramec
}

bool CanReporter::checkLoopback(unsigned long &outId, uint8_t outBuf[8], uint8_t &outLen)
{
    if (can.checkReceive() != CAN_MSGAVAIL) {
        return false;
    }
    can.readMsgBuf(&outId, &outLen, outBuf);
    return true;
}