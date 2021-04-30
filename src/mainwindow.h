#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scheduler.h"

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QTableWidget>
#include <QItemDelegate>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addTaskButton_clicked();

    void timer_tick();

    void on_clearButton_clicked();

    void on_executeButton_toggled(bool checked);

private:
    Ui::MainWindow* _ui;

    struct Job {
        Process* process;
        QString id;
        QString name;
    };
    QList<Job> _jobs;

    void addRow(Job* job);
    void updateProgressView();
    Process* processForRow(int row);
    void addJob();
    void createMessage(const char *message, QMessageBox::Icon icon = QMessageBox::Icon::Information);

    Scheduler* _scheduler;    

    QTimer* _timer;
    QTime _time;
    int _programTick;

    QTableWidget* _progressView;
};

#endif // MAINWINDOW_H
