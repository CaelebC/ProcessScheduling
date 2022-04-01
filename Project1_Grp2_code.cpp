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
using namespace std;

struct FCFS {
    int arrivalTime;
    int burstTime;
    int priorityNum;

    FCFS(int arrival, int burst, int priority){
        this-> arrivalTime = arrival;
        this-> burstTime = burst;
        this-> priorityNum = priority;
    }

    ~FCFS(){}
};

struct SJF {
    int arrivalTime;
    int burstTime;
    int priorityNum;
    
    SJF(int arrival, int burst, int priority){
        this-> arrivalTime = arrival;
        this-> burstTime = burst;
        this-> priorityNum = priority;
    }

    ~SJF(){}
};

struct SRTF {
    int arrivalTime;
    int burstTime;
    int priorityNum;
    
    SRTF(int arrival, int burst, int priority){
        this-> arrivalTime = arrival;
        this-> burstTime = burst;
        this-> priorityNum = priority;
    }

    ~SRTF(){}
};

struct Prio {
    int arrivalTime;
    int burstTime;
    int priorityNum;
    
    Prio(int arrival, int burst, int priority){
        this-> arrivalTime = arrival;
        this-> burstTime = burst;
        this-> priorityNum = priority;
    }

    ~Prio(){}
};

struct RR {
    int arrivalTime;
    int burstTime;
    int priorityNum;
    
    RR(int arrival, int burst, int priority){
        this-> arrivalTime = arrival;
        this-> burstTime = burst;
        this-> priorityNum = priority;
    }

    ~RR(){}
};


int main(){
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
    while(cin) {
        cin >> numProcesses >> processName >> quantum;

        for(int n = numProcesses; n > 0; n--){
            cin >> arrival >> burst >> priority;

        }

        
        cout << "number Processes: " << numProcesses << " ";
        cout << "name Process: " << processName << " ";
        cout << "Quantum number: " << quantum << " ";
        cout << "" << endl;

        

    }
    
    
    return 0;
}