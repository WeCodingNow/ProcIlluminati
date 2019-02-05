#include "procdialog.h"
#include "ui_procdialog.h"

procdialog::procdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::procdialog)
{
    ui->setupUi(this);
    repopulateList();
    redrawList();
}

procdialog::~procdialog()
{
    delete ui;
}

void procdialog::repopulateList()
{
    hiddenListWidget.clear();

    HANDLE snapshotHandle;
    PROCESSENTRY32 entry;

    entry.dwSize = sizeof(PROCESSENTRY32);
    snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshotHandle, &entry) == TRUE)
    {
        while (Process32Next(snapshotHandle, &entry) == TRUE)
        {
            QString procEntry = QString::fromWCharArray(entry.szExeFile);
            hiddenListWidget.addItem(procEntry);
        }
    }
}

void procdialog::redrawList()
{
    ui->processList->clear();

    auto matched_items = hiddenListWidget.findItems(nameFilter, Qt::MatchContains);
    for (auto& item : matched_items)
    {
        QString itemText = item->text();
        ui->processList->addItem(itemText);
    }
}

void procdialog::on_lineEdit_textEdited(const QString &arg1)
{
    nameFilter = arg1;
    redrawList();
}

void procdialog::on_buttonBox_accepted()
{
    chosenProcName = ui->processList->currentItem()->text();
}

void procdialog::on_refreshButton_clicked()
{
    repopulateList();
    redrawList();
}

void procdialog::on_processList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->buttonBox->buttons()[0]->setDisabled(current == nullptr);
}
