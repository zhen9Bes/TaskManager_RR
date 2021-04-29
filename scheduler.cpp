#include "scheduler.h"

Scheduler::Scheduler()
    : _quantum(20)         // Значение кванта около 20-50 мс часто является разумным компромиссом.
{
    _processQueue = {};
    _endedProcesses = {};
}

Scheduler::Scheduler(int quantum, int timeslice)
    : _quantum(quantum),
      _timeslice(timeslice)
{
    _processQueue = {};
    _endedProcesses = {};
}

Scheduler::~Scheduler()
{

}

void Scheduler::schedule()
{
//    //Круговой циклический алгоритм (Round Robin)
//    Process* proc = _processQueue.front();
//    if(proc->getState() != State::BLOCKED){
//        proc->execute(_quantum);
//        if(proc->isComplete()){
//            proc->setState(State::ENDED);
//            _endedProcesses.push_back(proc);
//            _processQueue.pop_front();
//        }else{
//            proc->setState(State::READY);
//            _processQueue.push_back(proc);
//            _processQueue.pop_front();
//        }
//    }else{
//        proc->setState(State::READY);
//        _processQueue.push_back(proc);
//        _processQueue.pop_front();
//    }
}

void Scheduler::clear()
{
    if(!_processQueue.empty()){
        throw "Process queue is not empty.";
    }else{
        _endedProcesses.clear();
    }
}

bool Scheduler::isComplete()
{
    if(_processQueue.empty()){
        return true;
    }else{
        return false;
    }
}

void Scheduler::addProcess(Process* process)
{
    Task task(process, _quantum);
    _processQueue.push_back(task);
}

void Scheduler::addProcess(std::string name, int duration, std::string program)
{
    Process* proc = new Process(name, duration, program);
    Task task(proc, _quantum);

    // При алгоритме циклического планирования каждый новый процесс добавляется в конец очереди
    _processQueue.push_back(task);
}

const std::deque<Task>* Scheduler::getProcessQueue()
{
    return &_processQueue;
}

const std::vector<Task> *Scheduler::getEndedProcess()
{
    return &_endedProcesses;
}

//int Scheduler::getProgressProcess(Process* process)
//{
//    int result = process->getProgress();
//    return result;
//}

void Scheduler::setQuantum(int quantum)
{
    _quantum = quantum;
}

std::vector<Process*> Scheduler::getCompletedProcesses()
{
//    return _endedProcesses;
}

int Scheduler::getQuantum()
{
    return _quantum;
}
