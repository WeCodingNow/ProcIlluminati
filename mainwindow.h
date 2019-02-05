#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//QT
#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <QMessageBox>
#include <QDialog>
#include <QPlainTextEdit>
#include <QFileDialog>

//Доп. формы
#include "procdialog.h"
#include "addwatcherdialog.h"
#include "checkmemorydialog.h"

//Стандартная библиотека
#include <memory>

//Рабочие классы
#include "MyClasses/MemoryWatcher.h"
#include "memorywatcheritem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void saveNewInfo(QString data);
private slots:
    void on_AttachToProcessAction_triggered();
    void on_AddWatcherAction_triggered();
    void on_MemRefreshButton_clicked();
    void on_action_Qt_triggered();
    void on_WatchersListView_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void AddWatcher(QString, unsigned long, unsigned int);
    void SetRefreshInterval(int);
    void RefreshWatchers();

//    void RefreshLog();

    void on_FrequencySlider_sliderPressed();

    void on_CheckBoxActiveWatcher_stateChanged(int arg1);

    void on_CheckMemoryAction_triggered();

    void on_ButtonDeleteWatcher_clicked();

    void on_SaveLogAction_triggered();

public slots:
    void SaveToFile();

private:
    Ui::MainWindow *ui;

    enum class State {processNotAttached, processAttached};
    State myState;
    QString processName;
    std::shared_ptr<ProcessHook> hook;
//    QVector<std::unique_ptr<MemoryWatcher>> watchers;
    AddWatcherDialog* watcherDialog;
    QTimer* watchertimer;
    int refresh_interval;
    QString filename;


    void AttachToProcess();
    void transitionState(State st);

};

#endif // MAINWINDOW_H
