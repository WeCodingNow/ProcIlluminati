/* TODO(x): Сделать кнопку "Посмотреть область памяти"
 * TODO: Показывать лог для каждого надсмотрщика отдельно
 * TODO: Окрашивать измененные области памяти
 * TODO: (ADV) Менять частоту работы каждого надсмотрщика отдельно
 * TODO: Возможность сохранять логи
 * TODO: Возможность обнаруживать новые строки в памяти
 * TODO: Запилить русский язык для сообщений в логах
 * TODO: (ADV) Показывать логи в таблице:
 *                                  |n-ый надсмотрщик/имя надсмотрщика|
 *                                  |облатсь памяти|обнаружена строка |
 *                                  |..............|..................|
 *                                  |область памяти|изменение строки  |
 *                                  |..............|..................|
 * TODO: (ADV) Возможность изменять определенные области памяти, так, чтобы изменения полностью "влазили" в старую строку.
 * TODO: Возможность представлять информацию в разных видах(десятичное число, ASCII-строка, UNICODE-строка, т.д.)
 *
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"

//начало области памяти с названиями операций в Stend.exe 0x0046B420;
//адрес "AC->T" в Stend.exe 0x0046B42C;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    watchertimer = new QTimer(this);
    watchertimer->setTimerType(Qt::PreciseTimer);
    QObject::connect(watchertimer, SIGNAL(timeout()), this, SLOT(RefreshWatchers()));
    QObject::connect(this->ui->FrequencySlider, SIGNAL(sliderMoved(int)), this, SLOT(SetRefreshInterval(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::transitionState(State st)
{
    switch (st)
    {
        case State::processAttached:
        ui->AttachToProcessAction->setDisabled(true);
        ui->AddWatcherAction->setEnabled(true);
        ui->CheckMemoryAction->setEnabled(true);
        ui->CheckBoxActiveWatcher->setEnabled(true);
            break;
        case State::processNotAttached:
        ui->AttachToProcessAction->setEnabled(true);
        ui->AddWatcherAction->setDisabled(true);
        ui->CheckMemoryAction->setDisabled(true);
        ui->MemRefreshButton->setDisabled(true);
        ui->CheckBoxActiveWatcher->setDisabled(true);
            break;
    }
}

void MainWindow::AttachToProcess()
{
    auto temp_hook = std::make_shared<ProcessHook>((this->processName).toLocal8Bit().constData());

    if(!temp_hook->IsHooked())
    {
        QMessageBox::critical(this, "Ошибка", "Невозможно подключиться к процессу.\nЭтот процесс больше не запущен.");
        return;
    }
    hook = temp_hook;

    this->setWindowTitle(this->windowTitle() + " : " + this->processName);
    this->transitionState(State::processAttached);

//    debug code
//    auto memExample = hook->ReadMemory(adress, 64);
//    std::string allMemory{""};
//    allMemory = MemoryToString(memExample);
//    auto QallMemory = QString::fromLocal8Bit(allMemory.c_str());
//    QallMemory = formatMemoryString(QallMemory, 8);
//    ui->MemoryText->setPlainText(QallMemory);
}

const int max_interval = 2000;
const int min_interval = 200;

void MainWindow::SetRefreshInterval(int)
{
    int desiredFrequency = ui->FrequencySlider->value();
    double period = (1 / desiredFrequency);
    this->refresh_interval = (period * max_interval);

    if(this->watchertimer->isActive())
    {
        this->watchertimer->stop();
        this->watchertimer->setInterval(this->refresh_interval);
        this->watchertimer->start();
    }
    else
    {
        this->watchertimer->setInterval(this->refresh_interval);
    }
}

void MainWindow::RefreshWatchers()
{
    if (!this->hook->IsHooked())
    {
        ui->LoggingListWidget->addItem("Процесс " + this->processName + " закрыт.");
        this->watchertimer->stop();
        transitionState(State::processNotAttached);
        return;
    }

    QFile file = QFile(filename);
    if (filename!= "" && !file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QMessageBox::information(this, tr("Ошибка при открытии файла"),
                                 file.errorString());
        return;
    }

    QTextStream out(&file);

//    for(int i = 0; i < ui->LoggingListWidget->count(); ++i)
//    {
//        QListWidgetItem* item = ui->LoggingListWidget->item(i);

//        out << (item->text() + "\r\n").toUtf8();
//    }

    file.close();

    for (int i = 0; i < ui->WatchersListView->count(); i++)
    {
        auto element = static_cast<MemoryWatcherItem*>(ui->WatchersListView->item(i));
        element->tick();
        std::shared_ptr<QStringList> logP = element->getLogString();

        for(int j = 0; j < logP->count(); j++)
        {
            auto* bass = new QListWidgetItem(logP->at(j));

            out << (bass->text() + "\r\n").toUtf8();

            ui->LoggingListWidget->addItem(bass);//logP->at(j));

        }
    }

    file.close();
    ui->SaveLogAction->setEnabled(true);
}

void MainWindow::SaveToFile()
{
    filename = QFileDialog::getSaveFileName(
        this,
        "Сохранить лог",
        "",
        "Текстовый файл (*.txt);;Все файлы (*)"
    );

    QFile file = QFile(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Ошибка при открытии файла"),
                                 file.errorString());
        return;
    }

    QTextStream out(&file);


    for(int i = 0; i < ui->LoggingListWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->LoggingListWidget->item(i);

        out << (item->text() + "\r\n").toUtf8();
    }

    file.close();
}

void MainWindow::saveNewInfo(QString data) {
//    if(filename != "") {
//        QFile file = QFile(filename);

//        if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
//            QMessageBox::information(this, tr("Ошибка при открытии файла"),
//                                     file.errorString());
//            return;
//        }

//        QDataStream out(&file);
//        out << (data + "\r\n").toUtf8();
//        file.close();
//    }

}

//void MainWindow::RefreshLog()
//{
//    if(!ui->CheckboxGeneralLog->isChecked()) {
//        auto items = ui->LoggingListWidget->findItems("*", Qt::MatchFlag::MatchWildcard);
//        for (auto &i : items) {
//            ui->LoggingListWidget->setItemHidden(i, true);
//        }
//    }
//    else {
//        auto items = ui->LoggingListWidget->findItems("*", Qt::MatchFlag::MatchWildcard);
//        ui->WatchersListView->currentItem();
//        for (auto &i : items) {
//            ui->LoggingListWidget->setItemHidden(i, false);
//        }
//    }
//}

void MainWindow::on_AttachToProcessAction_triggered()
{
    auto procDialog = std::unique_ptr<procdialog>(new procdialog());
    procDialog->setModal(true);
    auto result = procDialog->exec();

    if(result == QDialog::DialogCode::Accepted)
    {
        this->processName = procDialog->chosenProcName;
        AttachToProcess();
    }
}

void MainWindow::on_AddWatcherAction_triggered()
{
    watcherDialog = new AddWatcherDialog(this);
    watcherDialog->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(watcherDialog, SIGNAL(finishedAddingWatchers(bool)), this->ui->AddWatcherAction, SLOT(setEnabled(bool)));
    QObject::connect(watcherDialog, SIGNAL(newWatcher(QString, unsigned long, unsigned int)),
                     this, SLOT(AddWatcher(QString, unsigned long, unsigned int)));
    watcherDialog->show();

    ui->AddWatcherAction->setDisabled(true);
}

void MainWindow::AddWatcher(QString name, unsigned long address, unsigned int length)
{
    auto newItem = new MemoryWatcherItem(name, this->hook, address, length);
    ui->MemRefreshButton->setEnabled(true);
    ui->WatchersListView->addItem(newItem);
}

void MainWindow::on_MemRefreshButton_clicked()
{
    this->RefreshWatchers();
    if(!this->watchertimer->isActive()){
        this->watchertimer->start();
        this->ui->MemRefreshButton->setText("Стоп");
    }
    else {
        this->watchertimer->stop();
        this->ui->MemRefreshButton->setText("Старт");
    }
}

void MainWindow::on_action_Qt_triggered()
{
    QMessageBox::aboutQt(this, "Про Qt");
}

void MainWindow::on_WatchersListView_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current != nullptr)
    {
        ui->MemoryText->setPlainText(static_cast<MemoryWatcherItem*>(current)->getFormattedMemory());
        ui->CheckBoxActiveWatcher->setEnabled(true);
        ui->CheckBoxActiveWatcher->setChecked(static_cast<MemoryWatcherItem*>(current)->getActive());

    }
    else
    {
        ui->MemoryText->setPlainText("");
        ui->CheckBoxActiveWatcher->setDisabled(true);
    }
}

void MainWindow::on_FrequencySlider_sliderPressed()
{

}

void MainWindow::on_CheckBoxActiveWatcher_stateChanged(int arg1)
{
    static_cast<MemoryWatcherItem*>(ui->WatchersListView->currentItem())->setActive(arg1);
}

void MainWindow::on_CheckMemoryAction_triggered()
{
    auto checkMemoryDialog = new checkmemorydialog();
    checkMemoryDialog->hookToProc(hook.get());
    checkMemoryDialog->show();
}

void MainWindow::on_ButtonDeleteWatcher_clicked()
{
    auto drop = ui->WatchersListView->takeItem(ui->WatchersListView->row(ui->WatchersListView->currentItem()));
    delete drop;
}

void MainWindow::on_SaveLogAction_triggered()
{
    SaveToFile();
}
