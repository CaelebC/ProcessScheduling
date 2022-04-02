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
// Information on vectors found on https://www.cplusplus.com/reference/vector/vector/
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
#include <tuple>
using namespace std;


// This struct is to create Process objects with the arrival, burst, and priority
struct Process
{
    double arrivalTime;
    double burstTime;
    double priorityNum;
    int processIndex;

    Process(double arrival, double burst, double priority)
    {
        this-> arrivalTime = arrival;
        this-> burstTime = burst;
        this-> priorityNum = priority;
    }

    ~Process(){}
};


// These structs are to be used by the std::sort 
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

// This function is to compute and print the Scheduling Criteria
void computation(double totalBurstTime, double totalTimeElapsed, double totalNumProcesses, double totalWaitingTime, double totalTurnaroundTime, double totalResponseTime)
{
    double cpuUtilization = (totalBurstTime / totalTimeElapsed) * 100;
    double throughput = totalNumProcesses / totalTimeElapsed;
    double waitingTime = totalWaitingTime / totalNumProcesses;
    double turnaroundTime = totalTurnaroundTime / totalNumProcesses;
    double responseTime = totalResponseTime / totalNumProcesses;

    cout << "burst time: " << totalBurstTime << " time elapsed: " << totalTimeElapsed << endl;
    cout << "CPU Utilization: " << cpuUtilization << "%" << endl;
    cout << "Throughput: " << throughput << endl;
    cout << "Waiting time: " << waitingTime << endl;
    cout << "Turnaround time: " << turnaroundTime << endl;
    cout << "Response time: " << responseTime << endl;
}

// These structs are to schedule the processes
struct FCFS 
{
    vector<Process> processesInAlgorithm;
    double totalBurstTime = 0;
    double totalTimeElapsed = 0;
    double totalNumProcesses = processesInAlgorithm.size();
    double totalWaitingTime; 
    double totalTurnaroundTime; 
    double totalResponseTime;
    
    void run()
    {
        std::sort(processesInAlgorithm.begin(), processesInAlgorithm.end(), OrderingByArrival());
        
        for (int n = 0; n < processesInAlgorithm.size(); n++)
        {
            Process p = processesInAlgorithm[n];
            
            if (p.arrivalTime >= totalTimeElapsed)
            {
                double timeDelay = (p.arrivalTime - totalTimeElapsed);
                totalTimeElapsed += timeDelay;
                totalWaitingTime += timeDelay;
            }
            
            totalBurstTime += p.burstTime;
            cout << totalTimeElapsed << " " << p.processIndex << " " << p.burstTime << "X" << endl;
            totalTimeElapsed += p.burstTime;


            cout << "totalwaitingtime: " << totalWaitingTime << endl;
            cout << "totalturnaroundtime: " << totalTurnaroundTime << endl;
            cout << "totalresponsetime: " << totalResponseTime << endl;
            cout << "-----" << endl;

        }
        
        totalTurnaroundTime = totalBurstTime;
        computation(totalBurstTime, totalTimeElapsed, totalNumProcesses, totalWaitingTime, totalTurnaroundTime, totalResponseTime);
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
    double quantum;

    // Variables which make up the details of each process
    double arrival; 
    double burst; 
    double priority;
    
    cin >> testCases; // This gets the 1st line and only the 1st line of the input & stores in the variable

    for(int currentTestCaseNum = 1; currentTestCaseNum <= testCases; currentTestCaseNum++)
    {
        vector<Process> processArray = {};

        cout << "------SEPARATOR REMOVE BEFORE PASSING THIS PROJECT------" << endl;
        cout << currentTestCaseNum << endl;
        cin >> numProcesses >> processName;

        if (processName == "RR")  // This is to catch the quantum of the RR if ever there is one.
        {
            cin >> quantum;
        }
        
        for(int n = 0; n < numProcesses; n++) // Puts process details in a Process object.
        {
            cin >> arrival >> burst >> priority;
            Process p(arrival, burst, priority);
            p.processIndex = n + 1;  // The specs stated that the starting index is 1, not 0.
            processArray.push_back(p);
        }

        if (processName == "FCFS")
        {
            FCFS algo(processArray);
            algo.run();
        }
        else if (processName == "SJF")
        {
            cout << "not implemented yet sorry" << endl;
        }
        else if (processName == "SRTF")
        {
            cout << "not implemented yet sorry" << endl;
        }
        else if (processName == "P")
        {
            cout << "not implemented yet sorry" << endl;
        }
        else if (processName == "RR")
        {
            cout << "not implemented yet sorry" << endl;
        }
        else
        {
            cout << "Wrong process name input" << endl;
        }
    }

    return 0;
}

// Left here for array testing purposes
// for (int n = 0; n < <ARRAY_NAME_HERE>.size(); n++)
// {
//     Process p = <ARRAY_NAME_HERE>[n];
//     cout << p.arrivalTime << " " << p.burstTime << " " << p.priorityNum << endl;
// }