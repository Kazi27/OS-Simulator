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
    parent.setState(Process::State::Running); 
    std::cout << "parent PID is " << parent.getPID() << std::endl;

    Process child = Process();
    child.setState(Process::State::Ready);
    child.setPID(parent.getPID() + 1);
    child.setParent(parent);

    parent.setChild(child);
    std::cout << "child PID is " << child.getPID() << std::endl;

    readyQueue.push_back(child);
}

//process currently using cpu terminates, release memory used by process.
//if parent waiting, process terminates immediately and parent becomes runnable aka goes to ready queue
//if parent hasnt called wait yet process turns into a zombie
//cascading termination for no orphans, if a process terminates all descendants terminate with it
void SimOS::SimExit()
{
    //first check if there is a currently running process because exiting needs a currently running rpocess
    if (GetCPU() == 0)
    {
        throw std::logic_error("No process currently running so nothing to exit");
    }

    if (currRunningProcess.hasParent() == false) //process does NOT have a parent
    {
        //erase from ram along with all its children
        cascadeTerminate(currRunningProcess);
    }

    else if ((currRunningProcess.hasParent() == true) && (currRunningProcess.getParent().getState() == Process::State::Waiting)) //process DOES have a parent and parent process DID called wait
    {
        currRunningProcess.getParent().setState(Process::State::Ready); //the parent process that did call wait, is now ready and willing to use the cpu

        readyQueue.push_back(currRunningProcess.getParent()); //now that its in the ready state, push it back to the ready queue

        //erase from ram along with all its children
        cascadeTerminate(currRunningProcess);
    }

    else //process DOES have a parent and parent process did NOT call wait
    {
        currRunningProcess.setState(Process::State::Zombie); //if parent process didnt call wait it turns into a zombie

        //erase from ram along with all its children
        cascadeTerminate(currRunningProcess);
    }

    currRunningProcess = Process(); //turn into default process to move it to ready qeuue (?)

    if (GetReadyQueueSize() != 0) 
    {
        // readyQueue.push_back(currRunningProcess); //current process goes to the end of the queue -- we don't do this (?)
        currRunningProcess = readyQueue.front(); //now the new front of the ready queue is the running process
        currRunningProcess.setState(Process::State::Running); //now this is running the cpu
        readyQueue.pop_front(); //remove the process in the front now because irs running the cpu now
    }
}

//process waits for any of its child to terminate, once wait is over process goes to end of ready queue or cpu
//if zombie child exists process keeps using the cpu
//if more than one zombie child exists process uses any of them to immediately resume parent while other zombies keep waiting for next wait from parent
void SimOS::SimWait()
{
    //first check if there is a currently running process because waiting needs a currently running process
    if (GetCPU() == 0)
    {
        throw std::logic_error("No process currently running so nothing to wait");
    }
    
    if (currRunningProcess.hasChild() == false) //process does NOT have a child
    {
        //what do you do if process does not have a child? do u go to the end of the ready queue or start using cpu? 
        //or do you let readyQueue.front use the cpu no?

        //do we do cascading termination?
        //erase from ram along with all its children
        cascadeTerminate(currRunningProcess);
    }

    else if ((currRunningProcess.hasChild() == true) && (currRunningProcess.getState() != Process::State::Waiting)) //process DOES have a child and did NOT call wait
    {
        //instead of the else if above should i do this to directly know if the child is a zombie or not
        // else if (currRunningProcess.getChild().getState() == Process::State::Zombie)
        // { }

        //if you are here, process has a child and is not waiting so process has a zombie child
        //in this case, process keeps using the cpu 

        currRunningProcess.setState(Process::State::Running);

        //erase from ram along with all its children
        cascadeTerminate(currRunningProcess);
    }

    //how do u implement this line:
    //if more than one zombie child exists process uses any of them to immediately resume parent while other zombies keep waiting for next wait from parent
}

void SimOS::TimerInterrupt()
{
    //first check if there is a currently running process because you cant signal time slice to be over without a process running rn
    if (GetCPU() == 0)
    {
        throw std::logic_error("No process currently running so timer interrupt can't signal time slice to be over");
    }
    
    if (GetReadyQueueSize() != 0) 
    {
        readyQueue.push_back(currRunningProcess); //current process goes to the end of the queue
        currRunningProcess = readyQueue.front(); //now the new front of the ready queue is the running process
        currRunningProcess.setState(Process::State::Running); //now this is running the cpu
        readyQueue.pop_front(); //remove the process in the front now because irs running the cpu now
    }
}

//current process requests to read the file from the disk with a given number
//process issuing disk reading requests stops using CPU even if ready-queue is empty
void SimOS::DiskReadRequest(int diskNumber, std::string fileName)
{
    //first check if there is a currently running process because disk reading needs a currently running rpocess
    if (GetCPU() == 0)
    {
        throw std::logic_error("No process currently running so cannot request to read the file from the disk");
    }

    if ((diskNumber > numberOfDisks) || (diskNumber < 0))
    {
        throw std::logic_error("Invalid disk number so can't read disk");
    }

    FileReadRequest readRequest = {currRunningProcess.getPID(), fileName};
    diskQueue[diskNumber].diskReadReq(readRequest, currRunningProcess); //

    currRunningProcess.setState(Process::State::Waiting); //is waiting while disk is being read
    currRunningProcess = Process(); //turn into default process to move it to ready qeuue (?)

    //resume round robin scheduling
    if (GetReadyQueueSize() != 0) 
    {
        // readyQueue.push_back(currRunningProcess); //current process goes to the end of the queue -- we don't do this (?)
        currRunningProcess = readyQueue.front(); //now the new front of the ready queue is the running process
        currRunningProcess.setState(Process::State::Running); //now this is running the cpu
        readyQueue.pop_front(); //remove the process in the front now because irs running the cpu now
    }
}

void SimOS::DiskJobCompleted(int diskNumber)
{
    Process completedProcess = diskQueue[diskNumber].currProcessReading(); //curr process read gives the process reading the disk rn

    readyQueue.push_back(completedProcess);
    completedProcess.setState(Process::State::Ready);

    //resume round robin scheduling
    if (GetReadyQueueSize() != 0) 
    {
        // readyQueue.push_back(currRunningProcess); //current process goes to the end of the queue -- we don't do this (?)
        currRunningProcess = readyQueue.front(); //now the new front of the ready queue is the running process
        currRunningProcess.setState(Process::State::Running); //now this is running the cpu
        readyQueue.pop_front(); //remove the process in the front now because irs running the cpu now
    }
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
    std::deque<int> readyQPIDs; //pids are ints

    for (int i = 0; i < GetReadyQueueSize(); ++i) 
    {
        readyQPIDs.push_back(readyQueue[i].getPID()); //extract pids from all the process objects and push it into the ready queue pid of ints
    }

    return readyQPIDs;
}

MemoryUsage SimOS::GetMemory()
{
    
}

FileReadRequest SimOS::GetDisk(int diskNumber)
{
    return diskQueue[diskNumber].getCurrReq();
}

std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber)
{
    return diskQueue[diskNumber].getDiskReadQueue();
}

//helper functions

int SimOS::GetReadyQueueSize()
{
    return readyQueue.size();
}

Process SimOS::GetFrontProcess()
{
  return readyQueue.front();
}

void SimOS::cascadeTerminate(Process Process)
{
    if (Process.hasChild() == true) 
    {
        cascadeTerminate(Process.getChild()); //recursive call till theres no more children left
    }
    
    Process.setState(Process::State::Terminated); //termination
}