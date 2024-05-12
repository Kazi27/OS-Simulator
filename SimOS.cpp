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
    std::cout << "the PID of this process is " << newPID << std::endl; //uncomment this to check if PID is being updated correctly
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
    std::cout << "parent process PID is " << parentProcess.getPID() << std::endl;
    int childPID = nextPID++;
    Process childProcess(childPID);
    readyQueue.push_back(childProcess); //push child back into the ready queue
    std::cout << "child process PID is " << childPID << std::endl;
}

Process SimOS::GetFrontProcess() //testing 
{
  return readyQueue.front();
}

void SimOS::SimExit()
{
    if (readyQueue.size() == 0) //nothing running u return
    {
        return;
    }

    Process exitProcess = readyQueue.front();
    readyQueue.pop_front(); //pop the front of the queue
    
    //gotta release memory, use paging -- not done
}

void SimOS::SimWait()
{
    //memory stuff, waiting lmao
}

void SimOS::TimerInterrupt()
{
    if (readyQueue.size() == 0)
    {
        return; //no processes in the queue
    }

    readyQueue.push_back(readyQueue.front()); //running process goes to the back
    readyQueue.pop_front();

    Process nextProcess = readyQueue.front(); //schedule next process
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
    for (const Process& process : readyQueue) {
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