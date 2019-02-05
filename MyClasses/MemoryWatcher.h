#pragma once
#include <memory>
#include <sstream>
#include <iomanip>
#include "ProcessHook.h"
#include "StringDetector.h"

class MemoryWatcher
{
private:
    std::shared_ptr<ProcessHook> myHook;
    StringDetector myDetector;
    unsigned long memoryPoint;
    unsigned int memoryLength;

    Memory_T scannedMemory;
    std::vector<memstring> changedStrings;
    std::stringstream loggingStream;


public:
    MemoryWatcher(std::shared_ptr<ProcessHook>, unsigned long, unsigned int);
    Memory_T getMemory();
    std::stringstream &getLoggingStream();
    bool refresh();
};
