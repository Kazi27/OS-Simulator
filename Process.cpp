#include "Process.h" 

Process::Process(int PID)
{
    this->PID = PID;
}

int Process::getPID() const
{
    return PID;
}
