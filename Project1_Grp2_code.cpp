// Caeleb Benjamin D. Camaro, 190875
// March 28, 2022

// I certify that this submission complies with the DISCS Academic Integrity Policy.

// If I have discussed my C++ language code with anyone other than
// my instructor(s), my groupmate(s), the teaching assistant(s),
// the extent of each discussion has been clearly noted along with a proper
// citation in the comments of my program.

// If any C++ language code or documentation used in my program
// was obtained from another source, either modified or unmodified, such as a
// textbook, website, or another individual, the extent of its use has been
// clearly noted along with a proper citation in the comments of my/our program.
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////
// cite your sources here, if any
// 
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////


#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;


// This struct is to create Process objects with the arrival, burst, and priority
struct Process
{
    int arrivalTime;
    int burstTime;
    int priorityNum;

    Process(int arrival, int burst, int priority)
    {
        this-> arrivalTime = arrival;
        this-> burstTime = burst;
        this-> priorityNum = priority;
    }

    ~Process(){}
};


// These structs are for using the sort algorithm 
struct OrderingByArrival
{
    inline bool operator() (const Process& struct1, const Process& struct2)
    {
        return struct1.arrivalTime < struct2.arrivalTime;
    }
};

struct OrderingByBurst
{
    inline bool operator() (const Process& struct1, const Process& struct2)
    {
        return struct1.burstTime < struct2.burstTime;
    }
};

struct OrderingByPriority
{
    inline bool operator() (const Process& struct1, const Process& struct2)
    {
        return struct1.priorityNum < struct2.priorityNum;
    }
};


// These structs are for the schedulings
struct FCFS 
{
    vector<Process> processesInAlgorithm;
    int elapsedTime;
    int processIndex;

    int cpuUtilization;
    int throughput;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
    
    void run()
    {
        std::sort(processesInAlgorithm.begin(), processesInAlgorithm.end(), OrderingByArrival());
        
        for (int n = 0; n < processesInAlgorithm.size(); n++)
        {
            Process p = processesInAlgorithm[n];
            cout << p.arrivalTime << " " << p.burstTime << " " << p.priorityNum << endl;
        }


        //  Process p = processesInAlgorithm[n];
        //  cout << p.arrivalTime << p.burstTime << p.priorityNum << endl;

    }

    FCFS(vector<Process> processesInAlgorithm)
    {
        this-> processesInAlgorithm = processesInAlgorithm;
    }

    ~FCFS(){}
};

struct SJF
{


    ~SJF(){}
};

struct SRTF
{
    

    ~SRTF(){}
};

struct Prio
{
    

    ~Prio(){}
};

struct RR
{
    

    ~RR(){}
};


int main()
{
    // Used to get the number of test cases and to store every line of the input
    string line;
    int testCases;

    // Variables for determining which algorithm to use and the number of processes to process
    // Quantum is only really used for RR algo
    int numProcesses;
    string processName;
    int quantum;

    // Variables which make up the details of each process
    int arrival; 
    int burst; 
    int priority;

    // This gets the 1st line and only the 1st line of the input & stores in the variable
    cin >> testCases;

    for(int n = 1; n <= testCases; n++)
    {
        // Information on lists found on https://www.guru99.com/cpp-list.html
        vector<Process> processArray = {};

        cin >> numProcesses >> processName;
        // cout << testCases << " " << processName << " " << numProcesses << endl;

        // This is to catch the quantum of the RR if ever there is one.
        if (processName == "RR")
        {
            cin >> quantum;
        }

        // Puts process details in a Process object.
        for(int n = numProcesses; n > 0; n--)
        {
            cin >> arrival >> burst >> priority;
            Process p(arrival, burst, priority);
            processArray.push_back(p);
        }

        // Determines which algorithm to use for scheduling.
        if (processName == "FCFS")
        {
            cout << n << endl;
            FCFS algo(processArray);
            algo.run();
        }

        else
        {
            cout << "not implemented yet sorry lods" << endl;
        }

        // for (Process x: processArray){
        //     cout << x.arrivalTime << " " << x.burstTime << " " << x.priorityNum << " " << endl;
        // }
    }
    
    return 0;
}