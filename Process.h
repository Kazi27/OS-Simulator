//Kazi Sameen Anwar

#ifndef PROCESS_H
#define PROCESS_H

class Process 
{
    public:
        Process(int PID);

        int getPID() const;

    private:
        int PID;
};

#endif