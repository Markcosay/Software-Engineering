#include<iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class StudentChanges
{
    bool status=true;
    void registerstudent() {
        string password = "";
        string enteredUsername;
        while(true)
        {
            cout << "Enter Username: ";
            cin >> enteredUsername;
            
            // Check if username already exists
            ifstream file("userdata.csv");
            if (!file.is_open()) {
                cout << "Error opening file!" << endl;
                return;
            }
        
            string line;
            bool usernameExists = false;
        
            while (getline(file, line)) {
                stringstream ss(line);
                string fileUsername;
                getline(ss, fileUsername, ','); // Get username from line
        
                if (fileUsername == enteredUsername) {
                    usernameExists = true;
                    break;
                }
            }
        
            file.close();
        
            if (usernameExists) {
                cout << "Username already taken. Please try again." << endl;
                continue;
                return;
            }
        
            // If username is unique, get password and save
            cout << "Enter Password: ";
            cin >> password;
        
            ofstream outFile("userdata.csv", ios::app); // append mode
            if (!outFile.is_open()) {
                cout << "Error opening file to write!" << endl;
                return;
            }
        
            outFile << enteredUsername << "," << password << endl;
            outFile.close();
        
            cout << "User registered successfully!" << endl;
            cin.get();
            break;
        }
    }
    void deletestudent()
    {
        string enteredUsername;
        cout << "Enter Username to delete: ";
        cin >> enteredUsername;

        ifstream file("userdata.csv");
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }

        vector<string> lines;
        string line;
        bool userFound = false;

        while (getline(file, line)) {
            stringstream ss(line);
            string fileUsername;
            getline(ss, fileUsername, ','); 

            if (fileUsername == enteredUsername) {
                userFound = true;
                continue; 
            }

            lines.push_back(line);
        }

        file.close();

        if (!userFound) {
            cout << "Username not found." << endl;
            return;
        }

        // Rewrite file without the deleted user
        ofstream outFile("userdata.csv");
        if (!outFile.is_open()) {
            cout << "Error writing to file!" << endl;
            return;
        }

        for (const string& l : lines) {
            outFile << l << endl;
        }

        outFile.close();

        cout << "User deleted successfully." << endl;
    }
    void viewstudents()
    {
        cout<<"List of all registered students in University : "<<endl;
        ifstream file("userdata.csv");
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }
    
        string line;
        bool usernameExists = false;
        int i=1;
        while (getline(file, line)) {
            stringstream ss(line);
            string fileUsername;
            getline(ss, fileUsername, ','); // Get username from line
    
            cout<<i<<". "<<fileUsername<<endl;
            i++;
        }
        cin.get();
        cout<<"returning back to menu"<<endl;
        file.close();
    
    }
    public: void mainwindow()
    {
        int i=0;
        while(status)
        {
            cout<<"Enter number to perform desired operations"<<"\n";
            cout<<"1. Register student"<<"\n";
            cout<<"2. Delete Student"<<"\n";
            cout<<"3. View student list"<<"\n";
            cout<<"4. View Student Time table"<<"\n";
            cout<<"5. Register student course"<<"\n";
            cout<<"6. Prevoius Menu"<<"\n";
            cin>>i;
            switch (i)
            {
                case 1: registerstudent();
                    break;
                case 2: deletestudent();
                    break;
                case 3: viewstudents();
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6: return;
                    break;
                default: cout<<"Please enter correct number"<<"\n";
                    break;
            }
            
        }
        cout<<"close ";
        cin.get();
    }
};
 
