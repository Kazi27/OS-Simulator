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

    std::cout << numberOfDisks << amountOfRAM << pageSize << disks << frame << pageEnum; //testing
}

void SimOS::NewProcess()
{
    int newPID = nextPID++;
    std::cout << "the PID of this process is " << newPID << std::endl; //testing
    //need to create a process object so u need a process class so seperate process header file needed

    Process newProcess(newPID); //creating new process

    readyQueue.push_back(newProcess);
}

int SimOS::GetReadyQueueSize()
{
    return readyQueue.size();
}

void SimOS::SimFork()
{
    
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