#include "job.h"

Job::Job(Process* process, float quantumTime)
{
    if(process == nullptr){
        throw "Job::Job() - Invalid process pointer";
    }
    if(quantumTime <= 0){
        throw "Job::Job() - Invalid quantum time";
    }
    _process = process;
    _id = _process->getId();
    _name = _process->getName();
    _progress = 0;
    int duration = _process->getDuration();
    _stepPogress = 100.0 / (duration / quantumTime);
}

Job::~Job()
{

}

Process* Job::getProcess()
{
    return _process;
}

int Job::getId()
{
    return _id;
}

std::string Job::getName()
{
    return _name;
}

float Job::getProgress()
{
    return _progress;
}

void Job::updateProgress()
{
    _progress += _stepPogress;
    if(_progress >= 90){
        _progress = 100;
    }
}
