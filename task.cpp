#include "task.h"

Task::Task(Process *process, float quantum)
{
    _process = process;
    _quantum = quantum;
    _progress = 0;
    int duration = process->getDuration();
    _stepProgress = 100.0 / (duration / quantum);
}

Task::~Task()
{
    delete _process;
}

Process *Task::getProcess()
{
    return _process;
}

int Task::getProgress()
{
    return _progress;
}

std::string Task::getState()
{
    State state = _process->getState();
    switch (state) {
    case RUNNING:
        return "Running";
        break;
    case READY:
        return "Ready";
        break;
    case BLOCKED:
        return "Blocked";
        break;
    case ENDED:
        return "Ended";
        break;
    default:
        return "Error";
    }
}
