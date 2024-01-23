
#include <iostream>
#include "RPLidar.h"
#include "Serial.h"
#include "export.h"


Serial serial;
HANDLE handle;
RPLidar* lidar;

bool openLidarPort(int portNum)
{
	handle = serial.openCommPort(portNum);
	if (handle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	return true;
}

bool closeLidarPort()
{
	if (handle != INVALID_HANDLE_VALUE)
	{
		serial.closeCommPort(handle);
		return true;
	}
	return false;
}

void initLidar(int numofDataPairs)
{
	lidar = new RPLidar(numofDataPairs, handle);
}

int* getPolarCoordinates()
{
	if (handle == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	return lidar->getPolarPoints();
}

int* getCartesianCoordinates()
{
	if (handle == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	return lidar->getCartesianPoints();
}

bool resetLidar()
{
	return lidar->reset();
}

void deleteLidar()
{
	delete lidar;
}




void main()
{
	std::cout << openLidarPort(3);

	initLidar(310);
	
	int* var = getCartesianCoordinates();


	if (var != 0)
	{
		for (size_t i = 0; i < 620; i++)
		{
			std::cout << var[i] << "  ";
			i++;
			std::cout << var[i] << std::endl;
		}
	}
	delete[] var;
	deleteLidar();

}