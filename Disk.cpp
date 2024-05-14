#include "Disk.h"

Disk::Disk() //to initialize current request
{
    currReadReq = {0, ""}; //pid of 0 and empty string initially
    currReadProcess = Process(); //initialize with default process
}

void Disk::diskReadReq(FileReadRequest ReadReq, Process Process) //to allow process to read the disk
{
    //first push the requests into disk read queue
    diskReadQueue.push_back(ReadReq);
        
    //also store associated process
    processQueue.push_back(Process);

    if (currReadReq.PID == 0) //if nobodys in the queue
    {
        currReadReq = diskReadQueue.front();
        currReadProcess = processQueue.front();
        diskReadQueue.pop_front();
        processQueue.pop_front();

        // currReadProcess = Process; //this process is currently reading
        // currReadReq = ReadReq; //currently serving this read request
        // diskReadQueue.pop_front(); //take it out from the queue as it's currently reading anyways
    }
}

Process Disk::currProcessReading() //to find out which process is currently reading
{
    // if (getDiskReadQueueSize() != 0) //if queue is not empty
    // {
    //     currReadReq = diskReadQueue.front(); //theres something in the queue waiting to read, whoever is first on the queue is current
    //     diskReadQueue.pop_front(); //take it out from the queue as it's currently reading anyways
    // }

    return currReadProcess; 

    // if (!diskReadQueue.empty()) 
    // {
    //     currReadReq = diskReadQueue.front(); // Store the actual request
    //     diskReadQueue.pop_front(); // Remove it from the queue
    // }
    // return currReadProcess;
}

//getters

std::deque<FileReadRequest> Disk::getDiskReadQueue()
{
    return diskReadQueue;
}

FileReadRequest Disk::getCurrReq()
{
    return currReadReq;
}

int Disk::getDiskReadQueueSize()
{
    return diskReadQueue.size();
}

void Disk::setNextRequest()
{
    if (!diskReadQueue.empty()) {
        currReadReq = diskReadQueue.front();
        currReadProcess = processQueue.front();
        diskReadQueue.pop_front();
        processQueue.pop_front();
    } else {
        currReadReq = {0, ""}; // Reset to default if no request is pending
        currReadProcess = Process(); // Reset to default process
    }
}