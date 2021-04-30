#ifndef CPU_H
#define CPU_H

#include "scheduler.h"

class CPU
{
public:
    CPU();

    int exetuce(Task& task, int quantum);

private:
    Task* _nextTask;
};

#endif // CPU_H
