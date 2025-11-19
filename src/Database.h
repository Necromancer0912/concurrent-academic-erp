#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Student.h"

class Database
{
private:
    static const std::string STUDENT_DB_FILE;
    static const std::string INSERTION_ORDER_FILE;
    static const std::string STUDENT_DB_BACKUP_1;
    static const std::string STUDENT_DB_BACKUP_2;
    static const int MAX_BACKUPS = 2;

    // Create backup of current database
    static void createBackup();

    // Rotate backups (backup_1 -> backup_2, current -> backup_1)
    static void rotateBackups();

    // Helper to convert StudentLevel enum to string
    static std::string levelToString(StudentLevel level);

    // Helper to convert string to StudentLevel enum
    static StudentLevel stringToLevel(const std::string &str);

public:
    // Save all student records to database
    template <typename RollType, typename CourseType>
    static void saveStudents(const std::vector<Student<RollType, CourseType>> &students);

    // Save insertion order (only roll numbers)
    template <typename RollType>
    static void saveInsertionOrder(const std::vector<RollType> &rollNumbers);

    // Load insertion order (only roll numbers)
    template <typename RollType>
    static std::vector<RollType> loadInsertionOrder();

    // Load all student records from database
    template <typename RollType, typename CourseType>
    static std::vector<Student<RollType, CourseType>> loadStudents();

    // Load a range of student records from database (for parallel processing)
    template <typename RollType, typename CourseType>
    static std::vector<Student<RollType, CourseType>> loadStudentsRange(int start, int count);

    // Count total students in database
    static int countStudents();

    // Restore from backup
    static bool restoreFromBackup(int backupNumber);

    // Get backup information
    static std::vector<std::string> getBackupInfo();

    // Initialize database files
    static void initializeDatabaseFiles();

    // Export database to CSV
    static void exportToCSV(const std::string &filename);

    // Clear insertion order file
    static void clearInsertionOrder();
};

// Insertion order template implementations
template <typename RollType>
void Database::saveInsertionOrder(const std::vector<RollType> &rollNumbers)
{
    std::ofstream file(INSERTION_ORDER_FILE, std::ios::trunc);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open insertion order file for writing");
    }

    file << "# Insertion Order File\n";
    file << "# Maintains the order in which students were added\n";
    file << "# Total: " << rollNumbers.size() << "\n";
    file << "---\n";

    for (const auto &roll : rollNumbers)
    {
        file << roll << "\n";
    }

    file.close();
}

template <typename RollType>
std::vector<RollType> Database::loadInsertionOrder()
{
    std::vector<RollType> rollNumbers;
    std::ifstream file(INSERTION_ORDER_FILE);

    if (!file.is_open())
    {
        return rollNumbers; // Return empty if file doesn't exist
    }

    std::string line;
    while (std::getline(file, line))
    {
        // Skip comments and header lines
        if (line.empty() || line[0] == '#' || line == "---")
        {
            continue;
        }

        RollType rollNumber;
        std::istringstream iss(line);
        iss >> rollNumber;
        rollNumbers.push_back(rollNumber);
    }

    file.close();
    return rollNumbers;
}

// Template implementations
template <typename RollType, typename CourseType>
void Database::saveStudents(const std::vector<Student<RollType, CourseType>> &students)
{
    // Create backup before saving
    createBackup();

    std::ofstream file(STUDENT_DB_FILE, std::ios::trunc);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open database file for writing");
    }

    file << "# Student Database File\n";
    file << "# Version: 2.0\n";
    file << "# Total Students: " << students.size() << "\n";
    file << "---\n";

    for (const auto &student : students)
    {
        file << "STUDENT_START\n";
        file << "RollNumber: " << student.get_roll_number() << "\n";
        file << "Name: " << student.get_name() << "\n";
        file << "BranchCode: " << student.get_branch().get_branch_code() << "\n";
        file << "BranchName: " << student.get_branch().get_branch_name() << "\n";
        file << "Department: " << student.get_branch().get_department() << "\n";
        file << "Level: " << levelToString(student.get_level()) << "\n";
        file << "StartYear: " << student.get_starting_year() << "\n";

        // Save current courses
        auto currentCourses = student.get_current_courses();
        auto currentGrades = student.get_current_grades();
        file << "CurrentCourses: " << currentCourses.size() << "\n";

        for (size_t i = 0; i < currentCourses.size(); ++i)
        {
            file << "COURSE_START\n";
            file << "CourseCode: " << currentCourses[i].get_course_code() << "\n";
            file << "CourseName: " << currentCourses[i].get_course_name() << "\n";
            file << "Instructor: " << currentCourses[i].get_instructor() << "\n";
            file << "Credits: " << currentCourses[i].get_credits() << "\n";
            file << "Semester: " << currentCourses[i].get_semester() << "\n";

            if (i < currentGrades.size())
            {
                auto &grade = currentGrades[i];
                file << "MidSem: " << grade.get_mid_sem_marks() << "\n";
                file << "EndSem: " << grade.get_end_sem_marks() << "\n";
                file << "Assignment: " << grade.get_assignment_marks() << "\n";
                file << "GradePoint: " << grade.get_grade_point() << "\n";
                file << "LetterGrade: " << grade.get_letter_grade() << "\n";
            }
            else
            {
                file << "MidSem: -1\n";
                file << "EndSem: -1\n";
                file << "Assignment: -1\n";
                file << "GradePoint: 0\n";
                file << "LetterGrade: N/A\n";
            }

            file << "COURSE_END\n";
        }

        // Save previous courses
        auto previousCourses = student.get_previous_courses();
        auto previousGrades = student.get_previous_grades();
        file << "PreviousCourses: " << previousCourses.size() << "\n";

        for (size_t i = 0; i < previousCourses.size(); ++i)
        {
            file << "COURSE_START\n";
            file << "CourseCode: " << previousCourses[i].get_course_code() << "\n";
            file << "CourseName: " << previousCourses[i].get_course_name() << "\n";
            file << "Instructor: " << previousCourses[i].get_instructor() << "\n";
            file << "Credits: " << previousCourses[i].get_credits() << "\n";
            file << "Semester: " << previousCourses[i].get_semester() << "\n";

            if (i < previousGrades.size())
            {
                auto &grade = previousGrades[i];
                file << "MidSem: " << grade.get_mid_sem_marks() << "\n";
                file << "EndSem: " << grade.get_end_sem_marks() << "\n";
                file << "Assignment: " << grade.get_assignment_marks() << "\n";
                file << "GradePoint: " << grade.get_grade_point() << "\n";
                file << "LetterGrade: " << grade.get_letter_grade() << "\n";
            }

            file << "COURSE_END\n";
        }

        file << "STUDENT_END\n";
    }

    file.close();
}

template <typename RollType, typename CourseType>
std::vector<Student<RollType, CourseType>> Database::loadStudents()
{
    std::vector<Student<RollType, CourseType>> students;
    std::ifstream file(STUDENT_DB_FILE);

    if (!file.is_open())
    {
        return students; // Return empty vector if file doesn't exist
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line == "STUDENT_START")
        {
            RollType rollNumber;
            std::string name, branchCode, branchName, department, levelStr;
            int startYear;

            // Read student basic info
            while (std::getline(file, line) && line != "STUDENT_END")
            {
                if (line.find("RollNumber: ") == 0)
                {
                    std::istringstream iss(line.substr(12));
                    iss >> rollNumber;
                }
                else if (line.find("Name: ") == 0)
                {
                    name = line.substr(6);
                }
                else if (line.find("BranchCode: ") == 0)
                {
                    branchCode = line.substr(12);
                }
                else if (line.find("BranchName: ") == 0)
                {
                    branchName = line.substr(12);
                }
                else if (line.find("Department: ") == 0)
                {
                    department = line.substr(12);
                }
                else if (line.find("Level: ") == 0)
                {
                    levelStr = line.substr(7);
                }
                else if (line.find("StartYear: ") == 0)
                {
                    startYear = std::stoi(line.substr(11));
                }
                else if (line.find("CurrentCourses: ") == 0)
                {
                    int numCourses = std::stoi(line.substr(16));

                    Branch branch(branchCode, branchName, department);
                    StudentLevel level = stringToLevel(levelStr);
                    Student<RollType, CourseType> student(name, rollNumber, branch, level, startYear);

                    // Read current courses
                    for (int i = 0; i < numCourses; ++i)
                    {
                        std::getline(file, line); // COURSE_START

                        CourseType courseCode;
                        std::string courseName, instructor, semester;
                        int credits;
                        double midSem = -1, endSem = -1, assignment = -1;

                        while (std::getline(file, line) && line != "COURSE_END")
                        {
                            if (line.find("CourseCode: ") == 0)
                            {
                                std::istringstream iss(line.substr(12));
                                iss >> courseCode;
                            }
                            else if (line.find("CourseName: ") == 0)
                            {
                                courseName = line.substr(12);
                            }
                            else if (line.find("Instructor: ") == 0)
                            {
                                instructor = line.substr(12);
                            }
                            else if (line.find("Credits: ") == 0)
                            {
                                credits = std::stoi(line.substr(9));
                            }
                            else if (line.find("Semester: ") == 0)
                            {
                                semester = line.substr(10);
                            }
                            else if (line.find("MidSem: ") == 0)
                            {
                                midSem = std::stod(line.substr(8));
                            }
                            else if (line.find("EndSem: ") == 0)
                            {
                                endSem = std::stod(line.substr(8));
                            }
                            else if (line.find("Assignment: ") == 0)
                            {
                                assignment = std::stod(line.substr(12));
                            }
                        }

                        Course<CourseType> course(courseCode, courseName, instructor, credits, semester);

                        if (midSem >= 0 && endSem >= 0 && assignment >= 0)
                        {
                            Grade grade(midSem, endSem, assignment);
                            student.add_current_course(course, grade);
                        }
                        else
                        {
                            student.add_current_course(course);
                        }
                    }

                    // Read previous courses
                    std::getline(file, line); // PreviousCourses: N
                    if (line.find("PreviousCourses: ") == 0)
                    {
                        int numPrevCourses = std::stoi(line.substr(17));

                        for (int i = 0; i < numPrevCourses; ++i)
                        {
                            std::getline(file, line); // COURSE_START

                            CourseType courseCode;
                            std::string courseName, instructor, semester;
                            int credits;
                            double midSem = -1, endSem = -1, assignment = -1;

                            while (std::getline(file, line) && line != "COURSE_END")
                            {
                                if (line.find("CourseCode: ") == 0)
                                {
                                    std::istringstream iss(line.substr(12));
                                    iss >> courseCode;
                                }
                                else if (line.find("CourseName: ") == 0)
                                {
                                    courseName = line.substr(12);
                                }
                                else if (line.find("Instructor: ") == 0)
                                {
                                    instructor = line.substr(12);
                                }
                                else if (line.find("Credits: ") == 0)
                                {
                                    credits = std::stoi(line.substr(9));
                                }
                                else if (line.find("Semester: ") == 0)
                                {
                                    semester = line.substr(10);
                                }
                                else if (line.find("MidSem: ") == 0)
                                {
                                    midSem = std::stod(line.substr(8));
                                }
                                else if (line.find("EndSem: ") == 0)
                                {
                                    endSem = std::stod(line.substr(8));
                                }
                                else if (line.find("Assignment: ") == 0)
                                {
                                    assignment = std::stod(line.substr(12));
                                }
                            }

                            // For previous courses, we might want to move them
                            Course<CourseType> course(courseCode, courseName, instructor, credits, semester);
                            if (midSem >= 0 && endSem >= 0 && assignment >= 0)
                            {
                                Grade grade(midSem, endSem, assignment);
                                student.add_current_course(course, grade);
                                student.move_to_previous_courses(courseCode);
                            }
                        }
                    }

                    students.push_back(student);
                    break;
                }
            }
        }
    }

    file.close();
    return students;
}

template <typename RollType, typename CourseType>
std::vector<Student<RollType, CourseType>> Database::loadStudentsRange(int start, int count)
{
    std::vector<Student<RollType, CourseType>> students;
    std::ifstream file(STUDENT_DB_FILE);

    if (!file.is_open())
    {
        return students; // Return empty vector if file doesn't exist
    }

    std::string line;
    int currentIndex = 0;
    int loadedCount = 0;

    while (std::getline(file, line) && loadedCount < count)
    {
        if (line == "STUDENT_START")
        {
            // Check if we should process this student
            if (currentIndex >= start)
            {
                RollType rollNumber;
                std::string name, branchCode, branchName, department, levelStr;
                int startYear;

                // Read student basic info
                while (std::getline(file, line) && line != "STUDENT_END")
                {
                    if (line.find("RollNumber: ") == 0)
                    {
                        std::istringstream iss(line.substr(12));
                        iss >> rollNumber;
                    }
                    else if (line.find("Name: ") == 0)
                    {
                        name = line.substr(6);
                    }
                    else if (line.find("BranchCode: ") == 0)
                    {
                        branchCode = line.substr(12);
                    }
                    else if (line.find("BranchName: ") == 0)
                    {
                        branchName = line.substr(12);
                    }
                    else if (line.find("Department: ") == 0)
                    {
                        department = line.substr(12);
                    }
                    else if (line.find("Level: ") == 0)
                    {
                        levelStr = line.substr(7);
                    }
                    else if (line.find("StartYear: ") == 0)
                    {
                        startYear = std::stoi(line.substr(11));
                    }
                    else if (line.find("CurrentCourses: ") == 0)
                    {
                        int numCourses = std::stoi(line.substr(16));

                        Branch branch(branchCode, branchName, department);
                        StudentLevel level = stringToLevel(levelStr);
                        Student<RollType, CourseType> student(name, rollNumber, branch, level, startYear);

                        // Read current courses
                        for (int i = 0; i < numCourses; ++i)
                        {
                            std::getline(file, line); // COURSE_START

                            CourseType courseCode;
                            std::string courseName, instructor, semester;
                            int credits;
                            double midSem = -1, endSem = -1, assignment = -1;

                            while (std::getline(file, line) && line != "COURSE_END")
                            {
                                if (line.find("CourseCode: ") == 0)
                                {
                                    std::istringstream iss(line.substr(12));
                                    iss >> courseCode;
                                }
                                else if (line.find("CourseName: ") == 0)
                                {
                                    courseName = line.substr(12);
                                }
                                else if (line.find("Instructor: ") == 0)
                                {
                                    instructor = line.substr(12);
                                }
                                else if (line.find("Credits: ") == 0)
                                {
                                    credits = std::stoi(line.substr(9));
                                }
                                else if (line.find("Semester: ") == 0)
                                {
                                    semester = line.substr(10);
                                }
                                else if (line.find("MidSem: ") == 0)
                                {
                                    midSem = std::stod(line.substr(8));
                                }
                                else if (line.find("EndSem: ") == 0)
                                {
                                    endSem = std::stod(line.substr(8));
                                }
                                else if (line.find("Assignment: ") == 0)
                                {
                                    assignment = std::stod(line.substr(12));
                                }
                            }

                            Course<CourseType> course(courseCode, courseName, instructor, credits, semester);

                            if (midSem >= 0 && endSem >= 0 && assignment >= 0)
                            {
                                Grade grade(midSem, endSem, assignment);
                                student.add_current_course(course, grade);
                            }
                            else
                            {
                                student.add_current_course(course);
                            }
                        }

                        // Read previous courses
                        std::getline(file, line); // PreviousCourses: N
                        if (line.find("PreviousCourses: ") == 0)
                        {
                            int numPrevCourses = std::stoi(line.substr(17));

                            for (int i = 0; i < numPrevCourses; ++i)
                            {
                                std::getline(file, line); // COURSE_START

                                CourseType courseCode;
                                std::string courseName, instructor, semester;
                                int credits;
                                double midSem = -1, endSem = -1, assignment = -1;

                                while (std::getline(file, line) && line != "COURSE_END")
                                {
                                    if (line.find("CourseCode: ") == 0)
                                    {
                                        std::istringstream iss(line.substr(12));
                                        iss >> courseCode;
                                    }
                                    else if (line.find("CourseName: ") == 0)
                                    {
                                        courseName = line.substr(12);
                                    }
                                    else if (line.find("Instructor: ") == 0)
                                    {
                                        instructor = line.substr(12);
                                    }
                                    else if (line.find("Credits: ") == 0)
                                    {
                                        credits = std::stoi(line.substr(9));
                                    }
                                    else if (line.find("Semester: ") == 0)
                                    {
                                        semester = line.substr(10);
                                    }
                                    else if (line.find("MidSem: ") == 0)
                                    {
                                        midSem = std::stod(line.substr(8));
                                    }
                                    else if (line.find("EndSem: ") == 0)
                                    {
                                        endSem = std::stod(line.substr(8));
                                    }
                                    else if (line.find("Assignment: ") == 0)
                                    {
                                        assignment = std::stod(line.substr(12));
                                    }
                                }

                                Course<CourseType> course(courseCode, courseName, instructor, credits, semester);
                                if (midSem >= 0 && endSem >= 0 && assignment >= 0)
                                {
                                    Grade grade(midSem, endSem, assignment);
                                    student.add_current_course(course, grade);
                                    student.move_to_previous_courses(courseCode);
                                }
                            }
                        }

                        students.push_back(student);
                        loadedCount++;
                        break;
                    }
                }
            }
            else
            {
                // Skip this student - just fast-forward to STUDENT_END
                while (std::getline(file, line) && line != "STUDENT_END")
                {
                    // Keep reading until we find STUDENT_END
                }
            }
            currentIndex++;
        }
    }

    file.close();
    return students;
}

#endif // DATABASE_H
