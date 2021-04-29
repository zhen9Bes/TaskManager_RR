#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
//#include "job.h"
#include "task.h"

#include <vector>
//#include <queue>
#include <deque>

class Scheduler
{
public:
    Scheduler();
    Scheduler(int quantum, int timeslice);
    ~Scheduler();

    void schedule();

    void clear();
    bool isComplete();

    void addProcess(Process* process);
    void addProcess(std::string name, int duration, std::string program);
    int getProgressProcess(Process* process);
    const std::deque<Task>* getProcessQueue();
    const std::vector<Task>* getEndedProcess();

    int getQuantum();
    void setQuantum(int quantum);

    std::vector<Process*> getCompletedProcesses();

private:
    int _quantum;
    int _timeslice;

    std::deque<Task> _processQueue;
    std::vector<Task> _endedProcesses;
};

#endif // SCHEDULER_H
