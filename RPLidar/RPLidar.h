#pragma once
#ifndef _RPLidar
#define _RPLidar

#include <Windows.h>

class  RPLidar
{
public:

    RPLidar(int numofPoints, HANDLE handle);

    ~RPLidar();

    int* getPolarPoints();
    int* getCartesianPoints();
    bool reset();

private:

    bool loopScan();
    void getPolarCoordiates();
    void getCartesianCoordinates();
    void shiftPointstoPositive();
    bool stopScan();
    bool getLidarRawData();
    int smallestX();
    int smallestY();

    int _rawSize{ 0 };
    int _numofPoints{ 0 };
    int* rawDataBuffer;
    int* processedBuffer;

    HANDLE _hComm;
    byte readData{ 0 };
    DWORD noBytesRead{ 0 };
    DWORD noBytesWriten{ 0 };

    static const byte START_FLAG[1];
    static const byte STOP[2];
    static const byte RESET[2];
    static const byte SCAN[2];
    static const byte EXPRESS_SCAN[2];
    static const byte FORCE_SCAN[2];
    static const byte GET_INFO[2];
    static const byte GET_HEALTH[2];
    static const byte GET_SAMPLERATE[2];
    static const byte GET_LIDAR_CONF[2];
};


#endif // !_RPLidar