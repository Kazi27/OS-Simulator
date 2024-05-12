#include "Process.h" 

Process::Process(int PID)
{
    this->PID = PID;
}

int Process::getPID() const
{
    return PID;
}

Process::State Process::getState() const 
{ 
    return state; 
}
    
void Process::setState(State newState) 
{ 
    state = newState; 
}