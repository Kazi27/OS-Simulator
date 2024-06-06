# OS-Simulator
## A Comprehensive Operating System Simulation

### This project implements a comprehensive operating system (OS) simulation designed to simulate the internal workings of an OS. It simulates essential functionalities, including:

- CPU Scheduling
  - Round-robin scheduling ensures fair CPU allocation among processes. Each process has a time limit for how long it can use the CPU at once. If a process uses the CPU for a longer time, it goes back to the end of the ready-queue.
- Memory Management
  - Paging with Least Recently Used page replacement efficiently utilizes memory resources. If the memory is full, the least recently used frame is removed from memory.
- Disk Management
  - File read requests and disk job requests are completed via a first-come, first-served model and so is the I/O queue.
- Process Management
  - You can create, fork, exit, or call wait on processes and make child/parent processes. Processes are terminated via cascading termination as well.

 ### Testing it out for yourself:
 - The repository contains `main1.cpp` which allows you to input your own number of disks, processes, RAM and more to understand what the OS is doing. 
 - To run `main1.cpp` please clone the repository and paste the following commands into your terminal:
#### `g++ -std=c++17 simos.cpp disk.cpp process.cpp main1.cpp -o main1`
#### `./main1`
