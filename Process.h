//Kazi Sameen Anwar

#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <vector>

class Process 
{    
    public:
        enum class State {New, Ready, Running, Waiting, Terminated, Zombie};

        Process(); //default constructor to make default values 

        Process(int PID);

        //getters and setters

        int getPID() const;

        State getState() const;

        Process getParent() const;

        Process getChild() const;

        std::vector<Process*>& getChildren();

        void setPID(int PID);
        
        void setState(State NewState);

        void setParent(Process Parent);

        void setChild(Process Child);

        bool hasParent() const; //simexit

        bool hasChild() const; //simwait

    private:
        int pid;
        State state = State::New;
        Process* parent = nullptr;
        Process* child = nullptr;
        std::vector<Process*> childrenProcesses; //for forking, all child will be in each process vector
}; 

#endif