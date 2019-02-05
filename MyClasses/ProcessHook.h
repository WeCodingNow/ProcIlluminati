#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>

#include "ProcessMemory.h"

class ProcessHook
{
private:
	std::string processName;
	HANDLE snapshotHandle;
	HANDLE processHandle;
	bool isHooked;
public:
	ProcessHook() = delete;
	ProcessHook(std::string);
	~ProcessHook();
	bool IsHooked();
	Memory_T ReadMemory(unsigned int, unsigned int); //возвращает {arg1} байт в little endian
	void WriteMemory(unsigned int, Memory_T);
	void WriteMemory(unsigned int, std::string);
};
