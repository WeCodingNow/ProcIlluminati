#pragma once
#include "StringDetector.h"


unsigned int StringDetector::DetectStrings(Memory_T input)
{
    if(input.size() == 0)
    {
        return 0;
    }

	detected_strings = std::vector<memstring>();
	unsigned int stringCounter = 0;
	bool foundAString = false;
	std::pair<unsigned long, std::string> tempMemString;
	
	for (int i = 0; i < input.size() - 1; i++)
	{
		switch (foundAString)
		{
		case false:
			if (input[i] == 0x0)
			{
				continue;
			}
			else
			{
				foundAString = true;
				tempMemString.first = i;
				tempMemString.second = "";
				tempMemString.second.push_back(input[i]);
			}
			break;

        case true:
            tempMemString.second.push_back(input[i]);
			if (input[i+1] == 0x0)
			{
				foundAString = false;
				detected_strings.push_back(tempMemString);
				stringCounter++;
			}
			break;
		}
	}

	return stringCounter;
}

std::vector<memstring> StringDetector::GetDetectedStrings()
{
	return detected_strings;
}

unsigned long StringDetector::GetStringOffset(std::string target)
{
	for (auto i : detected_strings)
	{
		if (i.second == target)
		{
			return i.first;
		}
	}
}
