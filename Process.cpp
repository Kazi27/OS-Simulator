//Kazi Sameen Anwar

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

std::vector<Process*>& Process::getChildren()
{
    return childrenProcesses;
}

void Process::setPID(int PID) 
{ 
    pid = PID; 
}
    
void Process::setState(State NewState) 
{ 
    state = NewState; 
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
    // if (parent == nullptr)
    // {
    //     return false;
    // } 

    // if (parent != nullptr)
    // {
    //     return true;
    // } 
    return parent != nullptr;
}

bool Process::hasChild() const //simwait
{ 
    // if (child == nullptr)
    // {
    //     return false;
    // } 

    // if (child != nullptr)
    // {
    //     return true;
    // } 
    return child != nullptr;
}