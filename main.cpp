//Kazi Sameen Anwar

#include "SimOS.h"
#include "Process.h"
#include <iostream>

int main()
{
    SimOS sim = SimOS(1, 2, 3);

    // //testing newprocess() --> go to simOS new process and uncomment it to check

    //testing fork
    //create some initial processes
    int numProcesses = 5;
    for (int i = 1; i < numProcesses; ++i) {
        sim.NewProcess();
        std::cout << "number of processes in ready queue: " << sim.GetReadyQueueSize() << std::endl;
    }

    // //testing if a process is "running" by removing it from the front of the queue
    // if (sim.GetReadyQueueSize() != 0) 
    // {
    //     Process runningProcess = sim.GetFrontProcess();
    //     sim.GetReadyQueue().pop_front(); 
    //     std::cout << "process running: " << runningProcess.getPID() << std::endl;
    // }

    // //test forking from "running" process
    // sim.SimFork();

    // //check ready queue size after the fork, shoudl be incresed by 1
    // std::cout << "Number of processes in ready queue after fork: " << sim.GetReadyQueueSize() << std::endl;

    //testing interrupt - getting weird numbers
    for (int i = 0; i < 5; i++) 
    {
        std::cout << "Timer Interrupt " << i + 1 << std::endl;
        sim.TimerInterrupt();
        std::cout << "Ready Queue after Interrupt " << i + 1 << ": ";
        std::deque<int> queuePIDs = sim.GetReadyQueue();
        for (int pid : queuePIDs) 
        {
            std::cout << pid << " ";
        }
        std::cout << std::endl;
    }

  return 0;
}