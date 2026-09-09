#include "TimeUtils.h"
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>



std::string getTimestamp() {
	tm tmStruct;
	std::time_t timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	localtime_s(&tmStruct, &timeNow);
	std::ostringstream oss;
	oss << std::put_time(&tmStruct, "%d.%m.%Y %H:%M:%S");
	return oss.str();
}