#include "memorywatcheritem.h"

MemoryWatcherItem::MemoryWatcherItem(
        const QString& name,
        std::shared_ptr<ProcessHook> myHook,
        unsigned long address,
        unsigned int length)
    :
        myName(name),
        myWatcher(std::make_unique<MemoryWatcher>(MemoryWatcher(myHook, address, length))){}

QVariant MemoryWatcherItem::data(int role) const
{
    if(role == Qt::DisplayRole)
    {
        return myName;
    }
    return QListWidgetItem::data(role);
}

QString formatMemoryString(const QString& memory_str, const unsigned int rowLength = 0)
{
    QString returnString;

    for(int i = 0; i < memory_str.length(); i++)
    {
        if(i % 2 == 0 && i != 0)
        {
            returnString.push_back(' ');
        }
        if((rowLength != 0) && (static_cast<unsigned int>(i) % (rowLength * 2) == 0) && (i != 0))
        {
            returnString.push_back('\n');
        }
        returnString.push_back(memory_str[i]);

    }

    return returnString;
}


QString MemoryWatcherItem::getMyName() const
{
    return myName;
}

QString MemoryWatcherItem::getFormattedMemory(unsigned int bytesInARow) const
{
        std::string allMemory{""};
        allMemory = MemoryToString(myWatcher->getMemory());
        auto QallMemory = QString::fromLocal8Bit(allMemory.c_str());
        return formatMemoryString(QallMemory, bytesInARow);
}

std::shared_ptr<QStringList> MemoryWatcherItem::getLogString() const
{
    std::shared_ptr<QStringList> returnList = std::make_shared<QStringList>();
    std::string oneLine;
    while(std::getline(myWatcher->getLoggingStream(), oneLine))
    {
        QString conversionString = QString::fromLocal8Bit(oneLine.c_str());
        *returnList << conversionString;
    }
    std::stringstream().swap(myWatcher->getLoggingStream());
    return returnList;
}

void MemoryWatcherItem::tick()
{
    if(isActive)
        myWatcher->refresh();
}

void MemoryWatcherItem::setActive(bool arg)
{
    isActive = arg;
}

bool MemoryWatcherItem::getActive()
{
    return isActive;
}
