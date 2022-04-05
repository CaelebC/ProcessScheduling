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
// Dynamic arrays (vectors) discovered on https://stackoverflow.com/questions/755835/how-to-add-element-to-c-array
// Information on vectors found on https://www.cplusplus.com/reference/vector/vector/
// Changing an object property's value found on https://stackoverflow.com/questions/21141168/why-cant-i-change-objects-in-a-vector
// SRTF algorithm explanation https://www.youtube.com/watch?v=_QcX99B-zbU
// Preemptive Priority algorithm explanation https://www.youtube.com/watch?v=23h3lkHNL_s
// Round Robin algorithm explanation https://www.youtube.com/watch?v=-jFGYDfWkXI
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
    double playPauseTime = 0; 
    double originalStartTime;
    double originalBurstTime;

    double waitingTime = 0;
    double turnaroundTime = 0;
    double responseTime = 0;

    bool firstRun = true;
    bool inWaiting = false;

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
        vector<Process> readyArray = {};
        double processesCompleted = 0;

        double totalBurstTime = 0;
        double totalTimeElapsed = 0;
        double totalNumProcesses = processesInAlgorithm.size();
        double totalWaitingTime = 0;  // Total ammount of time the processes WENT BACK + ARE STILL WAITING to be executed
        double totalTurnaroundTime = 0;  // Total amount of time the processes STARTING UNTIL ENDING + TIME WHEN IT GOES BACK TO WAITING
        double totalResponseTime = 0;  // Total amount of time the processes ARE STILL WAITING to be executed

        while (processesCompleted < totalNumProcesses)
        {
            // Adds processes which have arrived in the readyArray
            int n = 0;
            while (processesInAlgorithm.empty() == false && n < processesInAlgorithm.size())
            {
                Process& p = processesInAlgorithm[n];

                if (p.arrivalTime <= totalTimeElapsed)
                {
                    p.originalBurstTime = p.burstTime;
                    readyArray.push_back(p);
                    processesInAlgorithm.erase(processesInAlgorithm.begin());
                    
                    // This allows for last process to be added to the readyArray
                    if (processesInAlgorithm.empty() == true)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    n += 1;
                    continue;
                }
            }
            
            for (int n = 0; n < processesInAlgorithm.size(); n++)  
            {   
                Process& p = processesInAlgorithm[0];

                if (p.arrivalTime <= totalTimeElapsed)
                {
                    p.originalBurstTime = p.burstTime;
                    readyArray.push_back(p);
                    processesInAlgorithm.erase(processesInAlgorithm.begin());
                    
                    // This allows for last process to be added to the readyArray
                    if (processesInAlgorithm.empty() == true)
                    {
                        break;
                    }
                    continue;
                }
            }

            std::sort(readyArray.begin(), readyArray.end(), OrderingByBurst());

            // If else loop to determine if readyArray is empty, if it is then totalElapsedTime will be incremented
            if (readyArray.empty() == false)
            {
                
                Process& p = readyArray[0];
                
                p.burstTimeProcessed += p.burstTime;
                totalBurstTime += p.burstTime;
                totalTurnaroundTime += p.burstTime;
                totalWaitingTime += (totalTimeElapsed - p.arrivalTime);
                totalResponseTime += (totalTimeElapsed - p.arrivalTime);

                cout << totalTimeElapsed << " " << p.processIndex << " " << p.burstTimeProcessed << "X" << endl;
                totalTimeElapsed += p.burstTime;  // This has to be here because of the output's format in the specs
                p.burstTime -= p.burstTime;
                processesCompleted += 1;
                readyArray.erase(readyArray.begin());

                continue;
            }
            else
            {
                totalTimeElapsed += 1;
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
        std::sort(processesInAlgorithm.begin(), processesInAlgorithm.end(), OrderingByArrival());
        vector<Process> readyArray = {};
        double processesCompleted = 0;

        // This is to keep track of the previous process before switching.
        // The dummy object is needed to be able to create a Process obj with a & (process obj that could be modified).
        Process dummy(0, 0, 0);
        Process& previousProcess = dummy;  

        double totalBurstTime = 0;
        double totalTimeElapsed = 0;
        double totalNumProcesses = processesInAlgorithm.size();
        double totalWaitingTime = 0;  // Total ammount of time the processes WENT BACK + ARE STILL WAITING to be executed
        double totalTurnaroundTime = 0;  // Total amount of time the processes STARTING UNTIL ENDING + TIME WHEN IT GOES BACK TO WAITING
        double totalResponseTime = 0;  // Total amount of time the processes ARE STILL WAITING to be executed


        while (processesCompleted < totalNumProcesses)
        {
            double runTime = 1;  // Every process will be run for every unit of time dictated here

            // Adds processes which have arrived in the readyArray
            int n = 0;
            while (processesInAlgorithm.empty() == false && n < processesInAlgorithm.size())
            {
                Process& p = processesInAlgorithm[n];

                if (p.arrivalTime <= totalTimeElapsed)
                {
                    p.originalBurstTime = p.burstTime;
                    readyArray.push_back(p);
                    processesInAlgorithm.erase(processesInAlgorithm.begin());
                    
                    // This allows for last process to be added to the readyArray
                    if (processesInAlgorithm.empty() == true)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    n += 1;
                    continue;
                }
            }

            // If else loop to determine if readyArray is empty, if it is then totalElapsedTime will be incremented
            if (readyArray.empty() == false)
            {
                Process& p = readyArray[0];

                // To record times when the process goes in/out of the readyArray
                if (p.firstRun || p.inWaiting)
                {
                    p.playPauseTime = totalTimeElapsed;
                    previousProcess = p;
                    
                    if (p.firstRun)
                    {
                        p.originalStartTime = p.playPauseTime;
                    }

                    p.firstRun = false;
                    p.inWaiting = false;
                }

                // Detects if a new process is in readyArray, which could mean a switch in process
                if (readyArray.size() > 1)  
                {
                    bool wentThroughLoop = false;

                    for (int n = 1; n < readyArray.size(); n++)
                    {
                        Process& pNext = readyArray[n];
                        if (p.burstTime > pNext.burstTime)
                        {
                            cout << p.playPauseTime << " " << p.processIndex << " " << p.burstTimeProcessed << endl;
                            p.burstTimeProcessed = 0;
                            p.inWaiting = true;
                            previousProcess = p;
                            wentThroughLoop = true;
                            std::sort(readyArray.begin(), readyArray.end(), OrderingByBurst());
                            break;
                        }
                    }
                    if (wentThroughLoop)
                    {
                        continue;
                    }
                }
                  
                p.burstTime -= runTime;
                p.burstTimeProcessed += runTime;
                totalBurstTime += runTime;

                // When burstTime is 0, the totals variables are modified accordingly
                if (p.burstTime == 0)
                {
                    processesCompleted += 1;
                    cout << p.playPauseTime << " " << p.processIndex << " " << p.burstTimeProcessed << "X" << endl;
                    totalTimeElapsed += runTime;
                    p.turnaroundTime = (totalTimeElapsed - p.originalStartTime);
                    p.responseTime = (p.originalStartTime - p.arrivalTime);
                    p.waitingTime = (p.turnaroundTime - p.originalBurstTime) + p.responseTime;
                    
                    totalTurnaroundTime += p.turnaroundTime;
                    totalResponseTime += p.responseTime;
                    totalWaitingTime += p.waitingTime;
                    readyArray.erase(readyArray.begin());
                    continue;
                }
                else
                {
                    totalTimeElapsed += runTime;
                    previousProcess = p;
                    continue;
                }
            }
            else
            {
                totalTimeElapsed += runTime;
            }
        }

        computation(totalBurstTime, totalTimeElapsed, totalNumProcesses, totalWaitingTime, totalTurnaroundTime, totalResponseTime);
    }

    SRTF(vector<Process> processesInAlgorithm)
    {
        this-> processesInAlgorithm = processesInAlgorithm;
    }

    ~SRTF(){}
};

struct Prio
{
    vector<Process> processesInAlgorithm;

    void run()
    {        
        std::sort(processesInAlgorithm.begin(), processesInAlgorithm.end(), OrderingByArrival());
        vector<Process> readyArray = {};
        double processesCompleted = 0;

        // This is to keep track of the previous process before switching.
        // The dummy object is needed to be able to create a Process obj with a & (process obj that could be modified).
        Process dummy(0, 0, 0);
        Process& previousProcess = dummy;  

        double totalBurstTime = 0;
        double totalTimeElapsed = 0;
        double totalNumProcesses = processesInAlgorithm.size();
        double totalWaitingTime = 0;  // Total ammount of time the processes WENT BACK + ARE STILL WAITING to be executed
        double totalTurnaroundTime = 0;  // Total amount of time the processes STARTING UNTIL ENDING + TIME WHEN IT GOES BACK TO WAITING
        double totalResponseTime = 0;  // Total amount of time the processes ARE STILL WAITING to be executed


        while (processesCompleted < totalNumProcesses)
        {
            double runTime = 1;  // Every process will be run for every unit of time dictated here

            // Adds processes which have arrived in the readyArray
            for (int n = 0; n < processesInAlgorithm.size(); n++)  
            {   
                Process& p = processesInAlgorithm[n];

                if (p.arrivalTime <= totalTimeElapsed)
                {
                    p.originalBurstTime = p.burstTime;
                    readyArray.push_back(p);
                    processesInAlgorithm.erase(processesInAlgorithm.begin());
                    continue;
                }
            }

            // If else loop to determine if readyArray is empty, if it is then totalElapsedTime will be incremented
            if (readyArray.empty() == false)
            {
                Process& p = readyArray[0];

                // To record times when the process goes in/out of the readyArray
                if (p.firstRun || p.inWaiting)
                {
                    p.playPauseTime = totalTimeElapsed;
                    previousProcess = p;
                    
                    if (p.firstRun)
                    {
                        p.originalStartTime = p.playPauseTime;
                    }

                    p.firstRun = false;
                    p.inWaiting = false;
                }

                // Detects if a new process is in readyArray, which could mean a switch in process
                if (readyArray.size() > 1)
                {
                    bool wentThroughLoop = false;

                    for (int n = 1; n < readyArray.size(); n++)
                    {
                        Process& pNext = readyArray[n];
                        if (p.priorityNum > pNext.priorityNum)
                        {
                            cout << p.playPauseTime << " " << p.processIndex << " " << p.burstTimeProcessed << endl;
                            p.burstTimeProcessed = 0;
                            p.inWaiting = true;
                            previousProcess = p;
                            wentThroughLoop = true;
                            std::sort(readyArray.begin(), readyArray.end(), OrderingByPriority());
                            break;
                        }
                    }
                    if (wentThroughLoop)
                    {
                        continue;
                    }
                }
                  
                p.burstTime -= runTime;
                p.burstTimeProcessed += runTime;
                totalBurstTime += runTime;

                // When burstTime is 0, the totals variables are modified accordingly
                if (p.burstTime == 0)
                {
                    processesCompleted += 1;
                    cout << p.playPauseTime << " " << p.processIndex << " " << p.burstTimeProcessed << "X" << endl;
                    totalTimeElapsed += runTime;
                    p.turnaroundTime = (totalTimeElapsed - p.originalStartTime);
                    p.responseTime = (p.originalStartTime - p.arrivalTime);
                    p.waitingTime = (p.turnaroundTime - p.originalBurstTime) + p.responseTime;
                    
                    totalTurnaroundTime += p.turnaroundTime;
                    totalResponseTime += p.responseTime;
                    totalWaitingTime += p.waitingTime;
                    readyArray.erase(readyArray.begin());
                    continue;
                }
                else
                {
                    totalTimeElapsed += runTime;
                    previousProcess = p;
                    continue;
                }
            }
            else
            {
                totalTimeElapsed += runTime;
            }
        }

        computation(totalBurstTime, totalTimeElapsed, totalNumProcesses, totalWaitingTime, totalTurnaroundTime, totalResponseTime);
    }

    Prio(vector<Process> processesInAlgorithm)
    {
        this-> processesInAlgorithm = processesInAlgorithm;
    }

    ~Prio(){}
};

struct RR
{
    vector<Process> processesInAlgorithm;
    double quantum;

    void run()
    {        
        std::sort(processesInAlgorithm.begin(), processesInAlgorithm.end(), OrderingByArrival());
        vector<Process> readyArray = {};
        double processesCompleted = 0;

        // This is to be able to temporarily store the current process after the its removal in the readyArray.
        // The dummy object is needed to be able to create a Process obj with a & (process obj that could be modified).
        Process dummy(0, 0, 0);
        Process& tempProcess = dummy;  

        double totalBurstTime = 0;
        double totalTimeElapsed = 0;
        double totalNumProcesses = processesInAlgorithm.size();
        double totalWaitingTime = 0;  // Total ammount of time the processes WENT BACK + ARE STILL WAITING to be executed
        double totalTurnaroundTime = 0;  // Total amount of time the processes STARTING UNTIL ENDING + TIME WHEN IT GOES BACK TO WAITING
        double totalResponseTime = 0;  // Total amount of time the processes ARE STILL WAITING to be executed


        while (processesCompleted < totalNumProcesses)
        {
            double runTime = quantum;  // Every process will be run for every unit of time dictated here

            // Adds processes which have arrived in the readyArray
            for (int n = 0; n < processesInAlgorithm.size(); n++)  
            {   
                Process& p = processesInAlgorithm[n];
                if (p.arrivalTime <= totalTimeElapsed)
                {
                    p.originalBurstTime = p.burstTime;
                    readyArray.push_back(p);
                    processesInAlgorithm.erase(processesInAlgorithm.begin());
                    continue;
                }
            }

            // If else loop to determine if readyArray is empty, if it is then totalElapsedTime will be incremented
            if (readyArray.empty() == false)
            {
                Process& p = readyArray[0];

                // To record times when the process goes in/out of the readyArray
                if (p.firstRun || p.inWaiting)
                {
                    p.playPauseTime = totalTimeElapsed;
                    
                    if (p.firstRun)
                    {
                        p.originalStartTime = p.playPauseTime;
                    }

                    p.firstRun = false;
                    p.inWaiting = false;
                }
   
                // Processes will classify either as higher than the quantum, or smaller than/equal to the quantum.
                // The processing happens inside this if else statement.
                //
                // Erasing the 0th element of the readyArray is then followed by appending any newly arrived process, 
                // followed by the process that's been ran (if still not done yet)
                if (p.burstTime <= runTime)
                {
                    tempProcess = p;
                    readyArray.erase(readyArray.begin());
                    
                    totalBurstTime += tempProcess.burstTime;
                    tempProcess.burstTimeProcessed += tempProcess.burstTime;

                    cout << tempProcess.playPauseTime << " " << tempProcess.processIndex << " " << tempProcess.burstTimeProcessed << "X" << endl;
                    tempProcess.burstTimeProcessed = 0;
                    totalTimeElapsed += tempProcess.burstTime;
                    tempProcess.burstTime -= tempProcess.burstTime;

                    tempProcess.turnaroundTime = (totalTimeElapsed - tempProcess.originalStartTime);
                    tempProcess.responseTime = (tempProcess.originalStartTime - tempProcess.arrivalTime);
                    tempProcess.waitingTime = (tempProcess.turnaroundTime - tempProcess.originalBurstTime) + tempProcess.responseTime;
                    
                    totalTurnaroundTime += tempProcess.turnaroundTime;
                    totalResponseTime += tempProcess.responseTime;
                    totalWaitingTime += tempProcess.waitingTime;
                    
                    // This for loop is to add any processes that might've been ready while the current process was processing
                    for (int n = 0; n < processesInAlgorithm.size(); n++)  
                    {   
                        Process& p = processesInAlgorithm[n];
                        if (p.arrivalTime <= totalTimeElapsed)
                        {
                            p.originalBurstTime = p.burstTime;
                            readyArray.insert(readyArray.begin(), p);
                            processesInAlgorithm.erase(processesInAlgorithm.begin());
                        }
                    }

                    processesCompleted += 1;
                    continue;
                }
                else
                {
                    tempProcess = p;
                    readyArray.erase(readyArray.begin());
                    
                    tempProcess.inWaiting = true;
                    tempProcess.burstTime -= runTime;
                    tempProcess.burstTimeProcessed += runTime;
                    totalBurstTime += runTime;

                    cout << tempProcess.playPauseTime << " " << tempProcess.processIndex << " " << tempProcess.burstTimeProcessed << endl;
                    tempProcess.burstTimeProcessed = 0;
                    totalTimeElapsed += runTime;

                    // This for loop is to add any processes that might've been ready while the current process was processing
                    for (int n = 0; n < processesInAlgorithm.size(); n++)  
                    {   
                        Process& p = processesInAlgorithm[n];
                        if (p.arrivalTime <= totalTimeElapsed)
                        {
                            p.originalBurstTime = p.burstTime;
                            readyArray.insert(readyArray.begin(), p);
                            processesInAlgorithm.erase(processesInAlgorithm.begin());
                        }
                    }

                    readyArray.push_back(tempProcess);

                    continue;
                }
            }
            else
            {
                totalTimeElapsed += 1;
            }
        }

        computation(totalBurstTime, totalTimeElapsed, totalNumProcesses, totalWaitingTime, totalTurnaroundTime, totalResponseTime);
    }

    RR(vector<Process> processesInAlgorithm, double quantum)
    {
        this-> processesInAlgorithm = processesInAlgorithm;
        this-> quantum = quantum;
    }

    ~RR(){}
};


int main()
{
    // Used to get the number of test cases and to store every line of the input
    string line;
    int testCases;

    // Variables for determining which algorithm to use and the number of processes to process.
    // Quantum is only used for RR algorithm.
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

        // cout << "------SEPARATOR IN MAIN. COMMENT OUT BEFORE SUBMITTING------" << endl;

        cout << currentTestCaseNum << endl;
        cin >> numProcesses >> processName;

        if (processName == "RR")  // This is to catch the quantum of the RR if ever there is one.
        {
            cin >> quantum;
        }
        
        // Puts process details in a Process object.
        for(int n = 0; n < numProcesses; n++) 
        {
            cin >> arrival >> burst >> priority;
            Process p(arrival, burst, priority);
            p.processIndex = n + 1;  // The specs stated that the starting index of a process is 1, not 0.
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
            Prio algo(processArray);
            algo.run();
        }
        else if (processName == "RR")
        {
            RR algo(processArray, quantum);
            algo.run();
        }
        else
        {
            cout << "Wrong process name input, please check input file." << endl;
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

// cout << "p --> " << "index: " << p.processIndex << "  arrivalTime: " << p.arrivalTime << "  burstTime: " << p.burstTime << "  burstTimeProcessed: " << p.burstTimeProcessed << "  inWaiting: " << p.inWaiting << endl;
// cout << "this is what's inside the processesInAlgo array: " << "\n";
// for (int n = 0; n < processesInAlgorithm.size(); n++)
// {
//     Process a = processesInAlgorithm[n];
//     cout << "index: " << a.processIndex << "  arrivalTime: " << a.arrivalTime << "  burstTime: " << a.burstTime << "  burstTimeProcessed: " << a.burstTimeProcessed << endl;
// }
// cout << "this is what's inside the readyArray array: " << "\n";
// for (int n = 0; n < readyArray.size(); n++)
// {
//     Process a = readyArray[n];
//     cout << "index: " << a.processIndex << "  arrivalTime: " << a.arrivalTime << "  burstTime: " << a.burstTime << "  burstTimeProcessed: " << a.burstTimeProcessed << endl;
// }
// cout << endl;