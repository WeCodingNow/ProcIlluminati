#include "ProcessHook.h"

ProcessHook::ProcessHook(std::string procName)
{
    isHooked = false;

    std::wstring wideProcName = std::wstring(procName.begin(), procName.end());
    processName = procName;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshotHandle, &entry) == TRUE)
	{
		while (Process32Next(snapshotHandle, &entry) == TRUE)
		{
            if (_wcsicmp(entry.szExeFile, wideProcName.c_str()) == 0)
			{
				processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				isHooked = true;
			}
		}
	}
}

ProcessHook::~ProcessHook()
{
    if(IsHooked())
        CloseHandle(processHandle);
	CloseHandle(snapshotHandle);
}

bool ProcessHook::IsHooked()
{
    LPDWORD tempPointer = new DWORD;
    GetExitCodeProcess(processHandle, tempPointer);
    if (*tempPointer != STILL_ACTIVE)
    {
        isHooked = false;
    }
    delete tempPointer;

	return isHooked;
}

Memory_T ProcessHook::ReadMemory(unsigned int startingPoint, unsigned int amount)
{
	Memory_T returnMem;
	LPCVOID memoryStart = LPCVOID(startingPoint);

    if (!IsHooked()) return returnMem;

	unsigned char *buf = (Byte*) malloc(amount * sizeof(Byte));
	SIZE_T *bufsize = new SIZE_T;
	*bufsize = sizeof(Byte) * amount;

    if(ReadProcessMemory(processHandle, memoryStart, buf, *bufsize, bufsize) == 0)
    {
        isHooked = false;
        return returnMem;
    }


	for (int i = 0; i < amount; i++)
	{
		returnMem.push_back(*(buf + i));
	}

	free(buf);
	return returnMem;
}

void ProcessHook::WriteMemory(unsigned int address, Memory_T newMemory)
{
	//превращаем Memory_T в unsigned char*
	Byte* temp = (Byte*)malloc(sizeof(Byte) * newMemory.size());
	for (unsigned int i = 0; i < newMemory.size(); i++)
	{
		*(temp + i) = newMemory[i];
	}
	WriteProcessMemory(processHandle, (LPVOID)address, temp, sizeof(Byte) * newMemory.size(), NULL);

	free(temp);
}

Memory_T stringToMemory(std::string input)
{
	Memory_T tempMem;
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == ' ')
		{
			continue;
		}
		auto tempVal = CharToHexedChar(input[i]) * 16;
		tempVal += CharToHexedChar(input[i + 1]);
		tempMem.push_back(tempVal);
		i += 1;
	}

	return tempMem;
}

void ProcessHook::WriteMemory(unsigned int address, std::string newMemory)
{
	WriteMemory(address, stringToMemory(newMemory));
}
