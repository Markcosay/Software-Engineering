#include<iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "headerfiles/basicfunctions.h"
using namespace std;


class Courses
{
    bool status =true;
    void registerCourse() {
        string courseID, courseName, lecturerIDs, groupIDs;
        int credits;
    
        cout << "Enter Course ID (e.g., CSE101): ";
        cin >> courseID;
        cin.ignore();
    
        cout << "Enter Course Name: ";
        getline(cin, courseName);
    
        cout << "Enter Number of Credits: ";
        cin >> credits;
        cin.ignore();
    
        cout << "Enter Lecturer IDs (separated by '|', e.g., DR01|DR02): ";
        getline(cin, lecturerIDs);
    
        cout << "Enter Group IDs (separated by '|', e.g., GRP1|GRP2): ";
        getline(cin, groupIDs);
    
        // Check for duplicate Course ID
        ifstream infile("courses.csv");
        string line;
        bool duplicate = false;
    
        while (getline(infile, line)) {
            stringstream ss(line);
            string existingID;
            getline(ss, existingID, ',');
    
            if (existingID == courseID) {
                duplicate = true;
                break;
            }
        }
        infile.close();
    
        if (duplicate) {
            cout << "Course ID already exists. Registration failed." << endl;
            return;
        }
    
        // Write to file
        ofstream outfile("courses.csv", ios::app);
        if (!outfile.is_open()) {
            cout << "Error opening courses.csv for writing!" << endl;
            return;
        }
    
        outfile << courseID << "," << courseName << "," << credits << "," 
                << lecturerIDs << "," << groupIDs << "\n";
        outfile.close();
    
        cout << "Course registered successfully!" << endl;
    }
    void deleteCourse() {
        string courseID;
        cout << "Enter Course ID to delete: ";
        cin >> courseID;
    
        ifstream infile("courses.csv");
        if (!infile.is_open()) {
            cout << "Error opening courses.csv!" << endl;
            return;
        }
    
        ofstream tempFile("temp.csv");
        if (!tempFile.is_open()) {
            cout << "Error creating temporary file!" << endl;
            return;
        }
    
        string line;
        bool found = false;
    
        while (getline(infile, line)) {
            stringstream ss(line);
            string id;
            getline(ss, id, ',');
    
            if (id != courseID) {
                tempFile << line << endl;
            } else {
                found = true;
            }
        }
    
        infile.close();
        tempFile.close();
    
        // Replace original file with updated temp file
        remove("courses.csv");
        rename("temp.csv", "courses.csv");
    
        if (found) {
            cout << "Course deleted successfully." << endl;
        } else {
            cout << "Course ID not found." << endl;
        }
    }
    
    void viewCourses() {
        ifstream infile("courses.csv");
        if (!infile.is_open()) {
            cout << "Error opening courses.csv!" << endl;
            return;
        }
    
        string line;
        cout << "Registered Courses:\n";
        cout << "-----------------------------------------------------------\n";
        cout << "Course ID | Course Name | Credits | Lecturers | Groups\n";
        cout << "-----------------------------------------------------------\n";
    
        while (getline(infile, line)) {
            stringstream ss(line);
            string id, name, credits, lecturers, groups;
    
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, credits, ',');
            getline(ss, lecturers, ',');
            getline(ss, groups, ',');
    
            cout << id << " | " << name << " | " << credits << " | " << lecturers << " | " << groups << endl;
        }
    
        infile.close();
    }
    void assigntimetable()
    {
        string courseID;
        cout << "Enter Course ID: ";
        cin >> courseID;
    
        // Check if timetable already exists
        ifstream infile("timetable.csv");
        string line;
        bool exists = false;
    
        while (getline(infile, line)) {
            stringstream ss(line);
            string fileCourseID;
            getline(ss, fileCourseID, ',');
    
            if (fileCourseID == courseID) {
                exists = true;
                break;
            }
        }
        infile.close();
    
        if (exists) {
            cout << "Timetable already exists for this course." << endl;
            return;
        }
    
        // If not exists, get timetable info for 5 weekdays
        vector<string> weekdays = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
        vector<string> sessions;
    
        for (const auto& day : weekdays) {
            string startTime, endTime, room;
            cout << "Enter Start Time for " << day << " (e.g., 10:00): ";
            cin >> startTime;
            cout << "Enter End Time for " << day << " (e.g., 11:00): ";
            cin >> endTime;
            cout << "Enter Room for " << day << ": ";
            cin >> room;
    
            // Format: CourseID,SessionType,GroupID,LecturerID,Room,Day,StartTime,EndTime,Week
            string session = courseID + ",Lecture,G01,L001," + room + "," + day + "," + startTime + "," + endTime + ",Week1";
            sessions.push_back(session);
        }
    
        // Save to timetable.csv
        ofstream outfile("timetable.csv", ios::app);
        if (!outfile.is_open()) {
            cout << "Error opening timetable file!" << endl;
            return;
        }
    
        for (const auto& s : sessions) {
            outfile << s << endl;
        }
    
        outfile.close();
        cout << "Timetable added successfully for course: " << courseID << endl;
    }
    void registerstudent() {
        status = true;
        string stdid = "", csid = "", grpid = "";
        cout << "Enter student user ID: ";
        cin >> stdid;
        cout << "Enter course ID: ";
        cin >> csid;
        cout << "Enter group ID: ";
        cin >> grpid;

        try {
            // Check if student exists
            BasicFunctions bf;
            if (!bf.isStudentExists(stdid)) {
                throw runtime_error("Student ID does not exist in userdata.csv.");
            }

            // Check if course exists
            if (!bf.isCourseExists(csid)) {
                throw runtime_error("Course ID does not exist in course.csv.");
            }

            // Check if already registered
            ifstream infile("groups.csv");
            string line;
            bool alreadyRegistered = false;

            while (getline(infile, line)) {
                stringstream ss(line);
                string s_id, c_id, g_id;
                getline(ss, s_id, ',');
                getline(ss, c_id, ',');
                getline(ss, g_id, ',');

                if (s_id == stdid && c_id == csid) {
                    alreadyRegistered = true;
                    break;
                }
            }
            infile.close();

            if (alreadyRegistered) {
                cout << " Student " << stdid << " is already registered for course " << csid << endl;
                return;
            }

            // Register in groups.csv
            ofstream outfile("groups.csv", ios::app);
            if (!outfile.is_open()) throw runtime_error("Unable to open groups.csv for writing.");
            outfile << stdid << "," << csid << "," << grpid << endl;
            outfile.close();

            // Add to group file
            string groupFileName = "groups/" + csid + grpid + ".csv";
            ofstream groupfile(groupFileName, ios::app);
            if (!groupfile.is_open()) throw runtime_error("Unable to open group file: " + groupFileName);
            groupfile << stdid << endl;
            groupfile.close();

            cout << stdid << " successfully registered to " << csid << " of group " << grpid << endl;
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
    public : void mainwindow()
    {
        status=true;
        int i=1;
        while(status)
        {
            cout<<"Enter number to perform desired operations"<<"\n";
            cout<<"1. Register Course"<<"\n";
            cout<<"2. Delete Course"<<"\n";
            cout<<"3. View Course"<<"\n";
            cout<<"4. Assign TimeTable"<<"\n";
            cout<<"5. Register student "<<"\n";
            cout<<"6. Previous Window"<<"\n";
            cin>>i;
            switch (i)
            {
                case 1: registerCourse();
                    break;
            
                case 2: deleteCourse();
                    break;
                case 3:viewCourses();
                    break;
                case 4:assigntimetable();
                    break;
                case 5: registerstudent();
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

