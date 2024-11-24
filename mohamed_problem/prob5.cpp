#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <fstream>

using namespace std;

class process {
private:
    string name;
    int pid;
    int memoryUsage;
    string sessionName;
    string sessionID;

public:
    process(const string& name, int pid, int memoryUsage, const string& sessionName, string sessionID) : name(name), pid(pid), memoryUsage(memoryUsage), sessionName(sessionName), sessionID(sessionID) {}
    string getName() const {
        return name;
    }
    int getPid() const {
        return pid;
    }
    int getMemoryUsage() const {
        return memoryUsage;
    }
    string getSessionName() const {
        return sessionName;
    }
    string getSessionID() const {
        return sessionID;
    }
    void display() const {
        cout << setw(25) << left << name
             << setw(30) << pid
             << setw(30) << memoryUsage << "KB"  // Adjusted memory usage column
             << setw(30) << sessionName
             << sessionID << endl;
    }
};

class processes {
private:
    vector<process> pro;
    static int clean_memory_from_string(const string& memoryStr) {
        string mem = memoryStr;
        mem.erase(remove(mem.begin(), mem.end(), ','), mem.end());
        try {
            return stoi(mem);
        }
        catch (...) {
            return 0;
        }
    }
public:
    void loadProcesses() {
        pro.clear();
        system("tasklist /FO CSV > processes.csv");
        ifstream ifs("processes.csv");
        string line;
        bool skipHeader = true;

        while (getline(ifs, line)) {
            if (skipHeader) {
                skipHeader = false;
                continue; // Skip the header line
            }

            stringstream ss(line);
            string name, pid_str, session_name, session_id, memoryStr;

            getline(ss, name, ',');
            getline(ss, pid_str, ',');
            getline(ss, session_name, ',');
            getline(ss, session_id, ',');
            getline(ss, memoryStr, ',');
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            name.erase(remove(name.begin(), name.end(), '"'), name.end());
            pid_str.erase(remove(pid_str.begin(), pid_str.end(), '"'), pid_str.end());
            pid_str.erase(remove(pid_str.begin(), pid_str.end(), ' '), pid_str.end());
            memoryStr.erase(remove(memoryStr.begin(), memoryStr.end(), '"'), memoryStr.end());

            try {
                int pid = stoi(pid_str); // Safely convert pid_str to an integer
                int memoryUsage = clean_memory_from_string(memoryStr);
                pro.emplace_back(name, pid, memoryUsage,session_name,session_id);
            } catch (const invalid_argument& e) {
                cerr << "Error: Unable to convert PID to integer: " << pid_str << endl;
                continue;
            } catch (const out_of_range& e) {
                cerr << "Error: PID out of range: " << pid_str << endl;
                continue;
            }
        }
    }
    void sortByName() {
        cout<<"Sort from first smaller character or bigger character"<<endl;
        cout<<"1, smaller"<<endl;
        cout<<"2, bigger"<<endl;
        string choice;
        cin>>choice;
        while (choice != "1" && choice != "2") {
            cout<<"Please enter 1 or 2: ";
            cin.clear();
            cin>>choice;
        }
        if (choice == "1") {
            sort(pro.begin(), pro.end(),
                  [](const process& a, const process& b) { return a.getName() < b.getName(); });
        }
        else if (choice == "2") {
            sort(pro.rbegin(), pro.rend(),
                  [](const process& a, const process& b) { return a.getName() < b.getName(); });
        }
    }

    void sortByPID() {
        cout<<"Sort from first smaller pid or bigger pid"<<endl;
        cout<<"1, smaller"<<endl;
        cout<<"2, bigger"<<endl;
        string choice;
        cin>>choice;
        while (choice != "1" && choice != "2") {
            cout<<"Please enter 1 or 2: ";
            cin.clear();
            cin>>choice;
        }
        if (choice == "1") {
            sort(pro.begin(), pro.end(),
                  [](const process& a, const process& b) { return a.getPid() < b.getPid(); });
        }
        else if (choice == "2") {
            sort(pro.rbegin(), pro.rend(),
                  [](const process& a, const process& b) { return a.getPid() < b.getPid(); });
        }
    }

    void sortByMemoryUsage() {
        cout<<"Sort from first smaller memory_usage or bigger memory_usage"<<endl;
        cout<<"1, smaller"<<endl;
        cout<<"2, bigger"<<endl;
        string choice;
        cin>>choice;
        while (choice != "1" && choice != "2") {
            cout<<"Please enter 1 or 2: ";
            cin.clear();
            cin>>choice;
        }
        if (choice == "1") {
            sort(pro.rbegin(), pro.rend(),
                  [](const process& a, const process& b) { return a.getMemoryUsage() > b.getMemoryUsage(); });
        }
        else if (choice == "2") {
            sort(pro.begin(), pro.end(),
                  [](const process& a, const process& b) { return a.getMemoryUsage() > b.getMemoryUsage(); });
        }
    }
    void displayProcesses() const {
        cout << setw(25) << left << "Process Name"
             << setw(30) << "PID"
             << setw(30) << "Memory Usage"
             << setw(30) << "Session Name"
             << "Session ID" << endl;
        cout << string(115, '-') << endl; // Adjusted separator width
        for (const auto& p : pro) {
            p.display();
        }
    }
};
void men() {
    cout<<"Hello User on the demo Task Manager"<<endl;
    cout<<"1. Load Process without sorting"<<endl;
    cout<<"2. Sort by Name"<<endl;
    cout<<"3. Sort by PID"<<endl;
    cout<<"4. Sort by Memory Usage"<<endl;
    cout<<"5. Exit"<<endl;
    cout<<"Enter your choice: ";
}
string menu() {
    men();
    string choice;
    cin>>choice;
    while(choice<"1"||choice>"5") {
        cin.clear();
        cout<<"Invalid choice"<<endl;
        men();
        cin>>choice;
    }
    return choice;
}
int main() {
    processes ps;
    string menu_choice = menu();
    ps.loadProcesses();
    if(menu_choice == "1") {
        ps.displayProcesses();
    }
    else if(menu_choice == "2") {
        ps.sortByName();
        ps.displayProcesses();
    }
    else if(menu_choice == "3") {
        ps.sortByPID();
        ps.displayProcesses();
    }
    else if(menu_choice == "4") {
        ps.sortByMemoryUsage();
        ps.displayProcesses();
    }
    else if(menu_choice == "5") {
        cout<<"Thank you for using this program :)"<<endl;
        exit(0);
    }
    return 0;
}
