#include <iostream>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include "CSVparser.hpp"
#include "RedBlackTree.hpp"
#include "Main.h"
#include "StringToKey.hpp"
#include <SQLAPI.h>

using namespace std;
using std::vector;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */

void displayCourse(Course& course) {
    cout << course.courseId << ": " << course.title  << " | Prerequisites: ";
    for (int i = 0; i < course.prereqs.size(); i++) {
        cout << course.prereqs[i] << " ";
    }
    cout << endl;
    return;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadcourses(string filePath, RedBlackTree* bst) {
    cout << "Loading file " << filePath << endl;

    ifstream file (filePath);
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            istringstream iss(line);
            string item;
            char delim = ',';
            vector<string> vec;
            while (getline(iss, item, delim)) {
                vec.push_back(item);
            }

            Course course;
            course.courseId = vec[0];
            course.title = vec[1];

            for (int i = 2; i < vec.size(); i++) {
                course.prereqs.push_back(vec[i]);
            }

            bst->Insert(course, stringToKey(course.courseId));

        }
        file.close();
    }

    else cout << "Unable to open file";
}



/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    string csvPath, courseKey;
    csvPath = "courses.txt";
    courseKey = "CHEM101";

    // Define a timer variable
    clock_t ticks;

    RedBlackTree* rbt;
    Course* course;
    rbt = new RedBlackTree;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses with In Order Traversal" << endl;
        cout << "  3. Display Root Node" << endl;
        cout << "  4. Display All Courses with Pre Order Traversal" << endl;
        cout << "  5. Search for course id =" << courseKey << endl;
        cout << "  6. Delete course id =" << courseKey << endl;
        cout << "  7. Export courses to database" << endl;
        cout << "  8. Import courses from database" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            // Initialize a timer variable before loading courses
            ticks = clock();

            // Complete the method call to load the courses
            loadcourses(csvPath, rbt);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        case 2:
            rbt->PrintInOrderTraversal();
            break;
        case 3:
            cout << "Root node:" << endl;
            displayCourse(rbt->GetRoot()->course);
            break;
        case 4:
            rbt->PrintPreOrderTraversal();
            break;
        case 5:
            ticks = clock();

            course = rbt->Search(courseKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (course != nullptr) {
                displayCourse(*course);
            }
            else {
                cout << "course Id " << courseKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        case 6:
            rbt->deleteNode(courseKey);
            break;
        case 7:
            rbt->exportToDatabase();
            break;
        case 8:
            rbt->importFromDatabase();
            break;
        case 9:
            //DO NOT USE. exits program
            break;
        }

    }

    cout << "Good bye." << endl;

    return 0;
}
