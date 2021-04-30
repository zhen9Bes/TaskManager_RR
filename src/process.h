#ifndef PROCESS_H
#define PROCESS_H

#include <string>

/*
 * Перечислим возможные состояния процесса.
 * Это самая примитивная модель, поэтому не будем усложнять
 * И оставим только 3 состояния. [Tanenbaum Oper.Sys.]
 * 1. Активный (в данный момент исполняется процессором)
 * 2. Готов к работе (процесс временно приостановлен, чтобы уступить другому процессу)
 * 3. Заблокированный (процесс не может быть запущен пока, не произойдет некое внешнее событие) *
 */
enum State{
    RUNNING,
    READY,
    BLOCKED,
    ENDED       // Добавлено только для отладки. Чтобы отслеживать выполенные процессы.
};

/*
 * Процесс — это активность некоторого рода. У него есть программа, входные и выходные данные, а также состояние.
 * Один процессор может переключаться между различными процессами, опираясь на некий алгоритм планирования
 * для определения момента переключения от одного процесса к другому.
*/
class Process
{
public:
    Process(std::string name, int duration, std::string program = "");
    ~Process();

    Process() = delete;
    Process(const Process& process) = delete;
    Process& operator=(const Process& process) = delete;

    void execute(int timeExecute);

    int getProgress();

    int getId();
    int getCountAllProcesses();

    std::string getName();
    void setName(std::string name);

    void setComplete();
    bool isComplete();

    int getDuration();
    void setDuration(int duration);

    void setState(State state);
    std::string getStateStr();
    State getState();

private:
    static int _counterId;
    int _id;
    std::string _name;
    int _arrivalTime;
    int _duration;
    int _progress;
    double _stepProgress;
    std::string _program;
    State _state;
    bool _isComplete;
};

#endif // PROCESS_H
