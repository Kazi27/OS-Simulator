//Kazi Sameen Anwar

#include "SimOS.h"

SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize)
{
    this->numberOfDisks = numberOfDisks;
    this->amountOfRAM = amountOfRAM;
    this->pageSize = pageSize; //for these the amount depends on the parameter

    //main specifies total amount of disks so push those into the disk vector
    for (int i = 0; i < numberOfDisks; i++) 
    {
        diskQueue.push_back(Disk()); //still in progress, dont know what disk() does
    }

    // std::cout << numberOfDisks << amountOfRAM << pageSize << disks << frame << pageEnum; //testing
}

void SimOS::NewProcess()
{
    // int newPID = nextPID++; old
    // std::cout << "the PID of this process is " << newPID << std::endl; //uncomment this to check if PID is being updated correctly
    //need to create a process object so u need a process class so seperate process header file needed

    Process newProcess(pidCounter); //creating new process

    //if the cpu is idle and a new process is created, it can start using the cpu which means state = running and update the current running process variable
    if (GetCPU() == 0)
    {
        currRunningProcess.setPID(newProcess.getPID()); 
        //now the new process is running the cpu so change state
        newProcess.setState(Process::State::Running);
    }
    else
    {
        //if ur here, cpu is being used and process is ready and willing to use the cpu so push it to ready queue
        newProcess.setState(Process::State::Ready);
        readyQueue.push_back(newProcess);
    }

    //regardless, we need a new PID so increment count
    pidCounter++;
}

void SimOS::SimFork()
{
    //first check if there is a currently running process because forking needs a currently running rpocess
    if (GetCPU() == 0)
    {
        throw std::logic_error("No process currently running so can't fork");
    }

    Process parent = currRunningProcess; //inherits state, PID so no need to set those again
    parent.setState(Process::State::Running); //ask lubna if i should set it to running or not or what state
    std::cout << "parent PID is " << parent.getPID() << std::endl;

    Process child = Process();
    child.setState(Process::State::Ready);
    child.setPID(parent.getPID() + 1);
    child.setParent(parent);

    parent.setChild(child);
    std::cout << "child PID is " << child.getPID() << std::endl;

    readyQueue.push_back(child);
}

//     Process parentProcess = readyQueue.front(); //so now that we have the running process, we need to creat a child process and supply PID
//     std::cout << "parent process PID is " << parentProcess.getPID() << std::endl;
//     int childPID = nextPID++;
//     Process childProcess(childPID);
//     readyQueue.push_back(childProcess); //push child back into the ready queue
//     std::cout << "child process PID is " << childPID << std::endl;

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

    // Process nextProcess = readyQueue.front(); //schedule next process
}

//current process requests to read the file from the disk with a given number
//process issuing disk reading requests stops using CPU even if ready-queue is empty
void SimOS::DiskReadRequest(int diskNumber, std::string fileName)
{

}

void SimOS::DiskJobCompleted(int diskNumber)
{
    
}

void SimOS::AccessMemoryAddress(unsigned long long address)
{
    
}

//returns the PID process currently using the CPU which is in the currRunningProcess vairable. If CPU is idle returns NO_PROCESS
int SimOS::GetCPU()
{
    if (currRunningProcess.getPID() == 0) //nobody is using the cpu rn
    {
        return NO_PROCESS;
    }

    return currRunningProcess.getPID(); //this PID is using the cpu rn
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

// helper functions

int SimOS::GetReadyQueueSize()
{
    return readyQueue.size();
}

Process SimOS::GetFrontProcess() //testing 
{
  return readyQueue.front();
}