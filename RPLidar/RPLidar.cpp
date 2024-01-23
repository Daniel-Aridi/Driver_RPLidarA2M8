#include <Windows.h>
#include <math.h>
#include <iostream>
#include <string>
#include "RPLidar.h"

#define TO_RAD 0.0174527f 
 

RPLidar::RPLidar(int numofPoints, HANDLE handle)
{
    _numofPoints = numofPoints * 2;
    _hComm = handle;
    _rawSize = (((numofPoints) * 5) + 7);

    rawDataBuffer = new int[_rawSize];
    processedBuffer = new int[_numofPoints];
}



RPLidar::~RPLidar()
{
    delete[] rawDataBuffer;
    delete[] processedBuffer;
}

int* RPLidar::getPolarPoints()
{
    loopScan();
    getPolarCoordiates();
    return processedBuffer;
}

int* RPLidar::getCartesianPoints()
{
    loopScan();
    getPolarCoordiates();
    getCartesianCoordinates();
    return processedBuffer;
}

bool RPLidar::reset() {

    if (!(WriteFile(_hComm, (void*)RESET, 2, &noBytesWriten, NULL)))
    {
        return false;
    }
    Sleep(3);
    return true;
}


//private
bool RPLidar::getLidarRawData() {

    for (int i = 0; i < _rawSize; i++)
    {
        if (ReadFile(_hComm, &readData, 1, &noBytesRead, NULL))
        {
            rawDataBuffer[i] = readData;
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool RPLidar::loopScan() {

    PurgeComm(_hComm, PURGE_RXCLEAR);

    if (!(WriteFile(_hComm, (void*)SCAN, 2, &noBytesWriten, NULL)))
    {
        return false;
    }

    if (!(getLidarRawData()))
    {
        return false;
    }

    if (!stopScan())
    {
        return false;
    }

    return true;
}

void RPLidar::getPolarCoordiates() {

    int temp{ 0 };
    int j = 9;

    for (int x = 0; x < _numofPoints;)
    {

        temp = 0;
        temp = rawDataBuffer[j] << 8;
        temp |= rawDataBuffer[j - 1];
        temp >>= 1;
        processedBuffer[x] = temp / 64; //for angle in degree.

        j += 2;
        temp = rawDataBuffer[j] << 8;
        temp |= rawDataBuffer[j - 1];
        x++;
        processedBuffer[x] = temp / 40;  //for distance in cm.

        x++;
        j += 3;
    }
}

void RPLidar::getCartesianCoordinates() {

    int temp{ 0 };

    for (short x = 0; x < _numofPoints; x += 2)
    {
        temp = processedBuffer[x];

        processedBuffer[x] = processedBuffer[x + 1] * cos(temp * TO_RAD);
        processedBuffer[x + 1] = processedBuffer[x + 1] * sin(temp * TO_RAD);
    }
}

void RPLidar::shiftPointstoPositive() {

    int sX = abs(smallestX());
    int sY = abs(smallestY());

    for (int i = 0; i < _numofPoints; i++) {

        processedBuffer[i] += sX;
        processedBuffer[++i] += sY;
    }
}

bool RPLidar::stopScan() {

    if (!(WriteFile(_hComm, (void*)STOP, 2, &noBytesWriten, NULL)))
    {
        return false;
    }
    Sleep(3);
    return true;
}

int RPLidar::smallestX() {

    int temp = processedBuffer[0];

    for (int n = 2; n < _numofPoints; n += 2) {

        if (temp < processedBuffer[n]) {

            temp = processedBuffer[n];
        }
    }

    return temp;
}

int RPLidar::smallestY() {

    int temp = processedBuffer[1];

    for (int n = 3; n < _numofPoints; n += 2) {

        if (temp < processedBuffer[n]) {

            temp = processedBuffer[n];
        }
    }

    return temp;
}



const byte RPLidar::START_FLAG[1]{ 0xA5 };
const byte RPLidar::STOP[2]{ 0xA5, 0x25 };
const byte RPLidar::RESET[2]{ 0xA5, 0x40 };
const byte RPLidar::SCAN[2]{ 0xA5, 0x20 };
const byte RPLidar::EXPRESS_SCAN[2]{ 0xA5, 0x82 };
const byte RPLidar::FORCE_SCAN[2]{ 0xA5, 0x21 };
const byte RPLidar::GET_INFO[2]{ 0xA5, 0x50 };
const byte RPLidar::GET_HEALTH[2]{ 0xA5, 0x52 };
const byte RPLidar::GET_SAMPLERATE[2]{ 0xA5, 0x59 };
const byte RPLidar::GET_LIDAR_CONF[2]{ 0xA5, 0x84 };