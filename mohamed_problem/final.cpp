#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <cctype>
#include<limits>
using namespace std;

class process {
private:
    string name;
    int pid;
    string memoryUsage;
    string sessionName;
    string sessionID;
    long long memoryInBytes;
    size_t memoryStringLength;

    static long long convertMemoryToBytes(const string& memoryStr);
public:
    process(string  name, const int pid, const string &memoryUsage, string  sessionName, string sessionID)
        : name(move(name)), pid(pid), memoryUsage(memoryUsage),
          sessionName(move(sessionName)), sessionID(move(sessionID)) {
            memoryInBytes = convertMemoryToBytes(memoryUsage);
            memoryStringLength = memoryUsage.size();
    }

    string getName() const;
    int getPid() const;
    string getMemoryUsage() const;
    long long getMemoryInBytes() const;
    size_t getMemoryStringLength() const;
    string getSessionName() const;
    string getSessionID() const;
    void display() const;

};

class processes {
private:
    vector<process> pro;
    static string clean_memory_from_string(const string& memoryStr);
    static int convertPidToInt(const string& pidStr);

public:
    void loadProcesses();
    void displayProcesses() const;
    static string validation();
    static string validation_string();
    void sortByName();
    void sortByPID();
    void sortByMemoryUsage(bool ascending);
    void userSortByMemoryUsage();
};
class menus {
public:
    static void men();
    static string menu();
    static string sub_menu();
    static void men_main();
};

string process::getName() const {
    return name;
}

int process::getPid() const {
    return pid;
}

string process::getMemoryUsage() const {
    return memoryUsage;
}

long long process::getMemoryInBytes() const {
    return memoryInBytes;
}

size_t process::getMemoryStringLength() const {
    return memoryStringLength;
}

string process::getSessionName() const {
    return sessionName;
}

string process::getSessionID() const {
    return sessionID;
}

void process::display() const {
    // ANSI escape code for blue
    const string BLUE = "\033[34m";
    const string RESET = "\033[0m";

    cout << setfill(' ')
         << BLUE
         << setw(25) << left << name
         << setw(30) << pid
         << setw(30) << memoryUsage
         << setw(30) << sessionName
         << sessionID
         << RESET << endl;
}
long long process::convertMemoryToBytes(const string &memoryStr) {
    long long memory = 0;
    string mem = memoryStr;

    mem.erase(remove(mem.begin(), mem.end(), '"'), mem.end());
    mem.erase(remove(mem.begin(), mem.end(), ' '), mem.end());
    mem.erase(remove(mem.begin(), mem.end(), ','), mem.end());

    if (mem.empty() || mem == "N/A") {
        cerr << "Warning: Memory string is invalid or unavailable: '" << memoryStr << "'" << endl;
        return 0;
    }

    try {
        if (mem.find("KB") != string::npos) {
            mem.erase(mem.find("KB"), 2);
            memory = stoll(mem) * 1024;
        } else if (mem.find("MB") != string::npos) {
            mem.erase(mem.find("MB"), 2);
            memory = stoll(mem) * 1024 * 1024;
        } else if (mem.find("GB") != string::npos) {
            mem.erase(mem.find("GB"), 2);
            memory = stoll(mem) * 1024 * 1024 * 1024;
        } else {
            memory = stoll(mem);
        }
    } catch (const invalid_argument& e) {
        cerr << "Error: Invalid number format in memory string: " << memoryStr << endl;
        memory = 0;
    } catch (const out_of_range& e) {
        cerr << "Error: Number out of range in memory string: " << memoryStr << endl;
        memory = 0;
    }

    return memory;
}


string processes::clean_memory_from_string(const string &memoryStr) {
    string mem = memoryStr;
    mem.erase(remove(mem.begin(), mem.end(), '"'), mem.end());
    mem.erase(remove(mem.begin(), mem.end(), ' '), mem.end());
    if (mem.find("KB") != string::npos) {
        mem.erase(mem.find("KB"), 2);
    }
    return mem;
}

int processes::convertPidToInt(const string &pidStr) {
    try {
         if (pidStr.empty() || !all_of(pidStr.begin(), pidStr.end(), ::isdigit)) {
            throw invalid_argument("Non-numeric or empty PID: " + pidStr);
        }
        return stoi(pidStr);
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    } catch (const out_of_range& e) {
        cerr << "Error: PID out of range: " << pidStr << endl;
        return -1;
    }
}


void processes::loadProcesses() {
    pro.clear();
    system("tasklist /FO CSV > task_manager.csv");
    ifstream ifs("task_manager.csv");
    string line;
    bool skipHeader = true;

    while (getline(ifs, line)) {
        if (skipHeader) {
            skipHeader = false;
            continue;
        }
        stringstream ss(line);
        string name, pid_str, session_name, session_id, memoryStr;
        getline(ss, name, ',');
        getline(ss, pid_str, ',');
        getline(ss, session_name, ',');
        getline(ss, session_id, ',');
        getline(ss, memoryStr);

        transform(name.begin(), name.end(), name.begin(), ::tolower);
        name.erase(remove(name.begin(), name.end(), '"'), name.end());
        pid_str.erase(remove(pid_str.begin(), pid_str.end(), '"'), pid_str.end());
        pid_str.erase(remove(pid_str.begin(), pid_str.end(), ' '), pid_str.end());

        int pid = convertPidToInt(pid_str);
        if (pid == -1) continue;

        memoryStr = clean_memory_from_string(memoryStr);
        pro.emplace_back(name, pid, memoryStr, session_name, session_id);
    }
    ifs.close();
}

void processes::displayProcesses() const {
    cout << setw(25) << left << "Process Name"
            << setw(30) << "PID"
            << setw(30) << "Memory Usage"
            << setw(30) << "Session Name"
            << "Session ID" << endl;
    cout << string(135, '-') << endl;
    for (const auto& p : pro) {
        p.display();
    }
}

string processes::validation() {
    cout << "1, smaller" << endl;
    cout << "2, bigger" << endl;
    string choice;
    cin >> choice;
    while (choice != "1" && choice != "2") {
        cout << "Please enter 1 or 2: ";
        cin.clear();
        cin >> choice;
    }
    return choice;
}

string processes::validation_string() {
    cout << "\033[32m";
    cout << "1, smaller (a->z)" << endl;
    cout << "2, bigger (z->a)" << endl;
    string choice;
    cin >> choice;
    while (choice != "1" && choice != "2") {
        cout << "Please enter 1 or 2: ";
        cin.clear();
        cin >> choice;
    }
    cout << "\033[0m";
    return choice;
}

void processes::sortByName() {
    cout << "\033[32m";
    cout << "Sort from first smaller character or bigger character" << endl;
    string choice = validation_string();
    if (choice == "1") {
        sort(pro.begin(), pro.end(),
             [](const process& a, const process& b) { return a.getName() < b.getName(); });
    }
    else if (choice == "2") {
        sort(pro.rbegin(), pro.rend(),
             [](const process& a, const process& b) { return a.getName() < b.getName(); });
    }
    cout << "\033[0m";
}

void processes::sortByPID() {
    cout << "\033[33m";
    cout << "Sort from first smaller pid or bigger pid" << endl;
    string choice = validation();
    if (choice == "1") {
        sort(pro.begin(), pro.end(),
             [](const process& a, const process& b) { return a.getPid() < b.getPid(); });
    }
    else if (choice == "2") {
        sort(pro.rbegin(), pro.rend(),
             [](const process& a, const process& b) { return a.getPid() < b.getPid(); });
    }
    cout << "\033[0m";
}

void processes::sortByMemoryUsage(bool ascending) {
    cout << "Sorting by memory usage." << endl;

    sort(pro.begin(), pro.end(),
         [ascending](const process& a, const process& b) {
             if (a.getMemoryInBytes() == b.getMemoryInBytes()) {
                 return ascending ? a.getMemoryStringLength() < b.getMemoryStringLength()
                            : a.getMemoryStringLength() > b.getMemoryStringLength();
             }
             return ascending ? a.getMemoryInBytes() < b.getMemoryInBytes()
                        : a.getMemoryInBytes() > b.getMemoryInBytes();
         });
}

void processes::userSortByMemoryUsage() {
    cout << "\033[96m";
    cout << "Sort from smaller memory usage or bigger memory usage (1 for smaller, 2 for bigger)" << endl;

    string choice = validation();

    if (choice == "1") {
        sortByMemoryUsage(true);  //from smaller to bigger
    } else if (choice == "2") {
        sortByMemoryUsage(false);  //from bigger to smaller
    }
    cout << "\033[0m";
}

void menus::men(){
    cout<<char(2)<<" ";
    cout << "Hello User on the demo Task Manager "<<char(2) << endl;
    cout << "1. Load Process without sorting" << endl;
    cout << "2. Sort by Name" << endl;
    cout << "3. Sort by PID" << endl;
    cout << "4. Sort by Memory Usage" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

string menus::menu() {
    menus::men();
    string choice;
    cin >> choice;

    while (choice.length() != 1 || !isdigit(choice[0]) || choice < "1" || choice > "5") {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        cout << "Invalid choice. Please enter a number between 1 and 5." << endl;
        menus::men(); // Re-display the menu
        cin >> choice;
    }
    return choice;
}

string menus::sub_menu() {
    cout << "\033[95m";
    cout<<"Do you want to continue"<<endl;
    cout<<"1. Yes"<<endl;
    cout<<"2. No"<<endl;
    string choice;
    cin>>choice;
    while (choice != "1" && choice != "2") {
        cin.clear();
        cout << "Invalid choice" << endl;
        cout<<"1. Yes"<<endl;
        cout<<"2. No"<<endl;
        cin>>choice;
    }
    cout << "\033[0m";
    return choice;
}
void menus::men_main() {
    while (true) {
        processes ps;
        string menu_choice = menus::menu();
        ps.loadProcesses();
        if (menu_choice == "1") {
            ps.displayProcesses();
        } else if (menu_choice == "2") {
            ps.sortByName();
            ps.displayProcesses();
        } else if (menu_choice == "3") {
            ps.sortByPID();
            ps.displayProcesses();
        } else if (menu_choice == "4") {
            ps.userSortByMemoryUsage();
            ps.displayProcesses();
        } else if (menu_choice == "5") {
            cout << "\033[32m"; // ANSI code for green text
            cout << "Thank you for using this program " << char(1) << " " << char(3) << endl;
            // Reset color to default
            cout << "\033[0m";
            exit(0);
        }
        string x = menus::sub_menu();
        if (x == "1") {
            continue;
        } else if (x == "2") {
            cout << "\033[32m"; // ANSI code for green text
            cout << "Thank you for using this program " << char(1) << " " << char(3) << endl;
            // Reset color to default
            cout << "\033[0m";
            exit(0);
        }
    }
}

int main() {
    menus::men_main();
    return 0;
}
