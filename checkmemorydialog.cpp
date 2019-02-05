#include "checkmemorydialog.h"
#include "ui_checkmemorydialog.h"

checkmemorydialog::checkmemorydialog(QWidget *parent) :
    QDialog(parent),
ui(new Ui::checkmemorydialog)
{
    ui->setupUi(this);

}

checkmemorydialog::~checkmemorydialog()
{
    delete ui;
    delete myHook;
}

void checkmemorydialog::hookToProc(ProcessHook *parentHook)
{
    myHook = new ProcessHook(*parentHook);
    refreshMemText(0);
}

QString formatMemoryStringSecond(const QString& memory_str, const unsigned int rowLength = 0)
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

const int lowerMemoryMax = 0x00400000;
const int upperMemoryMax = 0x00500000;

void checkmemorydialog::refreshMemText(int sliderPos)
{
    int lowerMemoryBound= lowerMemoryMax + 384 * sliderPos;

    ui->lowerMemoryLabel->setText("0x" + QString::number(lowerMemoryBound, 16));
    ui->upperMemoryLabel->setText("0x" + QString::number(lowerMemoryBound + 384, 16));

    auto memory = myHook->ReadMemory(lowerMemoryBound, 384);

    QString conversionString = QString::fromLocal8Bit(MemoryToString(memory).c_str());

    ui->memoryText->setPlainText(formatMemoryStringSecond(conversionString, 16));
}

void checkmemorydialog::on_verticalScrollBar_valueChanged(int value)
{
    refreshMemText(value);
}
