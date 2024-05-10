//Kazi Sameen Anwar

#include "SimOS.h"

SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize)
{
    this->numberOfDisks = numberOfDisks;
    this->amountOfRAM = amountOfRAM;
    this->pageSize = pageSize; //for these the amount depends on the parameter

    int disks = 0;
    int frame = 0;
    int pageEnum = 0; //always starts from 0

    // std::cout << numberOfDisks << amountOfRAM << pageSize << disks << frame << pageEnum; //testing
}

void SimOS::NewProcess()
{
    int newPID = nextPID++;
    //std::cout << "the PID of this process is " << newPID << std::endl; //uncomment this to check if PID is being updated correctly
    //need to create a process object so u need a process class so seperate process header file needed

    Process newProcess(newPID); //creating new process

    readyQueue.push_back(newProcess);
}

int SimOS::GetReadyQueueSize()
{
    return readyQueue.size();
}

void SimOS::SimFork() //the currently running process forks a child so 
{
    //first get the currently running process
    if (readyQueue.size() == 0) //nothing running u return
    {
        return;
    }

    Process parentProcess = readyQueue.front(); //so now that we have the running process, we need to creat a child process and supply PID
    int childPID = nextPID++;
    Process childProcess(childPID);
    readyQueue.push_back(childProcess); //push child back into the ready queue
}

Process SimOS::GetFrontProcess() //testing 
{
  return readyQueue.front();
}

void SimOS::SimExit()
{
    
}

void SimOS::SimWait()
{
    
}

void SimOS::TimerInterrupt()
{
    
}

void SimOS::DiskReadRequest(int diskNumber, std::string fileName)
{
    
}

void SimOS::DiskJobCompleted(int diskNumber)
{
    
}

void SimOS::AccessMemoryAddress(unsigned long long address)
{
    
}

int SimOS::GetCPU()
{
    
}

std::deque<int> SimOS::GetReadyQueue()
{
    std::deque<int> queuePIDs;
    for (Process& process : readyQueue) 
    {
        queuePIDs.push_back(process.getPID());
    }
    return queuePIDs;
}

MemoryUsage SimOS::GetMemory()
{
    
}

FileReadRequest SimOS::GetDisk(int diskNumber)
{
    
}

std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber)
{
    
}