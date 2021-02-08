# Operating Systems  Lab.

AUT Operating Systems Lab. course (Fall 2020) resources.

All codes are written in C and tested on Linux kernel version 5.

## Table of Contents

1. [Introduction to Linux](https://github.com/radinshayanfar/os-lab/tree/master/Lab1)
2. [Kernel module programming](https://github.com/radinshayanfar/os-lab/tree/master/Lab2)
3. [Bash scripting](https://github.com/radinshayanfar/os-lab/tree/master/Lab3)
4. IPC (Inter-Process Communication):
	1. [Shared memory](https://github.com/radinshayanfar/os-lab/tree/master/Lab4/part1)
	2. [Simple terminal-based chat application using Socket](#42-simple-terminal-based-chat-application-using-socket)
5. [Threads and processes: Distributing computation on multiple processes (and probably multiple cores)](https://github.com/radinshayanfar/os-lab/tree/master/Lab5)
6. Process Synchronization (using Pthread mutex locks for synchronization):
	1. [The First Readers-Writers Problem (using fork() and shared memory)](#61-the-first-readers-writers-problem-using-fork-and-shared-memory)
	2. [The Dining-Philosophers Problem (using Pthread threads)](#62-the-dining-philosophers-problem-using-pthread-threads)
7. [Deadlock: Bankers algorithm](#7-deadlock-bankers-algorithm)

### 4.2. [Simple terminal-based chat application using Socket](https://github.com/radinshayanfar/os-lab/tree/master/Lab4/part2)

Supports up to `MAXGROUP` groups. Each client should join a group with `/join` command before sending any messages in the group.

To run the program, compile codes with `make all` and then execute the server code and arbitrary number of clients.

```bash
./server.out <port>
```

```bash
./client.out <server-address> <port> <client-name>
```

Supported commands (`<group-id>`s are numbers between [0, `MAXGROUP`) interval):

```
/join <group-id>
/send <group-id> <message>
/leave <group-id>
/quit <group-id>
```

### 6.1. [The First Readers-Writers Problem (using fork() and shared memory)](https://github.com/radinshayanfar/os-lab/tree/master/Lab6)

To run the program, execute `make run_reader_writer`. It'll be create 1 instance of writer and `READERS` number of reader(s). The writer increments `count` variable by one on each iteration up to `COUNT_MAX` times.

### 6.2. [The Dining-Philosophers Problem (using Pthread threads)](https://github.com/radinshayanfar/os-lab/tree/master/Lab6)

To run the program, execute `make run_dining_philosophers`. It'll be create 5 instances of philosophers and each one tries to eat `EAT_TIMES` times.

### 7. [Deadlock: Bankers algorithm](https://github.com/radinshayanfar/os-lab/tree/master/Lab7)

Compile the program with `make banker` and run it using `./banker.out`. Configure number of resources and customers by changing `NUMBER_OF_RESOURCES` and `NUMBER_OF_CUSTOMERS` definitions. Each customer runs for `MAX_CUSTOMER_ITERATION` iterations and requests resources randomly on each iteration from the banker. The banker then decides whether accepting the request or rejecting it (in order to keep the system in a safe state).

The program reads `Available matrix` from positional arguments and `Max matrix` from `MAX_FILE_NAME` file. For instance the following line initializes code with `Available matrix` = [5 6 6 5 5].

```bash
./banker.out 5 6 6 5 5
```



## Full description

Read the full description (in Persian) [here](https://github.com/radinshayanfar/os-lab/blob/master/OS_LAB_InstructionManual_1398_v02.pdf).