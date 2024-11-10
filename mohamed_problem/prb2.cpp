#include <iostream>
#include <sstream>
#include <unordered_set>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <cmath>
using namespace std;
void implementation();
class StringSet {
private:
    unordered_set<string> word_set;
    string toLowercase(const string& s) {
        string res;
        for (char ch : s) {
            res += tolower(ch);
        }
        return res;
    }
public:
    StringSet() {};
    StringSet(const string& file_name) {
        ifstream ProgramFile(file_name);
        if (!ProgramFile.is_open()) {
            cerr << "Error: File does not exist or cannot be opened: " << file_name << endl;
            // Create the file since it does not exist
            ofstream createFile(file_name);
            if (createFile.is_open()) {
                cout << "File created: " << file_name << endl;
                createFile.close();
            }
            else {
                cerr << "Error: Could not create file: " << file_name << endl;
                return;
            }
        }
        string line;
        while (getline(ProgramFile, line)) {
            stringstream ss(line);
            string word;
            while (ss >> word) {
                word.erase(remove_if(word.begin(), word.end(), [](char ch) {
                    return ispunct(ch) || isdigit(ch);
                }), word.end());

                // Check if the cleaned word is alphabetic before inserting
                if (!word.empty() && all_of(word.begin(), word.end(), ::isalpha)) {
                    word_set.insert(toLowercase(word));
                }
            }
        }
        ProgramFile.close();
    }
    StringSet(const string& input,bool) {
        istringstream iss(input);
        string word;
        while (iss >> word) {
            word.erase(remove_if(word.begin(), word.end(), [](char ch) {
                return ispunct(ch) || isdigit(ch);
            }), word.end());
            if (!word.empty() && all_of(word.begin(), word.end(), ::isalpha)) {
                word_set.insert(toLowercase(word));
            }
        }
    }
    void add_wordset(const string& word) {
        string cleaned_word = word;
        cleaned_word.erase(remove_if(cleaned_word.begin(), cleaned_word.end(), [](char ch) {
            return ispunct(ch) || isdigit(ch);
        }), cleaned_word.end());

        // Split the cleaned word into tokens and insert them into the set
        istringstream iss(cleaned_word);
        string token;
        while (iss >> token) {
            if (!token.empty() && all_of(token.begin(), token.end(), ::isalpha)) {
                word_set.insert(toLowercase(token));
            }
        }
    }
    void remove_wordset(const string& word) {
        word_set.erase(toLowercase(word));
    }
    void clear_wordset() {
        word_set.clear();
    }
    int size_wordset(){
        return word_set.size();
    }

    void display() {
        cout << "[ ";
        for (auto it = word_set.begin(); it != word_set.end(); ++it) {
            cout << *it;
            if (next(it) != word_set.end()) {
                cout << " , ";
            }
        }
        cout << " ]" << endl;
        cout << "Size of set: " << word_set.size() << endl;
    }
    StringSet operator+(StringSet& s) {
        StringSet result;
        result.word_set = word_set; // Start with the current set
        result.word_set.insert(s.word_set.begin(), s.word_set.end()); // Add all elements from s
        return result;
    }
    StringSet operator*(const StringSet& other) const {
        StringSet result;
        for (const auto& word : word_set) {
            if (find(other.word_set.begin(), other.word_set.end(), word) != other.word_set.end()) {
                result.word_set.insert(word);
            }
        }
        return result;
    }

    friend double similarity(StringSet& s1, StringSet& s2);
    friend string menu();
    friend void implementation();
};
double similarity(StringSet& s1, StringSet& s2) {
    StringSet result = s1 * s2; // Get the intersection
    cout << "Result: ";
    result.display();
    cout << endl;
    double sim = 0;
    double sz_s1 = s1.size_wordset();
    double sz_s2 = s2.size_wordset();
    double sz_res = result.size_wordset();
    if (sz_s1 > 0 && sz_s2 > 0) { // Prevent division by zero
        sim = sz_res / (sqrt(sz_s1) * sqrt(sz_s2));
    }
    return sim;
}
string menu() {
    bool flag = true;
    while(flag) {
        string choice;
        cout << "Menu" << endl;
        cout << "1. Output content on file" << endl;
        cout << "2. Add word_set" << endl;
        cout << "3. Remove word_set" << endl;
        cout << "4. Clear word_set" << endl;
        cout << "5. Size word_set" << endl;
        cout << "6. Union two word_sets" << endl;
        cout << "7. Intersection two word_sets" << endl;
        cout << "8. Similarity two word_sets" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        try {
            int x = stoi(choice);
            if(x < 1 || x > 9) {
                cout << "Please enter a valid choice." << endl;
            }
            else {
                flag = false;
                return choice;
            }
        }
        catch (invalid_argument& e) {
            cout << "Please enter a valid choice (numbers only)." << endl;
        }
        catch (out_of_range& e) {
            cout << "Please enter a valid choice within the range." << endl;
        }
    }
}
string menu_2() {
    cout<<"Do you want to continue"<<endl;
    cout<<"1. Continue"<<endl;
    cout<<"2. Exit"<<endl;
    cout<<"Enter your choice : ";
    string choice;
    cin>>choice;
    while(choice!="1"&&choice!="2") {
        cout<<"Enter valid choice"<<endl;
        cout<<"Do you want to continue"<<endl;
        cout<<"1. Continue"<<endl;
        cout<<"2. Exit"<<endl;
        cout<<"Enter your choice : ";
        cin>>choice;
    }
    if(choice=="1") {
        implementation();
    }
    else if(choice=="2") {
        cout<<"Thank You for Using this program"<<endl;
        exit(0);
    }
}
void implementation() {
    StringSet s1, s2;
    while(true) {
        string inp = menu();
        if (inp == "1") {
            string filename;
            cout<<"Enter file name: ";
            cin>>filename;
            s1 = StringSet(filename);
            s1.display();
        }
        else if (inp == "2") {
            string filename;
            cout<<"Enter file name: ";
            cin>>filename;
            s1 = StringSet(filename);
            string word;
            cout << "Enter word to add: ";
            cin>>word;
            s1.add_wordset(word);
            s1.display();
        }
        else if (inp == "3") {
            string filename;
            cout<<"Enter file name: ";
            cin>>filename;
            s1 = StringSet(filename);
            string word;
            cout << "Enter word to remove: ";
            cin >> word;
            s1.remove_wordset(word);
        }
        else if (inp == "4") {
            string filename;
            cout<<"Enter file name: ";
            cin>>filename;
            s1 = StringSet(filename);
            cout<<"Clear word_set"<<endl;
            s1.clear_wordset();
            s1.display();
        }
        else if (inp == "5") {
            string filename;
            cout<<"Enter file name: ";
            cin>>filename;
            s1 = StringSet(filename);
            cout << "Size of word set: " << s1.size_wordset() << endl;
        }
        else if (inp == "6") {
            string filename;
            cout<<"Enter first file name: ";
            cin>>filename;
            s1 = StringSet(filename);
            string filename_2;
            cout << "Enter second name of file: ";
            cin >> filename_2;
            s2 = StringSet(filename_2);
            StringSet res = s1 + s2;
            res.display();
        }
        else if (inp == "7") {
            string filename;
            cout<<"Enter first file name: ";
            cin>>filename;
            s1 = StringSet(filename);
            string filename_2;
            cout << "Enter second name of file: ";
            cin >> filename_2;
            s2 = StringSet(filename_2);
            StringSet res = s1 * s2;
            res.display();
        }
        else if(inp=="8") {
            string filename;
            cout<<"Enter first file name: ";
            cin>>filename;
            s1 = StringSet(filename);
            string filename_2;
            cout << "Enter second name of file: ";
            cin >> filename_2;
            s2 = StringSet(filename_2);
            cout<<"s1: ";
            s1.display();
            cout<<"s2: ";
            s2.display();
            cout<<"------------------"<<endl;
            double sim = similarity(s1,s2);
            cout<<"Similarity = "<<sim<<endl;
        }
        else if(inp=="9") {
            cout<<"Exit"<<endl;
            exit(0);
        }
        menu_2();
    }
}
int main() {
    implementation();
    return 0;
}