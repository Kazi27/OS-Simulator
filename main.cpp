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

// #include"SimOS.h"
// #include<deque>

// int main()
// {
// 	SimOS sim{ 3, 1000, 10 };
// 	bool allTestsClean{ true };

// 	if (sim.GetCPU() != NO_PROCESS)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 9 fails!" << std::endl;
// 	}

// 	sim.NewProcess();
// 	if (sim.GetCPU() != 1)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 16 fails!" << std::endl;
// 	}

// 	sim.DiskReadRequest(0, "file1.txt");
// 	if (sim.GetCPU() != NO_PROCESS)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 23 fails!" << std::endl;
// 	}

// 	FileReadRequest request{ sim.GetDisk(0) };
// 	if (request.PID != 1 || request.fileName != "file1.txt")
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 30 fails!" << std::endl;
// 	}

// 	std::deque<FileReadRequest> ioQueue0{ sim.GetDiskQueue(0) };
// 	if (ioQueue0.size() != 0)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 37 fails!" << std::endl;
// 	}

// 	sim.DiskJobCompleted(0);
// 	request = sim.GetDisk(0);
// 	if (request.PID != NO_PROCESS || request.fileName != "")
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 45 fails!" << std::endl;
// 	}

// 	if (sim.GetCPU() != 1)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 51 fails!" << std::endl;
// 	}

// 	std::deque<int> readyQueue{ sim.GetReadyQueue() };
// 	if (readyQueue.size() != 0)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 58 fails!" << std::endl;
// 	}

// 	sim.SimFork();
// 	readyQueue = sim.GetReadyQueue();
// 	if (readyQueue[0] != 2)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 66 fails!" << std::endl;
// 	}

// 	sim.TimerInterrupt();
// 	readyQueue = sim.GetReadyQueue();
// 	if (sim.GetCPU() != 2 || readyQueue[0] != 1)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 74 fails!" << std::endl;
// 	}

// 	sim.SimExit();
// 	readyQueue = sim.GetReadyQueue();
// 	if (sim.GetCPU() != 1 || readyQueue.size() != 0)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 82 fails!" << std::endl;
// 	}

// 	sim.AccessMemoryAddress(140);
// 	MemoryUsage ram{ sim.GetMemory() };
// 	if (ram[0].pageNumber != 14 || ram[0].PID != 1)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 90 fails!" << std::endl;
// 	}

// 	sim.SimWait();
// 	if (sim.GetCPU() != 1)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 97 fails!" << std::endl;
// 	}

// 	sim.SimExit();
// 	ram = sim.GetMemory();
// 	if (sim.GetCPU() != NO_PROCESS || ram.size() != 0)
// 	{
// 		allTestsClean = false;
// 		std::cout << "Test on the line 105 fails!" << std::endl;
// 	}

// 	if (allTestsClean)
// 		std::cout << "These preliminary tests are passed" << std::endl;

// 	return 0;
// }

// #include "SimOS.h"
// #include <deque>
// #include <iostream>

// //Used to clear the terminal
// #ifndef CLEARTERMINAL_HPP
// #define CLEARTERMINAL_HPP
//     #include <iostream>
//     //Running Windows?
//     #ifdef _WIN32
//         #include <windows.h>
//     //Running Linux?
//     #else
//         #include <cstdlib>
//     #endif
//     void clearTerminal() {
//         #ifdef _WIN32
//             system("cls");
//         #else
//             system("clear");
//         #endif
//     }
// #endif

// int main()
// {
//     int numHardDisks{0};
//     unsigned long long ramSize{0};
//     unsigned int pageSize{0};
//     short int numProcesses{0};
//     bool inputFlag{false};

//     clearTerminal();

//     std::cout << "Enter the amount of hard disks the system should have (you might want to make this a number above 1): ";
//     std::cin >> numHardDisks;
//     std::cout << std::endl;

//     std::cout << "Enter the page size (this should be something simple, like a multiple of 10): ";
//     std::cin >> pageSize;
//     std::cout << std::endl;

//     std::cout << "Enter the amount of RAM the system should have (this should be a multiple of the page size, otherwise the test won't work): ";
//     std::cin >> ramSize;
//     std::cout << std::endl;

//     std::cout << "Enter the amount of processes the system should be tested with: ";
//     std::cin >> numProcesses;
//     std::cout << std::endl;

//     std::cout << "Enter 1 to start (entering something else might kill the program unexpectedly): ";
//     std::cin >> inputFlag;
//     std::cout << std::endl;

//     if(!inputFlag || ramSize%pageSize != 0)  //Kill the program if the user didn't listen >:(
//     {
//         return 0;
//     }

//     SimOS sim{numHardDisks, ramSize, pageSize};

// //--------Testing NewProcess() with 10 processes
// //--------Testing TimerInterrupt() by cycling through all the newly created processes
// //--------Testing GetCPU() by checking to see if the correct PID is being returned
// //--------Testing GetReadyQueue() by checking to see if the order of processes is correct
//     clearTerminal();
//     std::cout << "Checking NewProcess(), TimerInterrupt(), and GetCPU()" << std::endl;
//     std::cout << "You should see processes numbered 1 -> " << numProcesses << " appear in order\n" << std::endl;
    
//     std::cout << "Expected order of processes: \t| ";
//     for(short int i = 1; i <= numProcesses; i++)
//     {
//         sim.NewProcess();
//         std::cout << i << " | ";
//     }
//     std::cout << std::endl;

//     std::cout << "Actual order of processes: \t| ";
//     for(short int i = 0; i < numProcesses; i++)
//     {
//         std::cout << sim.GetCPU() <<  " | ";
//         sim.TimerInterrupt();
//     }
//     std::cout << std::endl << std::endl;
    
//     std::cout << "Checking current and ready processes: " << std::endl;
//     std::cout << "\tCurrent Process: | " << sim.GetCPU() << " | " << std::endl;
//     std::deque<int> readyQueue = sim.GetReadyQueue();
//     std::cout << "\tReady Processes: | ";
//     for(auto i : readyQueue)
//         std::cout << i << " | ";
//     std::cout << std::endl << std::endl;

//     std::cout << "The created and checked processes should be in the same order. If there is a mismatch, check NewProcess(), TimerInterrupt(), and GetCPU()" << std::endl;

//     std::cout << std::endl;

// //--------Testing DiskReadRequest()
// //--------Testing GetDisk()
// //--------Testing GetDiskQueue()
// //--------Testing DiskJobCompleted()
//     std::cout << "To proceed with testing, enter 1 (entering something else could unexpectedly terminate the program): ";
//     std::cin >> inputFlag;
//     std::cout << std::endl;

//     if(!inputFlag)
//         return 0;
//     clearTerminal();

//     std::cout << "Checking DiskReadRequest(), GetDisk(), GetDiskQueue(), DiskJobCompleted()" << std::endl;
//     std::cout << "The processes will be the same processes used in the previous round of testing" << std::endl;
//     std::cout << std::endl;
//     std::cout << "Beginning the flood of DiskReadRequests. Every process will make a DiskReadRequest" << std::endl;
//     std::cout << "The requested files will follow the form: 'Process-/PID/_FileNumber-/SomeNumber/" << std::endl;
//     std::cout << "All disks are going to be used" << std::endl << std::endl;

//     std::string tempString;
//     for(short int i = 1; i <= numProcesses; i++)
//     {
//         tempString = "Process-" + std::to_string(sim.GetCPU()) + "_FileNumber-" + std::to_string(i);
//         sim.DiskReadRequest((i-1)%numHardDisks, tempString);
//     }

//     std::cout << "All processes have made a FileReadRequest. There should be no process using the CPU now" << std::endl;
//     std::cout << "Current Process: " << sim.GetCPU() << std::endl;

//     std::cout << std::endl;
//     for(int i = 0; i < numHardDisks; i++)
//     {
//         std::cout << "Checking the state of Hard Disk: " << i << std::endl;
//         std::cout << "\t\t| FileName" << std::endl;
//         std::cout << "\t----------------------------------------" << std::endl;
//         std::cout << "\tCurrent | " << sim.GetDisk(i).fileName << std::endl;
//         std::cout << "\t----------------------------------------" << std::endl;
//         for(auto x : sim.GetDiskQueue(i))
//             std::cout << "\tWaiting | " << x.fileName << std::endl;
//         std::cout << std::endl;
//     }

//     for(short int i = 1; i <= numProcesses; i++)
//     {
//         sim.DiskJobCompleted((i-1)%numHardDisks);
//     }
//     std::cout << "Completing all disk jobs" << std::endl;

//     std::cout << "Checking order of processes: " << std::endl;
//     std::cout << "\tCurrent Process: | " << sim.GetCPU() << " | " << std::endl;
//     std::deque<int> readyQueue2 = sim.GetReadyQueue();
//     std::cout << "\tReady Processes: | ";
//     for(auto i : readyQueue2)
//         std::cout << i << " | ";
//     std::cout << std::endl << std::endl;

//     std::cout << "End Test" << std::endl;
//     return 0;
// }

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

	sim.AccessMemoryAddress(140);
	MemoryUsage ram{ sim.GetMemory() };
	if (ram[0].pageNumber != 14 || ram[0].PID != 1)
	{
		allTestsClean = false;
		std::cout << "Test on the line 90 fails!" << std::endl;
	}

	sim.SimWait();
	if (sim.GetCPU() != 1)
	{
		allTestsClean = false;
		std::cout << "Test on the line 97 fails!" << std::endl;
	}

	sim.SimExit();
	ram = sim.GetMemory();
	if (sim.GetCPU() != NO_PROCESS || ram.size() != 0)
	{
		allTestsClean = false;
		std::cout << "Test on the line 105 fails!" << std::endl;
	}

	if (allTestsClean)
		std::cout << "These preliminary tests are passed" << std::endl;

	return 0;
}