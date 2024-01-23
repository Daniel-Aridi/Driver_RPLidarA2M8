#include <Windows.h>
#include <math.h>
#include <iostream>
#include <string>
#include "Serial.h"


HANDLE Serial::openCommPort(int portNumber) {


    std::string format = "\\\\.\\com";

    std::string fPortName = format + std::to_string(portNumber);

    std::wstring widestr = std::wstring(fPortName.begin(), fPortName.end());
    const wchar_t* portName = widestr.c_str();


    _hComm = CreateFile(portName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0, // FILE_ATTRIBUTE_NORMAL
        NULL);

    if (_hComm == INVALID_HANDLE_VALUE) { return INVALID_HANDLE_VALUE; }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

    if (!(SetCommState(_hComm, &dcbSerialParams))) { return INVALID_HANDLE_VALUE; }

    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!(SetCommTimeouts(_hComm, &timeouts))) { return INVALID_HANDLE_VALUE; }

    return _hComm;
}

bool Serial::closeCommPort(HANDLE handle) {

    this->_hComm = handle;

    if (_hComm != INVALID_HANDLE_VALUE) {

        CloseHandle(_hComm);
        _hComm = INVALID_HANDLE_VALUE;
        return true;
    }
    else
    {
        return false;
    }
}