#pragma once
#include <Windows.h>
#include <string>

class ControllerBox {
public:
	ControllerBox(std::string serialPort);
	~ControllerBox() = default;
private:
	HANDLE hSerial;
	std::string serialPort;
	std::string availablePorts();
	void createSerial();
	void run();

};