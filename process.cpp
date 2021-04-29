#include "process.h"
#include "windows.h"

int Process::_counterId = 0;    // Инициализируем статическую переменную только в cpp файле,
                                // чтобы избежать множественного объявиления

Process::Process(std::string name, int duration, std::string program)
    : _name(name),
      _duration(duration),
      _program(program)
{
    if(_name == ""){
        throw "Invalid process name";
    }else if(_duration <= 0){
        throw "Invalid duration time";
    }
    _id = ++Process::_counterId;
    _state = BLOCKED;       //Сделаем по умолчанию BLOCKED, но можно сделать и READY
    _isComplete = false;
    _progress = 0;
    _stepProgress = 100.0 / (_duration / 20.0);
}

Process::~Process()
{
    // Так как наш процесс не харнит ничего в heap,
    // то достаточно пустого деструктора
}


void Process::execute(int timeExecute)
{
    this->_state = State::RUNNING;
    if(_duration <= timeExecute){
        _duration = 0;
        _progress = 100;
        _isComplete = true;
        // код выполнения
        Sleep(timeExecute);
    }else{
        _duration -= timeExecute;
        _progress += _stepProgress;
        if(_progress >= 100){
            _progress = 95;
        }
        // код выполнения
        Sleep(timeExecute);
    }
}

int Process::getProgress()
{
    return _progress;
}

bool Process::isComplete()
{
    return _isComplete;
}

void Process::setComplete()
{
    _isComplete = true;
}

int Process::getDuration()
{
    return _duration;
}

void Process::setDuration(int duration)
{
    _duration = duration;
}

void Process::setState(State state)
{
    _state = state;
}

std::string Process::getStateStr()
{
    switch (_state) {
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
    }
}

State Process::getState()
{
    return _state;
}

int Process::getId()
{
    return _id;
}

int Process::getCountAllProcesses()
{
    return Process::_counterId;
}

std::string Process::getName()
{
    return _name;
}

void Process::setName(std::string name)
{
    _name = name;
}
