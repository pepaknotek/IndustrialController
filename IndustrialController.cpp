#include "TimeUtils.h"
#include <iostream>
#include "TemperatureSensor.h"
#include "Controller.h"
#define PocetTeplot 12

int main()
{
    std::cout << "Industrial Controller starting...\n";
    TemperatureSensor temperatureSensor;
    Controller controller;
    double temperature;
    double teploty[PocetTeplot] = { 75, 65, 75, 95, 85, 65, 75, 95, 85, 65, 75, 65 };
    for (int i = 0; i < PocetTeplot; i++) {
        temperatureSensor.setTemperature(teploty[i]);
        temperature = temperatureSensor.readSensor();
        controller.update(temperature);
        std::cout << "Temperature: " << temperature << std::endl;
        std::cout << "System state: " << controller.toString(controller.getState()) << std::endl;
    }
   


    return 0;
}