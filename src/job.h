#ifndef JOB_H
#define JOB_H

#include "process.h"

class Job
{
public:
    Job(Process* process, float quantumTime);
    Job(const Job& process);
    Job& operator=(const Job& process);
    ~Job();

    Job() = delete;

    Process* getProcess();
    int getId();
    std::string getName();
    float getProgress();
    void updateProgress();
private:
    Process* _process;
    int _id;
    std::string _name;
    float _progress;
    float _stepPogress;
};

#endif // JOB_H
