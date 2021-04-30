#ifndef TASK_H
#define TASK_H

#include "process.h"

class Task
{
public:
    Task() = delete;
    Task(Process* process, float quantum);
    ~Task();
    Process* getProcess();
    int getProgress();
    std::string getState();
private:
    Process* _process;
    int _quantum;
    int _progress;
    int _stepProgress;
};

#endif // TASK_H
