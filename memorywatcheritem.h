#ifndef MEMORYWATCHERITEM_H
#define MEMORYWATCHERITEM_H

#include <QListWidgetItem>
#include <QString>
#include <QTextStream>

#include "MyClasses/MemoryWatcher.h"

class MemoryWatcherItem : public QListWidgetItem
{
private:
    QString myName;
    std::unique_ptr<MemoryWatcher> myWatcher;
    bool isActive;

public:
    MemoryWatcherItem() = delete;
    MemoryWatcherItem(const QString &name, std::shared_ptr<ProcessHook> myHook, unsigned long address, unsigned int length);
    virtual QVariant data(int role) const;

    QString getMyName() const;
    QString getFormattedMemory(unsigned int = 8) const;
    std::shared_ptr<QStringList> getLogString() const;
    void tick();
    void setActive(bool);
    bool getActive();

};

#endif // MEMORYWATCHERITEM_H
