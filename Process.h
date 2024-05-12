//Kazi Sameen Anwar

#ifndef PROCESS_H
#define PROCESS_H

class Process 
{    
    public:
        enum class State {New, Ready, Running, Waiting, Terminated, Zombie};

        Process(int PID);

        int getPID() const;

        State getState() const;
        
        void setState(State newState);

    private:
        int PID;
        State state = State::New;
};

#endif