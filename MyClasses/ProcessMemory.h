#pragma once
#include <string>
#include <vector>
#include <sstream>

typedef unsigned char Byte;
typedef std::vector<Byte> Memory_T;

Byte CharToHexedChar(char);
Memory_T AsciiStringToMemory(std::string, unsigned int = 0);
std::string MemoryToString(const Memory_T&);
