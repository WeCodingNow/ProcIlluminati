#ifndef ADDWATCHERDIALOG_H
#define ADDWATCHERDIALOG_H

#include <QDialog>
#include <QValidator>
#include <QIntValidator>
#include <QDebug>

namespace Ui {
class AddWatcherDialog;
}

class AddWatcherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddWatcherDialog(QWidget *parent = nullptr);
    ~AddWatcherDialog();

signals:
    void newWatcher(QString, unsigned long, unsigned int);
    void finishedAddingWatchers(bool);

private slots:
    void on_AddWatcherButton_clicked();

private:
    Ui::AddWatcherDialog *ui;

};

#endif // ADDWATCHERDIALOG_H
