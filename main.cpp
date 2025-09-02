#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>

#include "main.h"

#define BUFFER 256

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    string procName; 
    cout << "Enter name of searching process (all - for all): ";
    cin.ignore();
    getline(cin, procName);

    char buffer[BUFFER];

    int counterOfProc = 0;
    vector<processInfo> procInfo;

    bool progLeave = false;
    setRawMode(true);

    int maxProcLen;

    while(!progLeave) {
        size_t termWidth = getTermSize();
        const size_t pidWidth = 12;
        int procLabelSize = 15;
        maxProcLen = termWidth - pidWidth - procLabelSize - 2;
        if (maxProcLen < 8) {
            maxProcLen = 8;
        }

        FILE* pipe = popen("ps -A -o pid,%cpu,%mem,comm", "r");
        if (!pipe) {
            setRawMode(false);
            cout << "Error: can't open pipe" << endl;
            exit(EXIT_FAILURE); 
        }

        cout << "\033[H\033[2J" << flush;
        procInfo.clear();
        counterOfProc = 0;

        cout << "PID\t\tCPU\t\tMEM\t\tCOMM\n";

        bool isFirstLine = true;
        while(fgets(buffer, BUFFER, pipe) != NULL) {
            if (isFirstLine) {
                isFirstLine = false;
                continue;
            }

            istringstream iss(buffer);
            int pid;
            double cpu;
            double mem;
            string comm;

            if (iss >> pid >> cpu >> mem >> comm) {
                getline(iss >> ws, comm);
                procInfo.push_back({pid, cpu, mem, comm}); 
            }
        }
        pclose(pipe);

        for (const auto& proc : procInfo) {
            string processName = proc.command;
            
            if (procName == "all") {
                if (processName.length() > maxProcLen) {
                    processName = processName.substr(0, maxProcLen - 3) + "...";
                }

                counterOfProc++;

                cout << "";
            }
            else if (processName.find(procName) != string::npos) {
                if (processName.length() > maxProcLen) {
                    processName = processName.substr(0, maxProcLen - 3) + "...";
                }

                counterOfProc++;
                cout << "";
            }
    
            if (procName == "all") {
                cout << "Count of working processes: " << counterOfProc << "\n"; 
            }
            else {
                cout << "Count of " << procName << " processes: " << counterOfProc << "\n"; 
            }
    
            if (kbhit()) {
                char c = getchar();
                if (c == 'q') {
                    break;
                }
            }
        }
        this_thread::sleep_for(chrono::milliseconds(200));
    }

    setRawMode(false);
    system("clear");

    return 0;
}