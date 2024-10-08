//Kazi Sameen Anwar

#ifndef SIMOS_H
#define SIMOS_H //header guards
#include <vector>
#include <string>
#include <deque>
#include <iostream>

#include "Process.h"
#include "Disk.h"

//moved struct FileReadRequest to dish.h to avoid redefinition error

struct MemoryItem
{
    unsigned long long pageNumber;
    unsigned long long frameNumber;
    int PID; // PID of the process using this frame of memory
};
 
using MemoryUsage = std::vector<MemoryItem>;
 
constexpr int NO_PROCESS{ 0 };

class SimOS 
{
    public:
        //constructor - specify number of hard disks, amount of memory and page size. Disks, frame, and page enumerations start from 0
        SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize); 

        //create new process inside simulated system, takes place in ready queue or starts using in the cpu
        //every process has a PID and increments by 1, cant reuse PID of terminated process
        void NewProcess();

        //currnt running process forks a child, child goes to end of the ready queue
        void SimFork();

        //process currently using cpu terminates, release memory used by process.
        //if parent waiting, process terminates immediately and parent becomes runnable aka goes to ready queue
        //if parent hasnt called wait yet process turns into a zombie
        //cascading termination for no orphans, if a process terminates all descendants terminate with it
        void SimExit();

        //process waits for any of its child to terminate
        //once wait is over process goes to end of ready queue or cpu
        //if zombie child exists process keeps using the cpu
        //if more than one zombie child exists process uses any of them to immediately resume parent while other zombies keep waiting for next wait from parent
        void SimWait();

        //interrupt arrives from timer signaling time slice of currently running process is over
        void TimerInterrupt();

        //current process requests to read the file from the disk with a given number
        //process issuing disk reading requests stops using CPU even if ready-queue is empty
        void DiskReadRequest(int diskNumber, std::string fileName);

        //disk with specified number reports that a single job is completed and served process returns to the ready-queue.
        void DiskJobCompleted(int diskNumber);

        //current running process accesses the logical memory address
        //system makes sure the page is loaded in the RAM and if it is already in the RAM, its “recently used” information is updated
        void AccessMemoryAddress(unsigned long long address);

        //returns the PID of the process currently using the CPU and CPU is idle it returns NO_PROCESS
        int GetCPU();

        //returns std::deque with PIDs of processes in ready-queue where element in front corresponds start of the ready-queue
        std::deque<int> GetReadyQueue();

        //returns MemoryUsage vector describing all currently used frames of RAM. 
        //terminated “zombie” processes don’t use memory, so they don’t contribute to memory usage
        //memoryItems appear in the MemoryUsage vector in the order they appear in memory (from low addresses to high)
        MemoryUsage GetMemory();

        //returns an object with PID of process served by specified disk and name of the file read for that process
        //if disk is idle, returns default FileReadRequest object (with PID 0 and empty string in fileName) 
        FileReadRequest GetDisk(int diskNumber);

        //returns I/O-queue of the specified disk starting from the “next to be served” process
        std::deque<FileReadRequest> GetDiskQueue(int diskNumber);

        //helper function to get ready queue size
        int GetReadyQueueSize();
        
        //helper function to get front of ready queue
        Process GetFrontProcess();

        //helper function to erase process along with its children from ram
        void cascadeTerminate(Process Process);

    
    private:  
        std::deque<Process> readyQueue; //hold all processes in the readyqueue
        int numberOfDisks;
        
        unsigned long long amountOfRAM; //included this for constructor param
        unsigned int pageSize = 0; //included this for constructor param
        
        unsigned long long totalFrames;
        MemoryUsage memory;
        unsigned long long nextFrameNumber = 0;

        std::vector<Disk> diskQueue; //disk vector

        Process currRunningProcess; //default constructor invoked
        
        int pidCounter = 1; //to keep track of PID

        //need memory vector?
        //unsigned int numPagesAllocated; // each process will have this amount of pages? if u uncomment this add 
        //numPagesAllocated = amountOfRAM/pageSize; 
        // the total number of pages allocated for each process (bc we assume each process can use entire RAM) to the cpp file
};

#endif