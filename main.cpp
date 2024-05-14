// //Kazi Sameen Anwar

// // #include "SimOS.h"
// // #include "Process.h"
// // #include <iostream>

// // int main()
// // {
// //     SimOS sim = SimOS(1, 2, 3);

// //     // //testing newprocess() --> go to simOS new process and uncomment it to check

// //     //testing fork
// //     //create some initial processes
// //     int numProcesses = 5;
// //     for (int i = 0; i < numProcesses; ++i) {
// //         sim.NewProcess();
// //         std::cout << "number of processes in ready queue: " << sim.GetReadyQueueSize() << std::endl;
// //     }

// //     //testing if a process is "running" by removing it from the front of the queue
// //     // if (sim.GetReadyQueueSize() != 0) 
// //     // {
// //     //     Process runningProcess = sim.GetFrontProcess();
// //     //     sim.GetReadyQueue().pop_front(); 
// //     //     std::cout << "process running: " << runningProcess.getPID() << std::endl;
// //     // }

// //     //test forking from "running" process
// //     sim.SimFork();

// //     //check ready queue size after the fork, shoudl be incresed by 1
// //     std::cout << "Number of processes in ready queue after fork: " << sim.GetReadyQueueSize() << std::endl;

// //     //testing interrupt - getting weird numbers
// //     // for (int i = 0; i < 5; i++) 
// //     // {
// //     //     std::cout << "Timer Interrupt " << i + 1 << std::endl;
// //     //     sim.TimerInterrupt();
// //     //     std::cout << "Ready Queue after Interrupt " << i + 1 << ": ";
// //     //     std::deque<int> queuePIDs = sim.GetReadyQueue();
// //     //     for (int pid : queuePIDs) 
// //     //     {
// //     //         std::cout << pid << " ";
// //     //     }
// //     //     std::cout << std::endl;
// //     // }

// //   return 0;
// // }

#include"SimOS.h"
#include<deque>

int main()
{
	SimOS sim{ 3, 1000, 10 };
	bool allTestsClean{ true };

	if (sim.GetCPU() != NO_PROCESS)
	{
		allTestsClean = false;
		std::cout << "Test on the line 9 fails!" << std::endl;
	}

	sim.NewProcess();
	if (sim.GetCPU() != 1)
	{
		allTestsClean = false;
		std::cout << "Test on the line 16 fails!" << std::endl;
	}

	sim.DiskReadRequest(0, "file1.txt");
	if (sim.GetCPU() != NO_PROCESS)
	{
		allTestsClean = false;
		std::cout << "Test on the line 23 fails!" << std::endl;
	}

	FileReadRequest request{ sim.GetDisk(0) };
	if (request.PID != 1 || request.fileName != "file1.txt")
	{
		allTestsClean = false;
		std::cout << "Test on the line 30 fails!" << std::endl;
	}

	std::deque<FileReadRequest> ioQueue0{ sim.GetDiskQueue(0) };
	if (ioQueue0.size() != 0)
	{
		allTestsClean = false;
		std::cout << "Test on the line 37 fails!" << std::endl;
	}

	sim.DiskJobCompleted(0);
	request = sim.GetDisk(0);
	if (request.PID != NO_PROCESS || request.fileName != "")
	{
		allTestsClean = false;
		std::cout << "Test on the line 45 fails!" << std::endl;
	}

	if (sim.GetCPU() != 1)
	{
		allTestsClean = false;
		std::cout << "Test on the line 51 fails!" << std::endl;
	}

	std::deque<int> readyQueue{ sim.GetReadyQueue() };
	if (readyQueue.size() != 0)
	{
		allTestsClean = false;
		std::cout << "Test on the line 58 fails!" << std::endl;
	}

	sim.SimFork();
	readyQueue = sim.GetReadyQueue();
	if (readyQueue[0] != 2)
	{
		allTestsClean = false;
		std::cout << "Test on the line 66 fails!" << std::endl;
	}

	sim.TimerInterrupt();
	readyQueue = sim.GetReadyQueue();
	if (sim.GetCPU() != 2 || readyQueue[0] != 1)
	{
		allTestsClean = false;
		std::cout << "Test on the line 74 fails!" << std::endl;
	}

	sim.SimExit();
	readyQueue = sim.GetReadyQueue();
	if (sim.GetCPU() != 1 || readyQueue.size() != 0)
	{
		allTestsClean = false;
		std::cout << "Test on the line 82 fails!" << std::endl;
	}

	// sim.AccessMemoryAddress(140);
	// MemoryUsage ram{ sim.GetMemory() };
	// if (ram[0].pageNumber != 14 || ram[0].PID != 1)
	// {
	// 	allTestsClean = false;
	// 	std::cout << "Test on the line 90 fails!" << std::endl;
	// }

	// sim.SimWait();
	// if (sim.GetCPU() != 1)
	// {
	// 	allTestsClean = false;
	// 	std::cout << "Test on the line 97 fails!" << std::endl;
	// }

	// sim.SimExit();
	// ram = sim.GetMemory();
	// if (sim.GetCPU() != NO_PROCESS || ram.size() != 0)
	// {
	// 	allTestsClean = false;
	// 	std::cout << "Test on the line 105 fails!" << std::endl;
	// }

	if (allTestsClean)
		std::cout << "These preliminary tests are passed" << std::endl;

	return 0;
}