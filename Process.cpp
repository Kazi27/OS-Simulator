#include "Process.h" 

Process::Process()
{
    pid = 0; //gets rid of junk val in PID by default
    setState(State::New);
    parent = nullptr;
    child = nullptr;
}

Process::Process(int PID)
{
    this->pid = PID;
    setState(State::New);
    this->parent = nullptr;
    child = nullptr;
}

//getters and setters

int Process::getPID() const
{
    return pid;
}

Process::State Process::getState() const 
{ 
    return state; 
}

Process Process::getParent() const 
{ 
    return *parent; 
}

Process Process::getChild() const 
{ 
    return *child; 
}

void Process::setPID(int PID) 
{ 
    pid = PID; 
}
    
void Process::setState(State newState) 
{ 
    state = newState; 
}

void Process::setParent(Process Parent) 
{ 
    this->parent = &Parent; 
}

void Process::setChild(Process Child) 
{ 
    this->child = &Child; 
}

bool Process::hasParent() const //simexit
{ 
    if (parent == nullptr)
    {
        return false;
    } 

    if (parent != nullptr)
    {
        return true;
    } 
}

bool Process::hasChild() const //simwait
{ 
    if (child == nullptr)
    {
        return false;
    } 

    if (child != nullptr)
    {
        return true;
    } 
}