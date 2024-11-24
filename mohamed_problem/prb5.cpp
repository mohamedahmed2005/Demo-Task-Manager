#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <cctype>

using namespace std;

class process {
private:
    string name;
    int pid;
    string memoryUsage;
    string sessionName;
    string sessionID;
    long long memoryInBytes; // Memory size in bytes
    size_t memoryStringLength; // Length of the memory string

public:
    process(const string& name, int pid, string memoryUsage, const string& sessionName, string sessionID)
        : name(name), pid(pid), memoryUsage(memoryUsage), sessionName(sessionName), sessionID(sessionID) {
            memoryInBytes = convertMemoryToBytes(memoryUsage);
            memoryStringLength = memoryUsage.size();
    }

    string getName() const {
        return name;
    }
    int getPid() const {
        return pid;
    }
    string getMemoryUsage() const {
        return memoryUsage;
    }
    long long getMemoryInBytes() const {
        return memoryInBytes; // Return the numeric value of memory usage in bytes
    }
    size_t getMemoryStringLength() const {
        return memoryStringLength; // Return the length of the memory usage string
    }
    string getSessionName() const {
        return sessionName;
    }
    string getSessionID() const {
        return sessionID;
    }
    void display() const {
        cout << setfill(' ')
             << setw(25) << left << name
             << setw(30) << pid
             << setw(30) << memoryUsage
             << setw(30) << sessionName
             << sessionID << endl;
    }

private:
    long long convertMemoryToBytes(const std::string& memoryStr) {
        long long memory = 0;
        std::string mem = memoryStr;

        // Remove quotes, spaces, and commas
        mem.erase(remove(mem.begin(), mem.end(), '"'), mem.end());
        mem.erase(remove(mem.begin(), mem.end(), ' '), mem.end());
        mem.erase(remove(mem.begin(), mem.end(), ','), mem.end());

        if (mem.empty()) {
            throw std::invalid_argument("Input string is empty or invalid");
        }

        // Find unit (KB, MB, GB, etc.)
        if (mem.find("KB") != std::string::npos) {
            mem.erase(mem.find("KB"), 2);  // Remove "KB"
            memory = std::stoll(mem) * 1024;  // Convert KB to bytes
        }
        else if (mem.find("MB") != std::string::npos) {
            mem.erase(mem.find("MB"), 2);  // Remove "MB"
            memory = std::stoll(mem) * 1024 * 1024;  // Convert MB to bytes
        }
        else if (mem.find("GB") != std::string::npos) {
            mem.erase(mem.find("GB"), 2);  // Remove "GB"
            memory = std::stoll(mem) * 1024 * 1024 * 1024;  // Convert GB to bytes
        }
        else {
            // If no unit is found, treat the input as bytes
            try {
                memory = std::stoll(mem);  // Convert string directly to bytes
            } catch (const std::invalid_argument& e) {
                throw std::invalid_argument("Invalid number format: " + mem);
            } catch (const std::out_of_range& e) {
                throw std::out_of_range("Number out of range: " + mem);
            }
        }

        return memory;
    }
};

class processes {
private:
    vector<process> pro;
    static string clean_memory_from_string(const string& memoryStr) {
        string mem = memoryStr;
        mem.erase(remove(mem.begin(), mem.end(), '"'), mem.end()); // Remove quotes
        mem.erase(remove(mem.begin(), mem.end(), ' '), mem.end()); // Remove spaces
        if (mem.find("KB") != string::npos) {
            mem.erase(mem.find("KB"), 2); // Remove "KB"
        }
        return mem;
    }

    // Function to safely convert PID string to integer with error handling
    static int convertPidToInt(const string& pidStr) {
        try {
            size_t pos;
            int pid = stoi(pidStr, &pos);  // Convert to integer and check for invalid characters
            if (pos != pidStr.length()) {
                throw invalid_argument("Non-numeric characters in PID");
            }
            return pid;
        } catch (const invalid_argument& e) {
            cerr << "Error: Invalid PID format: " << pidStr << endl;
            return -1; // Return an invalid PID if conversion fails
        } catch (const out_of_range& e) {
            cerr << "Error: PID out of range: " << pidStr << endl;
            return -1; // Return an invalid PID if conversion exceeds range
        }
    }

public:
    void loadProcesses() {
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

            // Remove extra commas or quotes that are present in the CSV fields
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            name.erase(remove(name.begin(), name.end(), '"'), name.end());
            pid_str.erase(remove(pid_str.begin(), pid_str.end(), '"'), pid_str.end());
            pid_str.erase(remove(pid_str.begin(), pid_str.end(), ' '), pid_str.end());

            // Convert PID string to integer
            int pid = convertPidToInt(pid_str);
            if (pid == -1) continue; // Skip invalid PIDs

            memoryStr = clean_memory_from_string(memoryStr);
            pro.emplace_back(name, pid, memoryStr, session_name, session_id);
        }
        ifs.close();
    }
    void displayProcesses() const {
        cout << setw(25) << left << "Process Name"
             << setw(30) << "PID"
             << setw(30) << "Memory Usage"
             << setw(30) << "Session Name"
             << "Session ID" << endl;
        cout << string(135, '-') << endl; // Adjusted separator width
        for (const auto& p : pro) {
            p.display();
        }
    }

    string validation() {
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
    string validation_string() {
        cout << "1, smaller (a->z)" << endl;
        cout << "2, bigger (z->a)" << endl;
        string choice;
        cin >> choice;
        while (choice != "1" && choice != "2") {
            cout << "Please enter 1 or 2: ";
            cin.clear();
            cin >> choice;
        }
        return choice;
    }
    void sortByName() {
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
    }

    void sortByPID() {
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
    }
    void sortByMemoryUsage(bool ascending) {
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

    void userSortByMemoryUsage() {
        cout << "Sort from smaller memory usage or bigger memory usage (1 for smaller, 2 for bigger)" << endl;
        string choice = validation();

        // Call sortByMemoryUsage with appropriate flag for sorting order
        if (choice == "1") {
            sortByMemoryUsage(true);  // Ascending order
        } else if (choice == "2") {
            sortByMemoryUsage(false);  // Descending order
        }
    }


};

void men() {
    cout << "Hello User on the demo Task Manager" << endl;
    cout << "1. Load Process without sorting" << endl;
    cout << "2. Sort by Name" << endl;
    cout << "3. Sort by PID" << endl;
    cout << "4. Sort by Memory Usage" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

string menu() {
    men();
    string choice;
    cin >> choice;
    while (choice < "1" || choice > "5" || choice.length() != 1 || !isdigit(choice[0])) {
        cin.clear(); // Clear the error flag
        cout << "Invalid choice. Please enter a number between 1 and 5." << endl;
        men();
        cin >> choice;
    }
    return choice;
}
string sub_menu() {
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
    return choice;
}
void men_main() {
    while(true) {
        processes ps;
        string menu_choice = menu();
        ps.loadProcesses();
        if (menu_choice == "1") {
            ps.displayProcesses();
        }
        else if (menu_choice == "2") {
            ps.sortByName();
            ps.displayProcesses();
        }
        else if (menu_choice == "3") {
            ps.sortByPID();
            ps.displayProcesses();
        }
        else if (menu_choice == "4") {
            ps.userSortByMemoryUsage();
            ps.displayProcesses();
        }
        else if (menu_choice == "5") {
            cout << "Thank you for using this program :)" << endl;
            exit(0);
        }
        string x = sub_menu();
        if(x=="1") {
            continue;
        }
        else if (x=="2") {
            cout << "Thank you for using this program :)" << endl;
            exit(0);
        }
    }
}
int main() {
    men_main();
    return 0;
}
