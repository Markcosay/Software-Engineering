#include<iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <dirent.h>
#include <sys/stat.h> 
#include <unistd.h>


using namespace std;
class StudentTimeTable{
    public : void viewTimetable(string studentID) {
        vector<pair<string, string>> enrolledGroups;
    
        DIR* dir;
        struct dirent* entry;
        if ((dir = opendir("../groups")) != nullptr) {
            while ((entry = readdir(dir)) != nullptr) {
                string filename = entry->d_name;
    
                // Skip "." and ".."
                if (filename == "." || filename == "..") continue;
    
                string filepath = "../groups/" + filename;
                ifstream file(filepath);
                if (!file.is_open()) continue;
    
                string line;
                while (getline(file, line)) {
                    if (line == studentID) {
                        string courseGroup = filename.substr(0, filename.find(".csv"));
                        string courseID = "";
                        string groupID = "";
    
                        // Split courseID and groupID
                        for (char ch : courseGroup) {
                            if (ch=='G') break;
                            courseID += ch;
                        }
                        groupID = courseGroup.substr(courseID.length());
    
                        enrolledGroups.push_back({courseID, groupID});
                        break;
                    }
                }
                file.close();
            }
            closedir(dir);
        } else {
            cerr << " Error opening 'groups' folder." << endl;
            return;
        }
    
        // Now read timetable.csv and print entries
        ifstream tfile("../timetable.csv");
        if (!tfile.is_open()) {
            cerr << "Unable to open timetable.csv" << endl;
            return;
        }
        string line;
        
        for (const auto& entry : enrolledGroups) {
            cout << "Course ID: " << entry.first << " | Group ID: " << entry.second << endl;
        }

        cout << "Timetable for student: " << studentID << "\n\n";
        while (getline(tfile, line)) {
            stringstream ss(line);
            string cid, gid, session, prof, room, time, day;
            
            getline(ss, cid, ',');
            getline(ss, gid, ',');
            getline(ss, session, ',');
            getline(ss, prof, ',');
            getline(ss, room, ',');
            getline(ss, time, ',');
            getline(ss, day, ',');  // ✅ added to match the 7th field

            for (auto& p : enrolledGroups) {
                if (p.first == cid && p.second == gid) {
                    cout << "Course: " << cid << " | Group: " << gid
                        << " | " << session << " | " << prof
                        << " | Room: " << room
                        << " | Time: " << time
                        << " | Day: " << day << endl;
                }
            }
        }

        tfile.close();
    }
    public : void searchTimetable(string studentID, string courseID) {
        string groupID = "";
        bool found = false;
    
        // Step 1 & 2: Check all group files that start with courseID
        DIR* dir;
        struct dirent* entry;
    
        if ((dir = opendir("../groups")) != nullptr) {
            while ((entry = readdir(dir)) != nullptr) {
                string filename = entry->d_name;
                cout<<filename<<endl;
                // Skip "." and ".." and unrelated files
                if (filename == "." || filename == "..") continue;
                if (filename.find(courseID) != 0 || filename.find(".csv") == string::npos) continue;
    
                string filepath = "../groups/" + filename;
                ifstream file(filepath);
                if (!file.is_open()) continue;
    
                string line;
                while (getline(file, line)) {
                    if (line == studentID) {
                        // Extract groupID from filename
                        string courseGroup = filename.substr(0, filename.find(".csv"));
                        groupID = courseGroup.substr(courseID.length());  // e.g., from CSE101GRP2 -> GRP2
                        found = true;
                        break;
                    }
                }
                file.close();
                if (found) break;
            }
            closedir(dir);
        }
        
        if (!found) {
            cout << " Student not enrolled in course " << courseID << endl;
            return;
        }
        
        // Step 4 & 5: Open timetable.csv and print entries
        ifstream tfile("../timetable.csv");
        if (!tfile.is_open()) {
            cerr << "Unable to open timetable.csv" << endl;
            return;
        }
    
        cout << "\n Timetable for student '" << studentID << "' in course " << courseID << " (Group: " << groupID << "):\n\n";
    
        string line;
        bool printed = false;
        while (getline(tfile, line)) {
            stringstream ss(line);
            string cid, gid, session, prof, room, time, day;
    
            getline(ss, cid, ',');
            getline(ss, gid, ',');
            getline(ss, session, ',');
            getline(ss, prof, ',');
            getline(ss, room, ',');
            getline(ss, time, ',');
            getline(ss, day, ',');
    
            if (cid == courseID && gid == groupID) {
                cout << day << " - " << time << " - " << session << " - " << room << " - " << prof << endl;
                printed = true;
            }
        }
    
        if (!printed) {
            cout << "ℹNo timetable entries found for this course/group." << endl;
        }
    
        tfile.close();
    }

public:
    void exportTimetable(string studentID) {
    vector<pair<string, string>> enrolledGroups;

    // Step 1: Read from groups folder
    DIR* dir;
    struct dirent* entry;
    if ((dir = opendir("../groups")) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string filename = entry->d_name;
            if (filename == "." || filename == "..") continue;

            string filepath = "../groups/" + filename;
            ifstream file(filepath);
            if (!file.is_open()) continue;

            string line;
            while (getline(file, line)) {
                if (line == studentID) {
                    string courseGroup = filename.substr(0, filename.find(".csv"));
                    string courseID = "";
                    string groupID = "";

                    // Split courseID and groupID (up to 'G')
                    for (char ch : courseGroup) {
                        if (ch == 'G') break;
                        courseID += ch;
                    }
                    groupID = courseGroup.substr(courseID.length());
                    enrolledGroups.push_back({courseID, groupID});
                    break;
                }
            }
            file.close();
        }
        closedir(dir);
    } else {
        cerr << " Error opening 'groups' folder." << endl;
        return;
    }

    // Step 2: Open timetable.csv
    ifstream tfile("../timetable.csv");
    if (!tfile.is_open()) {
        cerr << "Unable to open timetable.csv" << endl;
        return;
    }

    // Step 3: Ensure 'exports' folder exists
    if (access("exports", F_OK) == -1) {
        mkdir("exports");
    }

    // Step 4: Create output CSV file
    string outputFilePath = "exports/timetable_" + studentID + ".csv";
    ofstream outfile(outputFilePath);
    if (!outfile.is_open()) {
        cerr << " Could not create export file: " << outputFilePath << endl;
        return;
    }

    outfile << "CourseID,GroupID,Session,Professor,Room,Time,Day\n";

    string line;
    while (getline(tfile, line)) {
        stringstream ss(line);
        string cid, gid, session, prof, room, time, day;

        getline(ss, cid, ',');
        getline(ss, gid, ',');
        getline(ss, session, ',');
        getline(ss, prof, ',');
        getline(ss, room, ',');
        getline(ss, time, ',');
        getline(ss, day, ',');

        for (const auto& p : enrolledGroups) {
            if (p.first == cid && p.second == gid) {
                outfile << cid << "," << gid << "," << session << "," << prof << ","
                        << room << "," << time << "," << day << "\n";
            }
        }
    }

    tfile.close();
    outfile.close();

    cout << "Timetable exported successfully to: " << outputFilePath << endl;
}

    
};

int main()
{
    StudentTimeTable s;
   s.viewTimetable("jake");
   //s.searchTimetable("jake","CSE101");
   //s.exportTimetable("jake");
}