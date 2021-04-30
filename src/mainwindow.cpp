#include "mainwindow.h"
#include "./ui_mainwindow.h"

class ProgressBarDelegate : public QItemDelegate {
public:
    inline ProgressBarDelegate(QMainWindow* mainWindow = 0 ) : QItemDelegate(mainWindow) {}

    void paint( QPainter* painter,
                const QStyleOptionViewItem& option,
                const QModelIndex& index
                ) const override
    {
        if (index.column() != 3) {
            QItemDelegate::paint(painter, option, index);
            return;
        }
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.state = QStyle::State_Enabled;
        progressBarOption.direction = QApplication::layoutDirection();
        progressBarOption.rect = option.rect;
        progressBarOption.fontMetrics = QApplication::fontMetrics();
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
        progressBarOption.textVisible = true;

        int progress = index.data().toInt();
        progressBarOption.progress = progress < 0 ? 0 : progress;
        if(progressBarOption.progress == 0){
            progressBarOption.text = QString("Preparation!");
        }else if(progressBarOption.progress == 100){
            progressBarOption.text = QString("Comlete!");
        }else{
            progressBarOption.text = QString::asprintf("%d%%", progressBarOption.progress);
        }

        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _scheduler = new Scheduler();
    _tasks = _scheduler->getProcessQueue();
    _timer = new QTimer;
    _time = QTime::fromString("00:00.000", "mm:ss.zzz");
    _ui->timerLabel->setText(_time.toString());
    _programTick = 100;     // Устанавливаем тик программы. Для отладки, чтобы видеть порядок исполнения процессов
    _timer->start(_programTick);

    // Progress Table settings
    const QStringList headers = QStringList() << "ID" << "Name" << "Status" << "Progress";
    _progressView = _ui->progressView;
    _progressView->setColumnCount(headers.count());
    _progressView->setHorizontalHeaderLabels(headers);
    _progressView->setItemDelegateForColumn(3, new ProgressBarDelegate(this) );// устанавливаем наш делегат
    _progressView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _progressView->setAlternatingRowColors(true);
    _progressView->verticalHeader()->setVisible(false);
    _progressView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _progressView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _progressView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _progressView->horizontalHeader()->setStretchLastSection(true);

    // UI Edits settings
    _ui->quantumEdit->setText(QString::number(_scheduler->getQuantum()));
    _ui->processIdEdit->setText(QString::number(1));
    _ui->durationTimeEdit->setText(QString::number(20));
    _ui->nameProcessEdit->setText(QString::number(1));

    // Button settings
    _ui->addTaskButton->setDisabled(false);
    _ui->executeButton->setDisabled(false);
    _ui->clearButton->setDisabled(true);

    // Connection settings
    QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(timer_tick()));
}

MainWindow::~MainWindow()
{
    delete _ui;
    delete _scheduler;
    delete _timer;
}

void MainWindow::on_executeButton_toggled(bool checked)
{
    if(checked){
        _ui->executeButton->setText(tr("Stop execution"));
    }else{
        _ui->executeButton->setText(tr("Start execution"));
    }
}

void MainWindow::on_addTaskButton_clicked()
{
    try {
        std::string nameProcess = _ui->nameProcessEdit->text().toStdString();
        int duration = _ui->durationTimeEdit->text().toInt();
//        Process proc(nameProcess, duration, "");
//        _scheduler->addProcess(&proc);
        _scheduler->addProcess(nameProcess, duration, "");

//        Job job;
//        job.process = &proc;
//        job.id = QString::number(proc.getId());
//        job.name = QString(proc.getName().c_str());
//        _jobs << job;

//        addRow(&job);

        _ui->allProcessListWidget->addItem(_ui->nameProcessEdit->text() + "\t" + _ui->durationTimeEdit->text());

        _ui->durationTimeEdit->setText(QString::number(40 + rand() % 200));

        _ui->processIdEdit->setText(QString::number(_ui->processIdEdit->text().toInt() + 1));
        _ui->nameProcessEdit->setText(QString::number(_ui->processIdEdit->text().toInt()));

    }  catch (const std::exception& ex) {
        createMessage(ex.what(), QMessageBox::Icon::Critical);
    }  catch (const char* ex){
        createMessage(ex, QMessageBox::Icon::Warning);
    }
}

void MainWindow::timer_tick()
{
    //Обновляем значение времени на форме
    _time = _time.addMSecs(_programTick);
    _ui->timerLabel->setText(_time.toString());
    updateProgressView();
    if(!_scheduler->isComplete() && _ui->executeButton->isChecked()){
        _scheduler->schedule();

        std::vector<Process*> completedProcesses = _scheduler->getCompletedProcesses();
        _ui->endedProcessListWidget->clear();
        for(auto& proc : completedProcesses){
            _ui->endedProcessListWidget->addItem(QString(proc->getName().c_str()));
        }
        updateProgressView();
    }
}

void MainWindow::on_clearButton_clicked()
{
    try {
        _scheduler->clear();
        _jobs.clear();
        _ui->allProcessListWidget->clear();
        _ui->endedProcessListWidget->clear();
        _ui->timerLabel->setText("0");
        _progressView->setRowCount(0);
    } catch (std::exception& ex) {
        createMessage(ex.what(), QMessageBox::Icon::Critical);
    } catch (const char* ex) {
        createMessage(ex, QMessageBox::Icon::Warning);
    }

    _ui->executeButton->setDisabled(false);
    _ui->addTaskButton->setDisabled(false);
    _ui->clearButton->setDisabled(true);
}

void MainWindow::addRow(Job* job) {
    int currentRow = _progressView->rowCount();
    _progressView->insertRow(currentRow);

    if( QTableWidgetItem* item = new QTableWidgetItem( job->id ) ) {
        item->setFlags( item->flags() ^ Qt::ItemIsEditable );
        _progressView->setItem( currentRow, 0, item );
    }
    if( QTableWidgetItem* item = new QTableWidgetItem( job->name ) ) {
        item->setFlags( item->flags() ^ Qt::ItemIsEditable );
        _progressView->setItem( currentRow, 1, item );
    }
    Process* proc = processForRow(currentRow);
    std::string state = proc->getStateStr();
    if( QTableWidgetItem* item = new QTableWidgetItem( QString(state.c_str()) ) ) {
        item->setFlags( item->flags() ^ Qt::ItemIsEditable );
        _progressView->setItem( currentRow, 2, item );
    }
    if( QTableWidgetItem* item = new QTableWidgetItem( "0" ) ) {
        item->setFlags( item->flags() ^ Qt::ItemIsEditable );
        _progressView->setItem( currentRow, 3, item );
    }
}

void MainWindow::updateProgressView() {
    if(_tasks->size()>0)
        processForRow(0);

    for(int i = 0; i < _progressView->rowCount(); ++i){
        if( QTableWidgetItem* item = _progressView->item( i, 2 ) ) {
            Process* proc = processForRow(i);
            std::string state = proc->getStateStr();
            item->setData( Qt::DisplayRole, QString(state.c_str()) );
        }
        if( QTableWidgetItem* item = _progressView->item( i, 3 ) ) {
            Process* proc = processForRow(i);
//            int currentProgress = proc->getProgress();
//            item->setData( Qt::DisplayRole, currentProgress );
        }
    }
}

Process* MainWindow::processForRow(int row)
{
    Task task = _tasks->at(row);
    return task.getProcess();

//    return _jobs.at(row).process;
}

void MainWindow::createMessage(const char* message, QMessageBox::Icon icon)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    msgBox.setIcon(icon);
    msgBox.setText(message);
    msgBox.exec();
}

