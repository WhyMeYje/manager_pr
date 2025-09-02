#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>

struct processInfo {
    int pid;
    double cpu;
    double mem;
    std::string comm;
};

#define BUFFER 256

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    string procName; 
    cout << "Enter name of searching process (all - for all): ";
    getline(cin, procName);

    char buffer[BUFFER];

    int counterOfProc = 0;
    vector<processInfo> procInfo;

    bool progLeave = false;
    // setRawMode(true); // если реализовано

    int maxProcLen = 20;

    while (!progLeave) {
        FILE* pipe = popen("ps aux", "r");
        if (!pipe) {
            // setRawMode(false);
            cout << "Error: can't open pipe" << endl;
            exit(EXIT_FAILURE); 
        }

        cout << "\033[H\033[2J" << flush;
        procInfo.clear();
        counterOfProc = 0;

        cout << "PID\t\tCPU\t\tMEM\t\tCOMM\n";

        bool isFirstLine = true;
        while (fgets(buffer, BUFFER, pipe) != NULL) {
            if (isFirstLine) {
                isFirstLine = false;
                continue;
            }
            std::istringstream iss(buffer);
            std::string user;
            int pid;
            double cpu, mem;
            std::string vsz, rss, tty, stat, start, time, comm;

            iss >> user >> pid >> cpu >> mem >> vsz >> rss >> tty >> stat >> start >> time >> comm;
            getline(iss >> ws, comm);

            procInfo.push_back({pid, cpu, mem, comm});
        }
        pclose(pipe);

        for (const auto& proc : procInfo) {
            string processName = proc.comm;
            if (procName == "all" || processName.find(procName) != string::npos) {
                if (processName.length() > maxProcLen) {
                    processName = processName.substr(0, maxProcLen - 3) + "...";
                }
                counterOfProc++;
                cout << setw(10) << proc.pid << "\t"
                     << setw(8) << fixed << setprecision(2) << proc.cpu << "\t"
                     << setw(8) << fixed << setprecision(2) << proc.mem << "\t"
                     << processName << "\n";
            }
        }

        if (procName == "all") {
            cout << "Count of working processes: " << counterOfProc << "\n"; 
        }
        else {
            cout << "Count of " << procName << " processes: " << counterOfProc << "\n"; 
        }

        // if (kbhit()) { ... }

        this_thread::sleep_for(chrono::milliseconds(200));
    }

    // setRawMode(false);
    system("clear");

    return 0;
}
