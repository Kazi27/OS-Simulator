//Kazi Sameen Anwar

#include "SimOS.h"
#include <iostream>

int main()
{
    SimOS sim = SimOS(1, 2, 3);

    //testing newprocess() --> go to simOS new process and uncomment

    //testing fork
    //create some initial processes
    int numProcesses = 5;
    for (int i = 0; i < numProcesses; ++i) {
        sim.NewProcess();
        std::cout << "Number of processes in ready queue: " << sim.GetReadyQueueSize() << std::endl;
    }

    //Simulate a process "running" by removing it from the front of the queue
    if (sim.GetReadyQueueSize() != 0) 
    {
        Process runningProcess = sim.GetFrontProcess();
        sim.GetReadyQueue().pop_front(); 
        std::cout << "Simulating process " << runningProcess.getPID() << " running..." << std::endl;
    }

    // Now call SimFork to test forking from the "running" process
    sim.SimFork();

    // Print the ready queue size after the fork
    std::cout << "Number of processes in ready queue after fork: " << sim.GetReadyQueueSize() << std::endl;

    return 0;
}