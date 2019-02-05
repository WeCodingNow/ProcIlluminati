#ifndef CHECKMEMORYDIALOG_H
#define CHECKMEMORYDIALOG_H

#include <QDialog>
#include "MyClasses/ProcessHook.h"

namespace Ui {
class checkmemorydialog;
}

class checkmemorydialog : public QDialog
{
Q_OBJECT

public:
explicit checkmemorydialog(QWidget *parent = nullptr);
~checkmemorydialog();

void hookToProc(ProcessHook*);
void refreshMemText(int);

private slots:
    void on_verticalScrollBar_valueChanged(int value);

private:
    Ui::checkmemorydialog *ui;
    ProcessHook* myHook;
};

#endif // CHECKMEMORYDIALOG_H
