#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "job.h"

#include <vector>
//#include <queue>
#include <deque>

class Scheduler
{
public:
    Scheduler();
    ~Scheduler();

    void schedule();

    void clear();
    bool isComplete();

    void addProcess(Process* process);
    void addProcess(std::string name, int duration, std::string program);
    int getProgressProcess(Process* process);

    int getQuantum();
    void setQuantum(int quantum);

    std::vector<Process*> getCompletedProcesses();

private:
    int _quantum;
    bool _isComplete;
    std::deque<Process*> _processQueue;
    std::vector<Process*> _endedProcesses;
};

#endif // SCHEDULER_H
