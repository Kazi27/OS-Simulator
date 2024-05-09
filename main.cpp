//Kazi Sameen Anwar

#include "SimOS.h"
#include <iostream>

int main()
{
    SimOS sim = SimOS(1, 2, 3);

    // Call NewProcess multiple times
    int numProcesses = 5;
    for (int i = 0; i < numProcesses; ++i) 
    {
        sim.NewProcess();
        std::cout << "number of processes in ready queue: " << sim.GetReadyQueueSize() << std::endl;
    }
}