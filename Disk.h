#ifndef DISK_H
#define DISK_H

#include <deque>
#include <string>
#include "Process.h"

struct FileReadRequest 
{
  int PID;
  std::string fileName;
};

class Disk 
{
    public:
        Disk(); //to initialize current request

        void diskReadReq(FileReadRequest ReadReq, Process Process); //to allow process to read the disk

        Process currProcessReading(); //to find out which process is currently reading

        //getters

        std::deque<FileReadRequest> getDiskReadQueue();

        FileReadRequest getCurrReq();

        int getDiskReadQueueSize();

    private:
        FileReadRequest currReadReq;
        std::deque<FileReadRequest> diskReadQueue;
        Process currReadProcess;
};

#endif // DISK_H
