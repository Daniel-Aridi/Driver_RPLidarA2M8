#pragma once
#ifndef _SERIAL
#define _SERIAL

#include <Windows.h>

class Serial
{
private:

    DCB dcbSerialParams{ 0 };
    COMMTIMEOUTS timeouts{ 0 };
    HANDLE _hComm{ 0 };

public:

    HANDLE openCommPort(int portNumber);
    bool closeCommPort(HANDLE handle);
};


#endif // !LIDAR_SERIAL_METHODS
