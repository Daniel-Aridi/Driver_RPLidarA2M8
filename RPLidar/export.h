#pragma once


extern "C"
{
	__declspec(dllexport) bool openLidarPort(int portNum);

	__declspec(dllexport) void initLidar(int numofDataPairs);

	__declspec(dllexport) int* getPolarCoordinates();

	__declspec(dllexport) int* getCartesianCoordinates();
	
	__declspec(dllexport) bool closeLidarPort();

	__declspec(dllexport) bool resetLidar();
	
	__declspec(dllexport) void deleteLidar();
 }