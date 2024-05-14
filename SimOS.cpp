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
        diskQueue.push_back(Disk());
    }

    totalFrames = amountOfRAM / pageSize; //calculate total amount of frames

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

    Process child = Process();
    child.setState(Process::State::Ready);
    child.setPID(parent.getPID() + 1);
    child.setParent(parent);

    parent.setChild(child);
    //std::cout << "child PID is " << child.getPID() << std::endl;

    readyQueue.push_back(child);
}

//process currently using cpu terminates, release memory used by process.
//if parent waiting, process terminates immediately and parent becomes runnable aka goes to ready queue
//if parent hasnt called wait yet process turns into a zombie
//cascading termination for no orphans, if a process terminates all descendants terminate with it
void SimOS::SimExit()
{
    //first check if there is a currently running process because exiting needs a currently running process
    //if (currRunningProcess.getPID() == 0)
    if (GetCPU() == 0)
    {
        throw std::logic_error("No process currently running so nothing to exit");
    }

    if (currRunningProcess.hasParent() == false) //process does NOT have a parent
    {
        //erase from ram along with all its children
        cascadeTerminate(currRunningProcess);

        //round robin scheduling
        currRunningProcess = Process(); //turn into default process to indicate no process currently running, cpu is idle

        if (GetReadyQueueSize() != 0) 
        {
            // readyQueue.push_back(currRunningProcess); //current process goes to the end of the queue -- -- we don't do this because process is terminated, no need to go back to ready queue
            currRunningProcess = readyQueue.front(); //now the new front of the ready queue is the running process
            currRunningProcess.setState(Process::State::Running); //now this is running the cpu
            readyQueue.pop_front(); //remove the process in the front now because irs running the cpu now
        }
    }

    else if ((currRunningProcess.hasParent() == true) && (currRunningProcess.getParent().getState() == Process::State::Waiting)) //process DOES have a parent and parent process DID called wait
    {
        currRunningProcess.getParent().setState(Process::State::Ready); //the parent process that did call wait, is now ready and willing to use the cpu

        readyQueue.push_back(currRunningProcess.getParent()); //now that its in the ready state, push it back to the ready queue

        //erase from ram along with all its children
        cascadeTerminate(currRunningProcess);
        
        //round robin scheduling
        currRunningProcess = Process(); //turn into default process to indicate no process currently running, cpu is idle

        if (GetReadyQueueSize() != 0) 
        {
            // readyQueue.push_back(currRunningProcess); //current process goes to the end of the queue -- we don't do this because process is terminated, no need to go back to ready queue
            currRunningProcess = readyQueue.front(); //now the new front of the ready queue is the running process
            currRunningProcess.setState(Process::State::Running); //now this is running the cpu
            readyQueue.pop_front(); //remove the process in the front now because irs running the cpu now
        }
    }

    else //process DOES have a parent and parent process did NOT call wait
    {
        currRunningProcess.setState(Process::State::Zombie); //if parent process didnt call wait it turns into a zombie

        //erase from ram along with all its children
        cascadeTerminate(currRunningProcess);
        
        //round robin scheduling
        currRunningProcess = Process(); //turn into default process to indicate no process currently running, cpu is idle

        if (GetReadyQueueSize() != 0) 
        {
            // readyQueue.push_back(currRunningProcess); //current process goes to the end of the queue -- we don't do this because process is terminated, no need to go back to ready queue
            currRunningProcess = readyQueue.front(); //now the new front of the ready queue is the running process
            currRunningProcess.setState(Process::State::Running); //now this is running the cpu
            readyQueue.pop_front(); //remove the process in the front now because irs running the cpu now
        }
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
        return; //just return?
    }

    //note to self - use either of the below else if statements
    //else if ((currRunningProcess.hasChild() == true) && (currRunningProcess.getState() != Process::State::Waiting)) //process DOES have a child and did NOT call wait
    else if (currRunningProcess.getChild().getState() == Process::State::Zombie)  //process DOES have a child and did NOT call wait aka its child is a ZOMBIE
    {
        //if you are here, process has a child and is not waiting so process has a zombie child - in this case, process keeps using the cpu
        currRunningProcess.setState(Process::State::Running);

        //erase from ram along with all its children
        cascadeTerminate(currRunningProcess);
    }

    //this part deals with finding a zombie child 
    bool foundZombie = false;
    Process zombieChild;
    
    for (auto& child : currRunningProcess.getChildren())  //getChildren returns a list or vector of child processes, auto for type, range based loop goes over all children in getchildren vector from process class
    {
        if (currRunningProcess.getChild().getState() == Process::State::Zombie)
        {
            foundZombie = true;
            zombieChild = currRunningProcess.getChild();
            break;
        }
    }

    //if you did find a zombie child
    if (foundZombie == true)
    {
        //change the state of child to zombie and then return so parent process keeps using the cpu
        currRunningProcess.getChild().setState(Process::State::Zombie);
        return;
    }

    //if ur here, no zombie children have been found so process state is waiting
    currRunningProcess.setState(Process::State::Waiting);
    
    //round robin scheduling
    currRunningProcess = Process(); //turn into default process to indicate no process currently running, cpu is idle

    if (GetReadyQueueSize() != 0) 
    {
        // readyQueue.push_back(currRunningProcess); //current process goes to the end of the queue -- we don't do this because process is terminated, no need to go back to ready queue
        currRunningProcess = readyQueue.front(); //now the new front of the ready queue is the running process
        currRunningProcess.setState(Process::State::Running); //now this is running the cpu
        readyQueue.pop_front(); //remove the process in the front now because irs running the cpu now
    }
}

void SimOS::TimerInterrupt()
{
    //first check if there is a currently running process because you cant signal time slice to be over without a process running rn
    if (GetCPU() == 0)
    {
        throw std::logic_error("No process currently running so timer interrupt can't signal time slice to be over");
    }
    
    if (GetReadyQueueSize() != 0) //round robin scheduling
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
    if ((diskNumber > numberOfDisks) || (diskNumber < 0))
    {
        throw std::logic_error("Invalid disk number so can't complete disk job");
    }

    Process completedProcess = diskQueue[diskNumber].currProcessReading(); //curr process read gives the process reading the disk rn

    completedProcess.setState(Process::State::Ready);
    readyQueue.push_back(completedProcess);

    diskQueue[diskNumber].setNextRequest(); //set next request for the disk

    //resume round robin scheduling
    if (!readyQueue.empty()) 
    {
        // readyQueue.push_back(currRunningProcess); //current process goes to the end of the queue -- we don't do this (?)
        currRunningProcess = readyQueue.front(); //now the new front of the ready queue is the running process
        currRunningProcess.setState(Process::State::Running); //now this is running the cpu
        readyQueue.pop_front(); //remove the process in the front now because irs running the cpu now
    }
}

//Currently running process wants to access the specified logical memory address. 
//System makes sure the corresponding page is loaded in the RAM. 
//If the corresponding page is already in the RAM, its “recently used” information is updated.

//if page is not in memory, it loads it and if its full, it replaces the leased recently used page with it
void SimOS::AccessMemoryAddress(unsigned long long address)
{
    if (GetCPU() == NO_PROCESS) 
    {
        throw std::logic_error("No process currently running so cannot access memory");
    }

    unsigned long long pageNumber = address / pageSize; //get page number
    
    int pid = currRunningProcess.getPID(); //get pid

    //we need to check if the page is already in memory
    bool pageFound = false;
    for (auto it = memory.begin(); it != memory.end(); ++it) 
    {
        if (it->pageNumber == pageNumber && it->PID == pid) //if the iterator's pagenumber and pid matches ours then enter
        {
            pageFound = true; //if ur here we found the page so first set it to true
            
            MemoryItem item = *it; //create a copy of the memory item, store it in "item"
            
            memory.erase(it); //remove it from its current position
            
            memory.push_back(item); //push it in the back -- this is basically updating the recently used info as the most recent page is always pushed back, aka move to the end
            
            break;
        }
    }

    if (pageFound == false) //if the page was not found enter this
    {
        //now we have to load it but first
        if (memory.size() == totalFrames) //if memory is full enter this
        {
            memory.erase(memory.begin()); //remove the not recently used frame -- the least recent frame
        }
        memory.push_back({pageNumber, nextFrameNumber++, pid}); //load it
    }
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
    return memory;
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