#pragma once
#include <stdint.h>
#include <mcp_can.h>

class CanReporter {
public:
    // csPin            = Chip Select pin modulu MCP2515 (SPI)
    // reportIntervalMs = jak často posílat heartbeat rámec (např. 100 ms)
    CanReporter(uint8_t csPin, unsigned long reportIntervalMs = 100);

    // Inicializuje MCP2515 (SPI, rychlost sběrnice). Volat AŽ ze setup(),
    // ne z konstruktoru.
    bool begin();

    // Zvenčí se tímto předají aktuální data k odeslání - obdoba
    // BuzzerAlarm::setStatus(), jen místo jedné hodnoty tři najednou.
    void setData(uint8_t state, bool faultFlag, double temperature);

    // Neblokující - pošle rámec, jakmile uplyne reportIntervalMs.
    void update();

    // DOČASNÉ pro loopback test - zkontroluje, jestli nepřišel rámec zpátky
    // (v loopback režimu se tam vrací to, co sami odešleme). Vrací true,
    // pokud byl rámec přijat, a vyplní outId/outBuf/outLen. Smazat/nahradit,
    bool checkLoopback(unsigned long &outId, uint8_t outBuf[8], uint8_t &outLen);

private:
    MCP_CAN can;
    unsigned long reportIntervalMs;
    unsigned long lastSendMillis = 0;

    uint8_t state = 0;
    bool faultFlag = false;
    double temperature = 0.0;
};