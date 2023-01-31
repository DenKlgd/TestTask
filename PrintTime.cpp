#include "PrintTime.h"

void printTime()
{
	auto sysClock = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(sysClock);
	char* timeStr = std::ctime(&time);
	timeStr[strlen(timeStr) - 1] = ' ';
	std::cout << timeStr;
}
