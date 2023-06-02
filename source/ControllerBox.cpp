
#include "../include/ControllerBox.h"



ControllerBox::ControllerBox(std::string serialPort) {



	this->serialPort = serialPort;
	this->createSerial();

}


void ControllerBox::createSerial() {



	// Creating serial handle to read serial port defined in serialPort
	this->hSerial = CreateFile(this->serialPort,
								GENERIC_READ | GENERIC_WRITE,
								0,
								0
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								0);
	if (this->hSerial == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			std::cerr << "File was not found, ERROR # " << GetLastError() << std::endl;
			return;
		}

		std::cerr << "Error while creating serial handle, Error # " << GetLastError() << std::endl;
		return;
	}

	// Set parameters for serial handle (baud rate, byte size, stop bits, parity)
	DCB dcbSerialParameters = {0};

	dcbSerialParameters.DCBlength = sizeof(dcbSerialParameters);

	if (!GetCommState(this->hSerial, &dcbSerialParameters)){
		
		std::cerr << "Error getting state of serial port, Error # " << GetLastError() << std::endl;
	}

	dcbSerialParameters.BaudRate = 1000000; //This is no standard baud rate --> should be fixed
	dcbSerialParameters.ByteSize = 8;
	dcbSerialParameters.StopBits = ;
	dcbSerialParameters.Parity = ;

	if (!SetCommState(hSerial, &dcbSerialParameters)) {

		std::cerr << "Error getting state of serial port, Error # " << GetLastError() << std::endl;
	}


}


list<std::string> ControllerBox::availablePorts() {

	char lpTargetPath[5000]; // buffer to store the path of the COMPORTS
	list<std::string> ports;


	for (int i = 0; i < 255; i++) // checking ports from COM0 to COM255
	{
		std::string str = "COM" + std::to_string(i); // converting to COM0, COM1, COM2
		DWORD test = QueryDosDevice(str.c_str(), lpTargetPath, 5000);

		// Test the return value and error if any
		if (test != 0) //QueryDosDevice returns zero if it didn't find an object
		{
			//std::cout << str << ": " << lpTargetPath << std::endl;
			ports.push_back(str);
		}

		if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
		}
	}

	return ports;

}



void ControllerBox::run() {


	// Wait for new line and save line to file.






}