// Caeleb Benjamin D. Camaro, 190875
// April 1, 2022

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
// Changing an object property's value found on https://stackoverflow.com/questions/21141168/why-cant-i-change-objects-in-a-vector
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

    double burstTimeProcessed = 0;
    double waitingTime = 0;
    double turnaroundTime = 0;
    double responseTime = 0;
    bool hasArrived = false;
    bool processIsDone = false;

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
    double responseTime = totalResponseTime / totalNumProcesses;  // Total amount of time the processes ARE STILL WAITING to be executed

    cout << "CPU Utilization: " << cpuUtilization << "%" << endl;
    cout << "Throughput: " << throughput << endl;
    cout << "Waiting time: " << waitingTime << endl;
    cout << "Turnaround time: " << turnaroundTime << endl;
    cout << "Response time: " << responseTime << endl;

    cout << "\n" << "totalBurstTime: " << totalBurstTime << " totalTimeElapsed: " << totalTimeElapsed << " totalNumProcesses: " << totalNumProcesses << endl;
    cout << "totalWaitingTime: " << totalWaitingTime << " totalTurnaroundTime: " << totalTurnaroundTime << " totalResponseTime: " << totalResponseTime << endl;
}

// These structs are to schedule the processes
struct FCFS 
{
    vector<Process> processesInAlgorithm;

    void run()
    {
        std::sort(processesInAlgorithm.begin(), processesInAlgorithm.end(), OrderingByArrival());
        double totalBurstTime = 0;
        double totalTimeElapsed = 0;
        double totalNumProcesses = processesInAlgorithm.size();
        double totalWaitingTime = 0;  // Total ammount of time the processes WENT BACK + ARE STILL WAITING to be executed
        double totalTurnaroundTime = 0;  // Total amount of time the processes STARTING UNTIL ENDING + TIME WHEN IT GOES BACK TO WAITING
        double totalResponseTime = 0;  // Total amount of time the processes ARE STILL WAITING to be executed
        
        for (int n = 0; n < processesInAlgorithm.size(); n++)
        {
            Process p = processesInAlgorithm[n];
            
            if (p.arrivalTime > totalTimeElapsed)  // Process isn't ready yet
            {
                totalTimeElapsed += (p.arrivalTime - totalTimeElapsed);
            }
            else if (p.arrivalTime <= totalTimeElapsed)  // Process is/has been ready
            {
                totalWaitingTime += (totalTimeElapsed - p.arrivalTime);
                totalResponseTime += (totalTimeElapsed - p.arrivalTime);
            }
            
            totalBurstTime += p.burstTime;
            totalTurnaroundTime += p.burstTime;
            
            cout << totalTimeElapsed << " " << p.processIndex << " " << p.burstTime << "X" << endl;
            
            totalTimeElapsed += p.burstTime;  // This has to be here because of the output's format in the specs
        }

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
    vector<Process> processesInAlgorithm;

    void run()
    {
        std::sort(processesInAlgorithm.begin(), processesInAlgorithm.end(), OrderingByArrival());
        vector<Process> tempArray = {};
        double totalBurstTime = 0;
        double totalTimeElapsed = 0;
        double totalNumProcesses = processesInAlgorithm.size();
        double totalWaitingTime = 0;  // Total ammount of time the processes WENT BACK + ARE STILL WAITING to be executed
        double totalTurnaroundTime = 0;  // Total amount of time the processes STARTING UNTIL ENDING + TIME WHEN IT GOES BACK TO WAITING
        double totalResponseTime = 0;  // Total amount of time the processes ARE STILL WAITING to be executed

        for (int n = 0; n < processesInAlgorithm.size(); n++)
        {
            Process p = processesInAlgorithm[n];
            
            if (p.arrivalTime <= totalTimeElapsed)   // Process that is/has been ready is put inside the temporary array to sort by burst time
            {
                tempArray.push_back(p);
                std::sort(tempArray.begin(), tempArray.end(), OrderingByBurst());
                
                // This is to allow the last element of the main array to be processed by still going through code since continue isn't done
                if (processesInAlgorithm.size() - n > 1)
                {
                    continue;
                }
            }

            if (tempArray.empty() == false)
            {
                for (int n = 0; n < tempArray.size(); n++)
                {
                    Process ptemp = tempArray[n];
                    // cout << "CURRENTLY BEING PROCESSED IN tempArray.empty() == false : " << ptemp.arrivalTime << " " << ptemp.burstTime << " " << ptemp.priorityNum << " " << ptemp.processIndex << endl;
            
                    if (ptemp.arrivalTime > totalTimeElapsed)  // Process isn't ready yet
                    {
                        totalTimeElapsed += (ptemp.arrivalTime - totalTimeElapsed);
                    }
                    else if (ptemp.arrivalTime <= totalTimeElapsed)  // Process is/has been ready
                    {
                        totalWaitingTime += (totalTimeElapsed - ptemp.arrivalTime);
                        totalResponseTime += (totalTimeElapsed - ptemp.arrivalTime);
                    }
                    
                    totalBurstTime += ptemp.burstTime;
                    totalTurnaroundTime += ptemp.burstTime;
                    
                    cout << totalTimeElapsed << " " << ptemp.processIndex << " " << ptemp.burstTime << "X" << endl;
                    
                    totalTimeElapsed += ptemp.burstTime;  // This has to be here because of the output's format in the specs
                }

                tempArray = {};
                tempArray.push_back(p);
                std::sort(tempArray.begin(), tempArray.end(), OrderingByBurst());

                if (processesInAlgorithm.size() - n > 1)  // This is to allow the last element of the main array to be processed 
                {
                    continue;
                }
                
                else if ( (tempArray.size() == 1) && (processesInAlgorithm.size() -n == 1) )
                {
                    for (int n = 0; n < tempArray.size(); n++)
                    {
                        Process ptemp = tempArray[n];
                
                        if (ptemp.arrivalTime > totalTimeElapsed)  // Process isn't ready yet
                        {
                            totalTimeElapsed += (ptemp.arrivalTime - totalTimeElapsed);
                        }
                        
                        // This else if statement is removed since the totalWaitingTime & totalResponseTime values will be wrong
                        // else if (ptemp.arrivalTime <= totalTimeElapsed)
                        // {
                        //     totalWaitingTime += (totalTimeElapsed - ptemp.arrivalTime);
                        //     totalResponseTime += (totalTimeElapsed - ptemp.arrivalTime);
                        // }
                        
                        totalBurstTime += ptemp.burstTime;
                        totalTurnaroundTime += ptemp.burstTime;
                        
                        cout << totalTimeElapsed << " " << ptemp.processIndex << " " << ptemp.burstTime << "X" << endl;
                        
                        totalTimeElapsed += ptemp.burstTime;  // This has to be here because of the output's format in the specs
                    }
                }

            }
            else
            {
                // cout << "CURRENTLY BEING PROCESSED IN else: " << p.arrivalTime << " " << p.burstTime << " " << p.priorityNum << " " << p.processIndex << endl;
                totalTimeElapsed += (p.arrivalTime - totalTimeElapsed);
                tempArray.push_back(p);
                std::sort(tempArray.begin(), tempArray.end(), OrderingByBurst());
            }
        }

        computation(totalBurstTime, totalTimeElapsed, totalNumProcesses, totalWaitingTime, totalTurnaroundTime, totalResponseTime);
    }

    SJF(vector<Process> processesInAlgorithm)
    {
        this-> processesInAlgorithm = processesInAlgorithm;
    }

    ~SJF(){}
};

struct SRTF
{
    vector<Process> processesInAlgorithm;

    void run()
    {
        Process dummy(0, 0, 0);
        
        std::sort(processesInAlgorithm.begin(), processesInAlgorithm.end(), OrderingByArrival());
        vector<Process> readyArray = {};
        vector<Process> upcomingArray = {dummy};
        vector<Process> pausedArray = {};
        double totalBurstTime = 0;
        double totalTimeElapsed = 0;
        double totalNumProcesses = processesInAlgorithm.size();
        double totalWaitingTime = 0;  // Total ammount of time the processes WENT BACK + ARE STILL WAITING to be executed
        double totalTurnaroundTime = 0;  // Total amount of time the processes STARTING UNTIL ENDING + TIME WHEN IT GOES BACK TO WAITING
        double totalResponseTime = 0;  // Total amount of time the processes ARE STILL WAITING to be executed

        bool firstTime = true;
        bool lastElement = false;

        while (upcomingArray.empty() == false || lastElement == true)
        {
            for (int n = 0; n < processesInAlgorithm.size(); n++)  
            {   
                Process& p = processesInAlgorithm[n];

                if (p.arrivalTime <= totalTimeElapsed)
                {
                    p.hasArrived = true;
                    readyArray.push_back(p);
                    processesInAlgorithm.erase(processesInAlgorithm.begin());

                    if (processesInAlgorithm.empty() == true)
                    {
                        lastElement == true;
                    }
                    continue;
                }


            }

            std::sort(readyArray.begin(), readyArray.end(), OrderingByBurst());
            upcomingArray = processesInAlgorithm;
            // std::sort(upcomingArray.begin(), upcomingArray.end(), OrderingByBurst());

            for (int n = 0; n < readyArray.size(); n++)
            {
                Process& p = readyArray[n];
                Process& pUpcoming = upcomingArray[n];
                
                if (upcomingArray.empty())
                {
                    Process& pUpcoming = p;
                    lastElement = false;
                }

                cout << "p equals: " << p.arrivalTime << " " << p.burstTime << " " <<  p.priorityNum << " " << p.processIndex << " " << p.burstTimeProcessed << endl;
                cout << "pUpcoming equals: " << pUpcoming.arrivalTime << " " << pUpcoming.burstTime << " " <<  pUpcoming.priorityNum << " " << pUpcoming.processIndex << " " << p.burstTimeProcessed << endl;
                cout << "this is what's inside the processesInAlgorithm array: " << "\n";
                for (int n = 0; n < processesInAlgorithm.size(); n++)
                {
                    Process a = processesInAlgorithm[n];
                    cout << a.arrivalTime << " " << a.burstTime << " " << a.priorityNum << " " << a.processIndex << endl;
                }
                cout << "this is what's inside the readyArray array: " << "\n";
                for (int n = 0; n < readyArray.size(); n++)
                {
                    Process a = readyArray[n];
                    cout << a.arrivalTime << " " << a.burstTime << " " << a.priorityNum << " " << a.processIndex << endl;
                }
                cout << "this is what's inside the upcomingArray array: " << "\n";
                for (int n = 0; n < upcomingArray.size(); n++)
                {
                    Process a = upcomingArray[n];
                    cout << a.arrivalTime << " " << a.burstTime << " " << a.priorityNum << " " << a.processIndex << endl;
                }
                cout << "\n";


                if (upcomingArray.empty() == false)
                {
                    if (p.burstTime > pUpcoming.arrivalTime)  // When the 'upcoming' process has less time remaining
                    {
                        double pU_Arrival = pUpcoming.arrivalTime;

                        totalBurstTime += pU_Arrival;
                        p.burstTime -= pU_Arrival;
                        p.burstTimeProcessed = pU_Arrival;

                        cout << totalTimeElapsed << " " << p.processIndex << " " << p.burstTimeProcessed << endl;
                        totalTimeElapsed += pUpcoming.arrivalTime;  // This has to be here because of the output's format in the specs
                        readyArray.push_back(pUpcoming);
                        upcomingArray.erase(upcomingArray.begin());
                        firstTime = false;
                        break;
                    }
                    else if (p.burstTime <= pUpcoming.arrivalTime)
                    {
                        double p_Burst = p.burstTime;

                        totalBurstTime += p_Burst;
                        p.burstTimeProcessed = p_Burst;
                        p.burstTime -= p_Burst;
                        p.processIsDone = true;

                        cout << totalTimeElapsed << " " << p.processIndex << " " << p.burstTimeProcessed << "X" << endl;
                        totalTimeElapsed += p_Burst;  // This has to be here because of the output's format in the specs
                        readyArray.erase(readyArray.begin());
                        // upcomingArray.erase(upcomingArray.begin());
                        firstTime = false;
                        break;
                    }
                }
                else
                {
                    std::sort(readyArray.begin(), readyArray.end(), OrderingByBurst());

                    if (p.burstTime > pUpcoming.burstTime)  // When the 'upcoming' process has less time remaining
                    {
                        double pU_Burst = pUpcoming.burstTime;

                        totalBurstTime += pU_Burst;
                        p.burstTime -= pU_Burst;
                        p.burstTimeProcessed = pU_Burst;

                        cout << totalTimeElapsed << " " << p.processIndex << " " << p.burstTimeProcessed << endl;
                        totalTimeElapsed += pU_Burst;  // This has to be here because of the output's format in the specs
                        readyArray.push_back(pUpcoming);
                        upcomingArray.erase(upcomingArray.begin());
                        break;
                    }
                    else if (p.burstTime <= pUpcoming.burstTime)
                    {
                        double p_Burst = p.burstTime;

                        totalBurstTime += p_Burst;
                        p.burstTimeProcessed = p_Burst;
                        p.burstTime -= p_Burst;
                        p.processIsDone = true;

                        cout << totalTimeElapsed << " " << p.processIndex << " " << p.burstTimeProcessed << "X" << endl;
                        totalTimeElapsed += p_Burst;  // This has to be here because of the output's format in the specs
                        readyArray.erase(readyArray.begin());
                        // upcomingArray.erase(upcomingArray.begin());
                        break;
                    }
                }
                
            }

            cout << "----------end of while loop----------" << "\n" << endl;
            
        }
    }

    SRTF(vector<Process> processesInAlgorithm)
    {
        this-> processesInAlgorithm = processesInAlgorithm;
    }

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

        cout << "------SEPARATOR IN MAIN. REMOVE BEFORE SUBMITTING------" << endl;
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
            SJF algo(processArray);
            algo.run();
        }
        else if (processName == "SRTF")
        {
            SRTF algo(processArray);
            algo.run();
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
// for (int n = 0; n < ARRAY_NAME_HERE.size(); n++)
// {
//     Process a = ARRAY_NAME_HERE[n];
//     cout << a.arrivalTime << " " << a.burstTime << " " << a.priorityNum << " " << a.processIndex << endl;
// }