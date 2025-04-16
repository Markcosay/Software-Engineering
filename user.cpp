
#include<iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class User
{
    bool verifyUser(const string& username, const string& password) {
        ifstream file("userdata.csv");
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
            return false;
        }
        cout<<"file open";
        string line, fileUsername, filePassword;
    
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, fileUsername, ',');
            getline(ss, filePassword, ',');
    
            if (fileUsername == username && filePassword == password) {
                return true; 
            }
        }
    
        return false; 
    }
    public: void userLogin()
    {
        string username, password;
        cout<<"enter user id"<<endl;
        cin>>username;
        cout<<"enter password"<<endl;
        cin>>password;
        if(verifyUser(username,password))
        {
            cout<<"login ";
        }
        else{
            cout<<"no login";
        }
        cin.get();
        return ;

    }
};
