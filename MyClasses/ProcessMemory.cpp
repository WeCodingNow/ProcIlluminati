#include "ProcessMemory.h"

Byte CharToHexedChar(char c)
{
	if (c >= 'A' && c <= 'F')
	{
		return c - 55;
	}
	else if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
}

Memory_T AsciiStringToMemory(std::string input, unsigned int nullTerminatorsAmount)
{
	Memory_T returnMem;

	for (auto i : input)
	{
		returnMem.push_back(i);
	}
	for (int i = 0; i < nullTerminatorsAmount; i++)
	{
		returnMem.push_back(0);
	}

    return returnMem;
}


std::string MemoryToString(const Memory_T& input)
{
    std::stringstream bufStream;

    for(auto &i: input)
    {
        if ((unsigned int)i < 16)
            bufStream << '0';
        bufStream << std::hex << (unsigned int)i;
    }

    return bufStream.str();
}
