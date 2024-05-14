//Kazi Sameen Anwar

#ifndef DISK_H
#define DISK_H

#include <deque>
#include <string>
#include "Process.h"

struct FileReadRequest 
{
  int PID{0};
  std::string fileName{""};
};

class Disk 
{
    public:
        Disk(); //to initialize current request

        void diskReadReq(FileReadRequest ReadReq, Process Process); //to allow process to read the disk

        Process currProcessReading(); //to find out which process is currently reading

        //getters and setters

        std::deque<FileReadRequest> getDiskReadQueue();

        FileReadRequest getCurrReq();

        int getDiskReadQueueSize();

        void setNextRequest();

    private:
        FileReadRequest currReadReq;
        std::deque<FileReadRequest> diskReadQueue;
        std::deque<Process> processQueue; //track the processes associated with the requests
        Process currReadProcess;
};

#endif // DISK_H
