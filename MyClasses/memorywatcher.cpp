#include "MemoryWatcher.h"

MemoryWatcher::MemoryWatcher(std::shared_ptr<ProcessHook> someProcessHook,
                             unsigned long memoryPlace, unsigned int memLen) :
    myHook(someProcessHook), memoryPoint(memoryPlace), memoryLength(memLen)
{
    if(!myHook->IsHooked())
    {
     loggingStream << "Не удалось создать надсмотрщика: нет подключения к процессу!";
     return;
    }

    //первый раз смотрим содержимое памяти
    loggingStream
            << "Watcher created: "
            << "0x" << std::setfill('0') << std::setw(8) << std::hex << memoryPlace
            << " - "
            << "0x" << std::setfill('0') << std::setw(8) << std::hex << memoryPlace + memLen
            << std::endl;
    refresh();
}

Memory_T MemoryWatcher::getMemory()
{
    return scannedMemory;
}

std::stringstream &MemoryWatcher::getLoggingStream()
{
    return loggingStream;
}

bool MemoryWatcher::refresh()
{
    std::vector<memstring> oldMemstrs;
    std::vector<memstring> memstrs;

    bool isFirstTimeScanning = (myDetector.GetDetectedStrings().size() == 0);
    bool memChanged = false;
    scannedMemory = myHook->ReadMemory(memoryPoint, memoryLength);

    oldMemstrs = myDetector.GetDetectedStrings();
    myDetector.DetectStrings(scannedMemory);
    memstrs = myDetector.GetDetectedStrings();

    if(isFirstTimeScanning)
    {
        loggingStream
                << "Found strings at "
                << "0x" << std::setfill('0') << std::setw(8) << std::hex << memoryPoint
                << " - "
                << "0x" << std::setfill('0') << std::setw(8) << std::hex << memoryPoint + memoryLength
                << ": " << std::endl;
        for(auto &i: memstrs)
        {
            loggingStream
                    << "0x" << std::setfill('0') << std::setw(8) << std::hex << memoryPoint + i.first
                    << " : " << i.second << std::endl;
        }
       // loggingStream << std::endl;
    }
    else
    {
        unsigned long lastSameIndex = 0;
        for (auto& i: oldMemstrs)
        {
            for(unsigned long j = lastSameIndex; j < memstrs.size(); j++)
            {
                if(i.first == memstrs[j].first)
                {
                    lastSameIndex = j;
                    if(i.second != memstrs[j].second)
                    {
                        memChanged = true;
                        loggingStream
                                << "0x" << std::setfill('0') << std::setw(8) << std::hex << memoryPoint + i.first
                                << ": " << i.second
                                << " --> " << memstrs[j].second
                                << std::endl;
                        changedStrings.push_back(i);
                    }
                    break;
                }
                if(j == memstrs.size())
                {
                    memChanged = true;
                    loggingStream
                            << "0x" << std::setfill('0') << std::setw(8) << std::hex << memoryPoint + i.first
                            << ": " << i.second
                            << " --> " << " "
                            << std::endl;
                    changedStrings.push_back(i);
                }
            }
        }
    }

    return memChanged;
}

