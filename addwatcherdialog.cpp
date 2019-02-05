#include "addwatcherdialog.h"
#include "ui_addwatcherdialog.h"

AddWatcherDialog::AddWatcherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddWatcherDialog)
{
    ui->setupUi(this);
}

AddWatcherDialog::~AddWatcherDialog()
{
    delete ui;
    emit finishedAddingWatchers(true);
}

void AddWatcherDialog::on_AddWatcherButton_clicked()
{
    emit newWatcher(ui->lineEditName->text(),
                    static_cast<unsigned int>(ui->lineEditAddress->text().toLong(nullptr,16)),
                    static_cast<unsigned int>(ui->lineEditBytesAmount->text().toInt()));
}
