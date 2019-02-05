#pragma once
#include <vector>
#include <string>

#include "ProcessMemory.h"


typedef std::pair<unsigned long, std::string> memstring;
//unsigned long - смещение относительно начала блока памяти


class StringDetector
{
private:
	std::vector<memstring> detected_strings;
public:
	StringDetector() = default;
	unsigned int DetectStrings(Memory_T); 
	std::vector<memstring> GetDetectedStrings();
	unsigned long GetStringOffset(std::string);
};