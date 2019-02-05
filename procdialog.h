#ifndef PROCDIALOG_H
#define PROCDIALOG_H

#include <windows.h>
#include <tlhelp32.h>

#include <QDialog>
#include <QListWidget>
#include <QString>
#include <QMessageBox>

namespace Ui {
class procdialog;
}

class procdialog : public QDialog
{
    Q_OBJECT

public:
    explicit procdialog(QWidget *parent = nullptr);
    ~procdialog();

    QString chosenProcName;

private slots:
    void on_lineEdit_textEdited(const QString &arg1);
    void on_buttonBox_accepted();
    void on_refreshButton_clicked();

    void on_processList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::procdialog *ui;

    void repopulateList();
    void redrawList();
    QListWidget hiddenListWidget;
    QString nameFilter;

signals:
    void ProcessNameChosen(QString procName);
};

#endif // PROCDIALOG_H
