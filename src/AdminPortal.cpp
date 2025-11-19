#include "AdminPortal.h"
#include "CSVHandler.h"
#include "Database.h"
#include "SortingManager.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <thread>
#include <future>
#include <mutex>
#include <sstream>
#include <map>

// Helper function to convert string to uppercase
static std::string toUpperCase(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c)
                   { return std::toupper(c); });
    return result;
}

AdminPortal::AdminPortal(const std::string &username,
                         ERPSystem<std::string, std::string> *erp,
                         CourseCatalog *catalog,
                         SemesterManager *semMgr)
    : adminUsername(username), erpSystem(erp), courseCatalog(catalog),
      semesterManager(semMgr)
{
}

void AdminPortal::displayMenu()
{
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    ADMIN PORTAL - IIIT-Delhi ERP               ║\n";
    std::cout << "║                    Logged in as: " << std::left << std::setw(27) << adminUsername << "   ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  STUDENT MANAGEMENT                                            ║\n";
    std::cout << "║   1. Add Single Student                                        ║\n";
    std::cout << "║   2. Add Students from CSV (Bulk)                              ║\n";
    std::cout << "║   3. View All Students                                         ║\n";
    std::cout << "║   4. Search Student                                            ║\n";
    std::cout << "║   5. Update Student Marks                                      ║\n";
    std::cout << "║   6. Delete Single Student                                     ║\n";
    std::cout << "║   7. Delete Students from CSV (Bulk)                           ║\n";
    std::cout << "║   8. Delete ALL Students (DANGEROUS)                           ║\n";
    std::cout << "║   9. Sort Students (By Name/Roll/CGPA + Threading)             ║\n";
    std::cout << "║  10. Create Student Account (Generate Credentials)             ║\n";
    std::cout << "║  11. Bulk Create All Student Accounts (abc123)                 ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  COURSE MANAGEMENT                                             ║\n";
    std::cout << "║  12. Manage Course Catalog                                     ║\n";
    std::cout << "║  13. View Course Statistics                                    ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  COURSE REGISTRATION APPROVAL                                  ║\n";
    std::cout << "║  14. View Pending Course Requests                              ║\n";
    std::cout << "║  15. Approve/Reject Course Requests                            ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  SEMESTER MANAGEMENT                                           ║\n";

    // Line 16: Course Registration Status
    std::string regStatus = semesterManager->isCourseAddEnabled() ? "OPEN  " : "CLOSED";
    std::cout << "║  16. Toggle Course Registration (Currently: " << regStatus << ")            ║\n";

    // Line 17: Course Drop Status
    std::string dropStatus = semesterManager->isCourseDropEnabled() ? "ENABLED" : "DISABLED";
    std::cout << "║  17. Toggle Course Drop (Currently: " << dropStatus << ")                  ║\n";

    std::cout << "║  18. View Semester Manager Status                              ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  DATABASE & BACKUP                                             ║\n";
    std::cout << "║  19. View Backup Information                                   ║\n";
    std::cout << "║  20. Restore from Backup                                       ║\n";
    std::cout << "║  21. Export Data to CSV                                        ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  COURSE ENROLLMENT & ANALYSIS                                  ║\n";
    std::cout << "║  22. View Course Enrollments (Students per Course)             ║\n";
    std::cout << "║  23. IIIT-IIT Course Integration Demo (Template Demo)          ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  ITERATORS & GRADE SEARCH (Assignment Features)                ║\n";
    std::cout << "║  25. View Students (Insertion Order)                           ║\n";
    std::cout << "║  26. View Students (Sorted Order)                              ║\n";
    std::cout << "║  27. Find Students by Grade (Quick Search)                     ║\n";
    std::cout << "║  28. Rebuild Grade Index                                       ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  SYSTEM                                                        ║\n";
    std::cout << "║  29. View System Statistics                                    ║\n";
    std::cout << "║  30. Change Password                                           ║\n";
    std::cout << "║  31. Logout                                                    ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    std::cout << "Enter your choice: ";
}

void AdminPortal::run()
{
    int choice;

    while (true)
    {
        displayMenu();

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore();

        switch (choice)
        {
        case 1:
            addSingleStudent();
            break;
        case 2:
            addBulkStudents();
            break;
        case 3:
            viewAllStudents();
            break;
        case 4:
            searchStudent();
            break;
        case 5:
            updateStudentMarks();
            break;
        case 6:
            deleteStudent();
            break;
        case 7:
            deleteBulkStudents();
            break;
        case 8:
            deleteAllStudents();
            break;
        case 9:
            sortStudentsMenu();
            break;
        case 10:
            createStudentAccount();
            break;
        case 11:
            bulkCreateStudentAccounts();
            break;
        case 12:
            manageCourses();
            break;
        case 13:
            std::cout << "\nCourse statistics feature - coming soon!\n";
            break;
        case 14:
            viewPendingCourseRequests();
            break;
        case 15:
            approveCourseRequest();
            break;
        case 16:
            semesterManager->setCourseAddEnabled(!semesterManager->isCourseAddEnabled());
            break;
        case 17:
            semesterManager->setCourseDropEnabled(!semesterManager->isCourseDropEnabled());
            break;
        case 18:
            std::cout << semesterManager->getStatus();
            break;
        case 19:
            viewBackups();
            break;
        case 20:
            restoreBackup();
            break;
        case 21:
            exportData();
            break;
        case 22:
            viewCourseEnrollments();
            break;
        case 23:
            demoIIITIITCourseIntegration();
            break;
        case 24:
            changeOwnPassword();
            break;
        case 25:
            viewStudentsInsertionOrder();
            break;
        case 26:
            viewStudentsSortedOrder();
            break;
        case 27:
            findStudentsByGrade();
            break;
        case 28:
            rebuildGradeIndex();
            break;
        case 29:
            viewStatistics();
            break;
        case 30:
            changeOwnPassword();
            break;
        case 31:
            std::cout << "\nLogging out from admin portal...\n";
            return;
        case 32:
            std::cout << "\nLogging out from admin portal...\n";
            return;
        default:
            std::cout << "\nInvalid choice. Please try again.\n";
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
}

void AdminPortal::addSingleStudent()
{
    std::cout << "\n=== Add Single Student ===\n";

    std::string rollNumber, name, branchCode, level;
    int startYear;

    std::cout << "Enter Roll Number (max 15 chars, no spaces): ";
    std::getline(std::cin, rollNumber);
    rollNumber = toUpperCase(rollNumber); // Convert to uppercase

    std::cout << "Enter Name (FirstName LastName format): ";
    std::getline(std::cin, name);
    name = toUpperCase(name); // Convert to uppercase

    std::cout << "Enter Level (BTECH/MTECH/PHD/DUAL_DEGREE): ";
    std::getline(std::cin, level);
    level = toUpperCase(level); // Convert to uppercase

    // Parse StudentLevel enum
    StudentLevel studentLevel;
    if (level == "BTECH")
        studentLevel = StudentLevel::BTECH;
    else if (level == "MTECH")
        studentLevel = StudentLevel::MTECH;
    else if (level == "PHD")
        studentLevel = StudentLevel::PHD;
    else if (level == "DUAL_DEGREE")
        studentLevel = StudentLevel::DUAL_DEGREE;
    else
    {
        std::cout << "\nX Invalid level. Use BTECH/MTECH/PHD/DUAL_DEGREE\n";
        return;
    }

    // Display branches based on level
    std::cout << "\n=== Available Branches for " << level << " ===\n";

    if (studentLevel == StudentLevel::BTECH || studentLevel == StudentLevel::DUAL_DEGREE)
    {
        std::cout << "1.  CSE     - Computer Science and Engineering\n";
        std::cout << "2.  ECE     - Electronics and Communications Engineering\n";
        std::cout << "3.  CSAI    - Computer Science & Artificial Intelligence\n";
        std::cout << "4.  CSAM    - Computer Science and Applied Mathematics\n";
        std::cout << "5.  CSD     - Computer Science and Design\n";
        std::cout << "6.  CSB     - Computer Science and Biosciences\n";
        std::cout << "7.  CSSS    - Computer Science and Social Sciences\n";
        std::cout << "8.  EVE     - Electronics & VLSI Engineering\n";
        std::cout << "9.  CSEcon  - Computer Science and Economics\n";
    }
    else if (studentLevel == StudentLevel::MTECH)
    {
        std::cout << "1. MTCSE - M.Tech Computer Science & Engineering\n";
        std::cout << "   Specializations: AI, Data Engineering, Information Security, Mobile Computing\n";
        std::cout << "2. MTECE - M.Tech Electronics & Communications Engineering\n";
        std::cout << "   Specializations: VLSI, Embedded Systems, Communication, Machine Learning\n";
        std::cout << "3. MTCB  - M.Tech Computational Biology\n";
    }
    else if (studentLevel == StudentLevel::PHD)
    {
        std::cout << "1. PHDCSE   - PhD Computer Science & Engineering\n";
        std::cout << "2. PHDECE   - PhD Electronics & Communications Engineering\n";
        std::cout << "3. PHDCB    - PhD Computational Biology\n";
        std::cout << "4. PHDMATH  - PhD Mathematics\n";
        std::cout << "5. PHDHCD   - PhD Human-Centred & Design\n";
        std::cout << "6. PHDSSH   - PhD Social Sciences & Humanities\n";
    }

    std::cout << "\nEnter Branch Code: ";
    std::getline(std::cin, branchCode);
    branchCode = toUpperCase(branchCode); // Convert to uppercase

    std::cout << "Enter Start Year: ";
    std::cin >> startYear;
    std::cin.ignore();

    try
    {
        // Create branch based on code
        Branch branch(branchCode, branchCode, "Department"); // Will be replaced by proper mapping
        auto student = std::make_shared<Student<std::string, std::string>>(name, rollNumber, branch, studentLevel, startYear);
        erpSystem->addStudent(student);

        // Save insertion order to file
        erpSystem->saveInsertionOrderToFile();

        std::cout << "\n=> Student added successfully!\n";
        std::cout << "  Roll Number: " << rollNumber << "\n";
        std::cout << "  Name: " << name << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

void AdminPortal::addBulkStudents()
{
    std::cout << "\n=== Add Students from CSV ===\n";
    std::cout << "Enter CSV filename (default: demo_students.csv): ";

    std::string filename;
    std::getline(std::cin, filename);

    // Use default if user pressed Enter without typing
    if (filename.empty())
    {
        filename = "demo_students.csv";
        std::cout << "Using default file: demo_students.csv\n";
    }

    try
    {
        CSVHandler<std::string, std::string> csvHandler(filename);
        auto students = csvHandler.readStudents();

        std::cout << "\nFound " << students.size() << " students in CSV.\n";
        std::cout << "Adding to system...\n";

        int added = 0;
        for (const auto &student : students)
        {
            try
            {
                erpSystem->addStudent(student);
                added++;
            }
            catch (const std::exception &e)
            {
                std::cout << "Warning: Could not add student "
                          << student->getRollNumber() << ": " << e.what() << "\n";
            }
        }

        std::cout << "\n=> Successfully added " << added << " out of "
                  << students.size() << " students.\n";

        // Save insertion order to file after bulk add
        if (added > 0)
        {
            erpSystem->saveInsertionOrderToFile();
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

// =============================================================================
// COMPREHENSIVE SORTING SYSTEM - By Name, Roll Number, or CGPA
// =============================================================================

void AdminPortal::sortStudentsMenu()
{
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║              STUDENT SORTING SYSTEM                            ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  Select Sorting Criteria:                                      ║\n";
    std::cout << "║   1. Sort by Name (Dictionary Order)                           ║\n";
    std::cout << "║   2. Sort by Roll Number                                       ║\n";
    std::cout << "║   3. Sort by CGPA (Descending)                                 ║\n";
    std::cout << "║   0. Back to Main Menu                                         ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    std::cout << "Enter your choice: ";

    int criteria;
    if (!(std::cin >> criteria))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        return;
    }
    std::cin.ignore();

    if (criteria == 0)
        return;

    if (criteria < 1 || criteria > 3)
    {
        std::cout << "Invalid choice.\n";
        return;
    }

    // Now ask for threading option
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Select Threading Mode:                                        ║\n";
    std::cout << "║   1. Single-threaded (Sequential)                              ║\n";
    std::cout << "║   2. Multi-threaded (Parallel - 2 threads)                     ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    std::cout << "Enter your choice: ";

    int threadChoice;
    if (!(std::cin >> threadChoice))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        return;
    }
    std::cin.ignore();

    if (threadChoice < 1 || threadChoice > 2)
    {
        std::cout << "Invalid choice.\n";
        return;
    }

    bool multiThreaded = (threadChoice == 2);

    // Perform the sort
    sortStudents(criteria, multiThreaded);
}

void AdminPortal::sortStudents(int criteria, bool multiThreaded)
{
    const char *criteriaNames[] = {"", "Name", "Roll Number", "CGPA"};
    const char *threadMode = multiThreaded ? "Multi-threaded" : "Single-threaded";

    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  " << std::left << std::setw(60) << (std::string(threadMode) + " Sort by " + criteriaNames[criteria]) << "  ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

    try
    {
        // Load all students from memory
        auto loadStart = std::chrono::high_resolution_clock::now();
        auto allStudents = erpSystem->getAllStudents();
        auto loadEnd = std::chrono::high_resolution_clock::now();
        long long loadMs = std::chrono::duration_cast<std::chrono::milliseconds>(loadEnd - loadStart).count();

        if (allStudents.empty())
        {
            std::cout << "No students in the system to sort.\n";
            return;
        }

        int totalCount = allStudents.size();
        std::cout << "Loaded " << totalCount << " students from memory in " << loadMs << " ms\n";

        if (!multiThreaded)
        {
            // ========== SINGLE-THREADED SORT ==========
            auto sortStart = std::chrono::high_resolution_clock::now();

            switch (criteria)
            {
            case 1: // Sort by Name
                std::sort(allStudents.begin(), allStudents.end(),
                          [](const auto &a, const auto &b)
                          { return a->getName() < b->getName(); });
                break;
            case 2: // Sort by Roll Number
                std::sort(allStudents.begin(), allStudents.end(),
                          [](const auto &a, const auto &b)
                          { return a->getRollNumber() < b->getRollNumber(); });
                break;
            case 3: // Sort by CGPA (descending)
                std::sort(allStudents.begin(), allStudents.end(),
                          [](const auto &a, const auto &b)
                          { return a->getLoadedCGPA() > b->getLoadedCGPA(); });
                break;
            }

            auto sortEnd = std::chrono::high_resolution_clock::now();
            long long sortMs = std::chrono::duration_cast<std::chrono::milliseconds>(sortEnd - sortStart).count();

            std::cout << "Sorted " << totalCount << " students in " << sortMs << " ms\n";

            // Update ERP system with sorted list (only in-memory, not persisted)
            erpSystem->setSortedStudents(allStudents);

            std::cout << "\n--- Performance Summary ---\n";
            std::cout << "Sort time: " << sortMs << " ms\n";
            std::cout << "Note: Sorted order is in-memory only, not saved to database.\n";
            std::cout << "      This preserves insertion order for the next program run.\n";
        }
        else
        {
            // ========== MULTI-THREADED SORT USING SORTINGMANAGER ==========

            // Create SortingManager with appropriate criteria
            SortCriteria sortCriteria;
            switch (criteria)
            {
            case 1:
                sortCriteria = SortCriteria::BY_NAME;
                break;
            case 2:
                sortCriteria = SortCriteria::BY_ROLL_NUMBER;
                break;
            case 3:
                sortCriteria = SortCriteria::BY_CGPA;
                break;
            default:
                sortCriteria = SortCriteria::BY_NAME;
                break;
            }

            SortingManager<std::string, std::string> sortingManager(2, sortCriteria);

            // Perform parallel sort (automatically determines optimal thread count)
            auto sortedStudents = sortingManager.parallelSort(allStudents);

            // Update ERP system with sorted list (only in-memory, not persisted)
            erpSystem->setSortedStudents(sortedStudents);

            // Display thread performance logs
            sortingManager.displayThreadLogs();

            std::cout << "Note: Sorted order is in-memory only, not saved to database.\n";
            std::cout << "      This preserves insertion order for the next program run.\n";
        }

        // Print first 10 students after sorting
        std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║  First 10 Students After Sorting:                             ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
        auto sortedStudents = erpSystem->getAllStudents();
        for (size_t i = 0; i < sortedStudents.size() && i < 10; ++i)
        {
            std::cout << "  " << std::setw(3) << (i + 1) << ". "
                      << std::setw(20) << sortedStudents[i]->getRollNumber()
                      << " | " << std::setw(25) << sortedStudents[i]->getName()
                      << " | CGPA: " << std::fixed << std::setprecision(2) << sortedStudents[i]->getLoadedCGPA() << "\n";
        }

        std::cout << "\n=> Students sorted successfully!\n";
        std::cout << "   Insertion order preserved. Database unchanged.\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

void AdminPortal::viewAllStudents()
{
    std::cout << "\n=== All Students ===\n";

    auto students = erpSystem->getAllStudents();

    if (students.empty())
    {
        std::cout << "No students in the system.\n";
        return;
    }

    std::cout << "\nTotal Students: " << students.size() << "\n\n";
    std::cout << std::left << std::setw(16) << "Roll No"
              << std::setw(33) << "Name"
              << std::setw(48) << "Branch"
              << std::setw(14) << "Level"
              << std::setw(10) << "Year" << "\n";
    std::cout << std::string(121, '-') << "\n";

    for (const auto &student : students)
    {
        // Convert StudentLevel enum to string
        std::string levelStr;
        switch (student->getLevel())
        {
        case StudentLevel::BTECH:
            levelStr = "BTECH";
            break;
        case StudentLevel::MTECH:
            levelStr = "MTECH";
            break;
        case StudentLevel::PHD:
            levelStr = "PHD";
            break;
        case StudentLevel::DUAL_DEGREE:
            levelStr = "DUAL_DEGREE";
            break;
        }

        std::cout << std::left << std::setw(16) << student->getRollNumber()
                  << std::setw(33) << student->getName()
                  << std::setw(48) << student->getBranch().getBranchName()
                  << std::setw(14) << levelStr
                  << std::setw(10) << student->getStartingYear() << "\n";
    }
}

void AdminPortal::searchStudent()
{
    std::cout << "\n=== Search Student ===\n";
    std::cout << "Enter Roll Number: ";

    std::string rollNumber;
    std::getline(std::cin, rollNumber);

    try
    {
        auto student = erpSystem->findStudent(rollNumber);
        if (!student)
        {
            std::cout << "\nX Student not found.\n";
            return;
        }

        // Convert StudentLevel enum to string
        std::string levelStr;
        switch (student->getLevel())
        {
        case StudentLevel::BTECH:
            levelStr = "BTECH";
            break;
        case StudentLevel::MTECH:
            levelStr = "MTECH";
            break;
        case StudentLevel::PHD:
            levelStr = "PHD";
            break;
        case StudentLevel::DUAL_DEGREE:
            levelStr = "DUAL_DEGREE";
            break;
        }

        std::cout << "\n=> Student Found:\n";
        std::cout << "Roll Number: " << student->getRollNumber() << "\n";
        std::cout << "Name: " << student->getName() << "\n";
        std::cout << "Branch: " << student->getBranch().getBranchName() << "\n";
        std::cout << "Level: " << levelStr << "\n";
        std::cout << "Start Year: " << student->getStartingYear() << "\n";

        // Combine current and previous courses
        auto currentCourses = student->getCurrentCourses();
        auto previousCourses = student->getPreviousCourses();

        std::cout << "\nCurrent Courses: " << currentCourses.size() << "\n";
        std::cout << "Previous Courses: " << previousCourses.size() << "\n";
        std::cout << "Total Enrollments: " << (currentCourses.size() + previousCourses.size()) << "\n";

        if (!currentCourses.empty())
        {
            std::cout << "\nCurrent Courses:\n";
            std::cout << std::string(80, '-') << "\n";
            for (const auto &course : currentCourses)
            {
                std::cout << course.getCourseCode() << " - "
                          << course.getCourseName() << " ("
                          << course.getCredits() << " credits)\n";
            }
        }

        if (!previousCourses.empty())
        {
            std::cout << "\nPrevious Courses:\n";
            std::cout << std::string(80, '-') << "\n";
            auto previousGrades = student->getPreviousGrades();
            for (size_t i = 0; i < previousCourses.size(); ++i)
            {
                const auto &course = previousCourses[i];
                std::cout << course.getCourseCode() << " - "
                          << course.getCourseName() << " ("
                          << course.getCredits() << " credits)";

                if (i < previousGrades.size())
                {
                    const auto &grade = previousGrades[i];
                    std::cout << " - Grade: " << grade.getLetterGrade()
                              << " (" << std::fixed << std::setprecision(2)
                              << grade.getGradePoint() << ")";
                }
                std::cout << "\n";
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

void AdminPortal::updateStudentMarks()
{
    std::cout << "\n=== Update Student Marks ===\n";
    std::cout << "Enter Roll Number: ";

    std::string rollNumber;
    std::getline(std::cin, rollNumber);

    try
    {
        auto student = erpSystem->findStudent(rollNumber);
        if (!student)
        {
            std::cout << "\nX Student not found.\n";
            return;
        }

        auto currentCourses = student->getCurrentCourses();

        if (currentCourses.empty())
        {
            std::cout << "Student has no current course enrollments.\n";
            return;
        }

        std::cout << "\nCurrent Courses:\n";
        for (size_t i = 0; i < currentCourses.size(); ++i)
        {
            std::cout << i + 1 << ". " << currentCourses[i].getCourseCode()
                      << " - " << currentCourses[i].getCourseName() << "\n";
        }

        std::cout << "\nSelect course number: ";
        int courseNum;
        std::cin >> courseNum;
        std::cin.ignore();

        if (courseNum < 1 || courseNum > static_cast<int>(currentCourses.size()))
        {
            std::cout << "Invalid course number.\n";
            return;
        }

        std::cout << "\nEnter Mid Sem marks (0-100): ";
        double midSem;
        std::cin >> midSem;

        std::cout << "Enter End Sem marks (0-100): ";
        double endSem;
        std::cin >> endSem;

        std::cout << "Enter Assignment marks (0-100): ";
        double assignment;
        std::cin >> assignment;
        std::cin.ignore();

        // Calculate grade from marks
        double total = (midSem * 0.3) + (endSem * 0.5) + (assignment * 0.2);
        Grade grade(total);

        // Update grade through ERP system
        erpSystem->updateStudentGrade(rollNumber, currentCourses[courseNum - 1].getCourseCode(), grade);

        std::cout << "\n=> Marks updated successfully!\n";
        std::cout << "  Total: " << std::fixed << std::setprecision(2) << total << "\n";
        std::cout << "  Grade: " << grade.getLetterGrade() << " (" << grade.getGradePoint() << ")\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

void AdminPortal::deleteStudent()
{
    std::cout << "\n=== Delete Single Student ===\n";
    std::cout << "Enter Roll Number: ";

    std::string rollNumber;
    std::getline(std::cin, rollNumber);
    rollNumber = toUpperCase(rollNumber);

    // Check if student exists
    auto student = erpSystem->findStudent(rollNumber);
    if (!student)
    {
        std::cout << "\nX Student with roll number '" << rollNumber << "' not found.\n";
        return;
    }

    // Display student info
    std::cout << "\nStudent Details:\n";
    std::cout << "  Roll Number: " << student->getRollNumber() << "\n";
    std::cout << "  Name: " << student->getName() << "\n";
    std::cout << "  Branch: " << student->getBranch().getBranchName() << "\n";

    std::cout << "\nAre you sure you want to delete this student? (yes/no): ";
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm == "yes" || confirm == "YES")
    {
        if (erpSystem->removeStudent(rollNumber))
        {
            // Save insertion order to file after removal
            erpSystem->saveInsertionOrderToFile();

            std::cout << "\n=> Student deleted successfully from database.\n";
            std::cout << "   Note: Student account credentials (if any) remain and must be manually removed.\n";
        }
        else
        {
            std::cout << "\nX Student could not be deleted from database.\n";
        }
    }
    else
    {
        std::cout << "\nDeletion cancelled.\n";
    }
}

void AdminPortal::deleteBulkStudents()
{
    std::cout << "\n=== Delete Students from CSV (Bulk) ===\n";
    std::cout << "Enter CSV filename (must contain roll numbers): ";

    std::string filename;
    std::getline(std::cin, filename);

    std::cout << "\nThis will delete all students whose roll numbers are listed in the CSV file.\n";
    std::cout << "Are you sure you want to proceed? (yes/no): ";
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm != "yes" && confirm != "YES")
    {
        std::cout << "\nBulk deletion cancelled.\n";
        return;
    }

    try
    {
        // Open CSV file
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cout << "\nX Error: Could not open file '" << filename << "'\n";
            return;
        }

        std::vector<std::string> rollNumbers;
        std::string line;
        bool isFirstLine = true;

        // Read roll numbers from CSV
        while (std::getline(file, line))
        {

            // Skip empty lines
            if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos)
                continue;

            // Skip header line if it looks like a header
            if (isFirstLine)
            {
                isFirstLine = false;
                std::string upperLine = toUpperCase(line);
                if (upperLine.find("ROLL") != std::string::npos ||
                    upperLine.find("NAME") != std::string::npos ||
                    upperLine.find("STUDENT") != std::string::npos)
                {
                    continue;
                }
            }

            // Parse CSV line - extract first column (roll number)
            size_t commaPos = line.find(',');
            std::string rollNumber;

            if (commaPos != std::string::npos)
            {
                rollNumber = line.substr(0, commaPos);
            }
            else
            {
                rollNumber = line; // Single column file
            }

            // Trim whitespace
            rollNumber.erase(0, rollNumber.find_first_not_of(" \t\r\n"));
            rollNumber.erase(rollNumber.find_last_not_of(" \t\r\n") + 1);

            if (!rollNumber.empty())
            {
                rollNumbers.push_back(toUpperCase(rollNumber));
            }
        }
        file.close();

        if (rollNumbers.empty())
        {
            std::cout << "\nX No valid roll numbers found in CSV file.\n";
            return;
        }

        std::cout << "\nFound " << rollNumbers.size() << " roll number(s) in file.\n";
        std::cout << "Processing deletions...\n\n";

        int successCount = 0;
        int notFoundCount = 0;

        for (const auto &rollNumber : rollNumbers)
        {
            // Check if student exists
            auto student = erpSystem->findStudent(rollNumber);

            if (!student)
            {
                std::cout << "  X Roll " << rollNumber << " - not found\n";
                notFoundCount++;
                continue;
            }

            // Delete from database
            if (erpSystem->removeStudent(rollNumber))
            {
                std::cout << "  ✓ Roll " << rollNumber << " - deleted (" << student->getName() << ")\n";
                successCount++;
            }
            else
            {
                std::cout << "  X Roll " << rollNumber << " - could not delete\n";
            }
        }

        std::cout << "\n=== Bulk Deletion Summary ===\n";
        std::cout << "Total roll numbers in file: " << rollNumbers.size() << "\n";
        std::cout << "Successfully deleted: " << successCount << "\n";
        std::cout << "Not found: " << notFoundCount << "\n";

        // Save insertion order to file after bulk deletions
        if (successCount > 0)
        {
            erpSystem->saveInsertionOrderToFile();
        }

        std::cout << "=> Bulk deletion completed.\n";
        std::cout << "   Note: Student account credentials (if any) remain and must be manually removed.\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error during bulk deletion: " << e.what() << "\n";
    }
}

void AdminPortal::deleteAllStudents()
{
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║               ⚠️  WARNING: DELETE ALL STUDENTS  ⚠️               ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  This action will PERMANENTLY DELETE ALL STUDENTS from the    ║\n";
    std::cout << "║  database. This operation CANNOT be undone!                   ║\n";
    std::cout << "║                                                                ║\n";
    std::cout << "║  All student records, enrollment data, and grades will be     ║\n";
    std::cout << "║  removed from the system.                                     ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";

    // Get current student count
    auto students = erpSystem->getAllStudents();
    size_t totalStudents = students.size();

    if (totalStudents == 0)
    {
        std::cout << "\nNo students in the database to delete.\n";
        return;
    }

    std::cout << "\nCurrent number of students in database: " << totalStudents << "\n";

    // First confirmation
    std::cout << "\nAre you absolutely sure you want to delete ALL students? (yes/no): ";
    std::string confirm1;
    std::getline(std::cin, confirm1);

    if (confirm1 != "yes" && confirm1 != "YES")
    {
        std::cout << "\nDeletion cancelled.\n";
        return;
    }

    // Second confirmation with exact phrase
    std::cout << "\n⚠️  FINAL WARNING ⚠️\n";
    std::cout << "To confirm, type exactly: DELETE ALL STUDENTS\n";
    std::cout << "> ";
    std::string confirm2;
    std::getline(std::cin, confirm2);

    if (confirm2 != "DELETE ALL STUDENTS")
    {
        std::cout << "\nDeletion cancelled. (Confirmation phrase did not match)\n";
        return;
    }

    std::cout << "\n[*] Starting deletion of all students...\n\n";

    int successCount = 0;
    int failCount = 0;

    // Create a copy of roll numbers to delete (to avoid iterator invalidation)
    std::vector<std::string> rollNumbers;
    rollNumbers.reserve(totalStudents);
    for (const auto &student : students)
    {
        rollNumbers.push_back(student->getRollNumber());
    }

    // Progress indicator
    std::cout << "Progress: [";
    size_t progressBarWidth = 50;
    size_t lastProgress = 0;

    for (size_t i = 0; i < rollNumbers.size(); ++i)
    {
        const auto &rollNumber = rollNumbers[i];

        if (erpSystem->removeStudent(rollNumber))
        {
            successCount++;
        }
        else
        {
            failCount++;
        }

        // Update progress bar
        size_t currentProgress = (i + 1) * progressBarWidth / rollNumbers.size();
        if (currentProgress > lastProgress)
        {
            for (size_t j = lastProgress; j < currentProgress; ++j)
            {
                std::cout << "=";
                std::cout.flush();
            }
            lastProgress = currentProgress;
        }
    }
    std::cout << "] 100%\n\n";

    // Summary
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║               DELETE ALL STUDENTS - SUMMARY                    ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  Total students processed: " << std::setw(36) << totalStudents << "║\n";
    std::cout << "║  Successfully deleted:     " << std::setw(36) << successCount << "║\n";
    std::cout << "║  Failed to delete:         " << std::setw(36) << failCount << "║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";

    if (successCount > 0)
    {
        // Clear insertion order file
        Database db;
        db.clearInsertionOrder();

        std::cout << "\n=> All students have been deleted from the database.\n";
        std::cout << "   Note: Student account credentials (if any) remain and must be manually removed.\n";
    }

    if (failCount > 0)
    {
        std::cout << "\n⚠️  Warning: " << failCount << " student(s) could not be deleted.\n";
    }
}

void AdminPortal::createStudentAccount()
{
    std::cout << "\n=== Create Student Account ===\n";
    std::cout << "Enter Roll Number: ";

    std::string rollNumber;
    std::getline(std::cin, rollNumber);

    try
    {
        // Check if student exists in ERP
        auto student = erpSystem->findStudent(rollNumber);
        if (!student)
        {
            std::cout << "\nX Student not found in ERP system.\n";
            std::cout << "  Please add the student first before creating account.\n";
            return;
        }

        // Create credentials
        std::string password = Auth::createStudentAccount(rollNumber);

        std::cout << "\n=> Student account created successfully!\n";
        std::cout << "  Roll Number (Login ID): " << rollNumber << "\n";
        std::cout << "  Generated Password: " << password << "\n";
        std::cout << "\n  ** Please share these credentials with the student **\n";
        std::cout << "  ** Student should change password after first login **\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

void AdminPortal::bulkCreateStudentAccounts()
{
    std::cout << "\n=== Bulk Create Student Accounts ===\n";
    std::cout << "\nThis will create accounts for ALL students in the database\n";
    std::cout << "who do not already have credentials.\n";
    std::cout << "\nDefault Password: abc123\n";
    std::cout << "User ID: Student's Roll Number\n";
    std::cout << "\nDo you want to proceed? (YES/NO): ";

    std::string confirm;
    std::getline(std::cin, confirm);
    confirm = toUpperCase(confirm);

    if (confirm != "YES")
    {
        std::cout << "\nBulk account creation cancelled.\n";
        return;
    }

    std::cout << "\n[*] Processing bulk account creation...\n\n";

    // Get all students from ERP system
    auto students = erpSystem->getAllStudents();

    if (students.empty())
    {
        std::cout << "X No students found in the database.\n";
        return;
    }

    int created = 0;
    int skipped = 0;
    const std::string defaultPassword = "abc123";

    std::cout << std::left << std::setw(20) << "Roll Number"
              << std::setw(35) << "Name"
              << "Status\n";
    std::cout << std::string(70, '-') << "\n";

    for (const auto &student : students)
    {
        std::string rollNumber = student->getRollNumber();
        std::string name = student->getName();

        try
        {
            bool success = Auth::createStudentAccountWithPassword(rollNumber, defaultPassword);

            if (success)
            {
                created++;
                std::cout << std::left << std::setw(20) << rollNumber
                          << std::setw(35) << name
                          << "=> Created\n";
            }
            else
            {
                skipped++;
                std::cout << std::left << std::setw(20) << rollNumber
                          << std::setw(35) << name
                          << "   Skipped (Already exists)\n";
            }
        }
        catch (const std::exception &e)
        {
            skipped++;
            std::cout << std::left << std::setw(20) << rollNumber
                      << std::setw(35) << name
                      << "X Error: " << e.what() << "\n";
        }
    }

    std::cout << "\n"
              << std::string(70, '=') << "\n";
    std::cout << "=> Bulk account creation completed!\n\n";
    std::cout << "Total Students: " << students.size() << "\n";
    std::cout << "Accounts Created: " << created << "\n";
    std::cout << "Skipped (Already existed): " << skipped << "\n";

    if (created > 0)
    {
        std::cout << "\n** All new accounts created with password: abc123 **\n";
        std::cout << "** Students should change their password after first login **\n";
    }
}

void AdminPortal::manageCourses()
{
    std::cout << "\n=== Course Catalog Management ===\n";
    std::cout << "1. View All Courses\n";
    std::cout << "2. Add New Course\n";
    std::cout << "3. View Course Details\n";
    std::cout << "4. Search Courses\n";
    std::cout << "5. Back to Main Menu\n";
    std::cout << "\nEnter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice)
    {
    case 1:
    {
        auto courses = courseCatalog->getAllCourses();
        std::cout << "\n=== All Courses (" << courses.size() << ") ===\n";
        std::cout << std::left << std::setw(12) << "Code"
                  << std::setw(40) << "Name"
                  << std::setw(10) << "Credits"
                  << std::setw(10) << "Type" << "\n";
        std::cout << std::string(72, '-') << "\n";

        for (const auto &course : courses)
        {
            std::cout << std::left << std::setw(12) << course.courseCode
                      << std::setw(40) << course.courseName
                      << std::setw(10) << course.credits
                      << std::setw(10) << course.courseType << "\n";
        }
        break;
    }
    case 2:
    {
        std::cout << "\n=== Add New Course ===\n";
        CourseInfo course;

        std::cout << "Course Code: ";
        std::getline(std::cin, course.courseCode);

        std::cout << "Course Name: ";
        std::getline(std::cin, course.courseName);

        std::cout << "Credits: ";
        std::cin >> course.credits;
        std::cin.ignore();

        std::cout << "Instructor: ";
        std::getline(std::cin, course.instructor);

        std::cout << "Type (Regular/Capstone/Research): ";
        std::getline(std::cin, course.courseType);

        std::cout << "Eligible Levels (BTech MTech PhD, space-separated): ";
        std::string levels;
        std::getline(std::cin, levels);
        std::istringstream iss(levels);
        std::string level;
        while (iss >> level)
        {
            course.eligibleLevels.insert(level);
        }

        courseCatalog->addCourse(course);
        std::cout << "\n=> Course added successfully!\n";
        break;
    }
    case 3:
    {
        std::cout << "\nEnter Course Code: ";
        std::string code;
        std::getline(std::cin, code);
        code = toUpperCase(code); // Convert to uppercase
        courseCatalog->displayCourse(code);
        break;
    }
    case 4:
    {
        std::cout << "\nEnter search query: ";
        std::string query;
        std::getline(std::cin, query);
        query = toUpperCase(query); // Convert to uppercase

        auto results = courseCatalog->searchCourses(query);
        std::cout << "\nFound " << results.size() << " courses:\n";
        for (const auto &course : results)
        {
            std::cout << course.courseCode << " - " << course.courseName << "\n";
        }
        break;
    }
    }
}

void AdminPortal::viewBackups()
{
    std::cout << "\n=== Database Backups ===\n";
    auto backups = Database::getBackupInfo();

    for (const auto &backup : backups)
    {
        std::cout << backup << "\n";
    }
}

void AdminPortal::restoreBackup()
{
    std::cout << "\n=== Restore from Backup ===\n";
    std::cout << "Enter backup number (1 or 2): ";

    int backupNum;
    std::cin >> backupNum;
    std::cin.ignore();

    std::cout << "Are you sure you want to restore from backup "
              << backupNum << "? (yes/no): ";
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm == "yes")
    {
        if (Database::restoreFromBackup(backupNum))
        {
            std::cout << "\n=> Database restored successfully from backup "
                      << backupNum << ".\n";
            std::cout << "  Please restart the application to load restored data.\n";
        }
        else
        {
            std::cout << "\nX Failed to restore backup.\n";
        }
    }
    else
    {
        std::cout << "\nRestore cancelled.\n";
    }
}

void AdminPortal::exportData()
{
    std::cout << "\n=== Export Data to CSV ===\n";
    std::cout << "Enter output filename: ";

    std::string filename;
    std::getline(std::cin, filename);

    try
    {
        Database::exportToCSV(filename);
        std::cout << "\n=> Data exported successfully to " << filename << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

void AdminPortal::viewStatistics()
{
    std::cout << "\n=== System Statistics ===\n";

    auto students = erpSystem->getAllStudents();

    std::cout << "Total Students: " << students.size() << "\n";

    // Count by level
    std::map<std::string, int> levelCount;
    std::map<std::string, int> branchCount;

    for (const auto &student : students)
    {
        // Convert StudentLevel enum to string
        std::string levelStr;
        switch (student->getLevel())
        {
        case StudentLevel::BTECH:
            levelStr = "BTECH";
            break;
        case StudentLevel::MTECH:
            levelStr = "MTECH";
            break;
        case StudentLevel::PHD:
            levelStr = "PHD";
            break;
        case StudentLevel::DUAL_DEGREE:
            levelStr = "DUAL_DEGREE";
            break;
        }

        levelCount[levelStr]++;
        branchCount[student->getBranch().getBranchName()]++;
    }

    std::cout << "\nStudents by Level:\n";
    for (const auto &pair : levelCount)
    {
        std::cout << "  " << pair.first << ": " << pair.second << "\n";
    }

    std::cout << "\nStudents by Branch:\n";
    for (const auto &pair : branchCount)
    {
        std::cout << "  " << pair.first << ": " << pair.second << "\n";
    }

    std::cout << "\nTotal Courses in Catalog: "
              << courseCatalog->getAllCourses().size() << "\n";

    std::cout << semesterManager->getStatus();
}

void AdminPortal::changeOwnPassword()
{
    std::cout << "\n=== Change Password ===\n";
    std::cout << "Enter current password: ";

    std::string oldPassword, newPassword, confirmPassword;
    std::getline(std::cin, oldPassword);

    std::cout << "Enter new password: ";
    std::getline(std::cin, newPassword);

    std::cout << "Confirm new password: ";
    std::getline(std::cin, confirmPassword);

    if (newPassword != confirmPassword)
    {
        std::cout << "\nX Passwords do not match.\n";
        return;
    }

    if (Auth::changePassword(adminUsername, oldPassword, newPassword, UserType::ADMIN))
    {
        std::cout << "\n=> Password changed successfully!\n";
    }
    else
    {
        std::cout << "\nX Failed to change password. Check your current password.\n";
    }
}

void AdminPortal::viewStudentsInsertionOrder()
{
    std::cout << "\n";
    std::cout << "=================================================================\n";
    std::cout << "   Students in Insertion Order\n";
    std::cout << "   (Shows the order in which students were added to the system)\n";
    std::cout << "=================================================================\n";
    std::cout << "\nUsing InsertionOrderIterator to traverse enrollment records.\n\n";

    try
    {
        int count = 0;
        std::cout << std::left << std::setw(5) << "#"
                  << std::setw(16) << "Roll No"
                  << std::setw(30) << "Name"
                  << std::setw(45) << "Branch"
                  << std::right << std::setw(6) << "CGPA" << "\n";
        std::cout << std::string(102, '-') << "\n";

        for (auto it = erpSystem->beginInsertionOrder(); it != erpSystem->endInsertionOrder(); ++it)
        {
            auto student = *it;
            count++;
            std::cout << std::left << std::setw(5) << count
                      << std::setw(16) << student->getRollNumber()
                      << std::setw(30) << student->getName()
                      << std::setw(45) << student->getBranch().getBranchName()
                      << std::right << std::setw(6) << std::fixed << std::setprecision(2)
                      << student->getLoadedCGPA() << "\n";

            // Show first 50 only, then ask to continue
            if (count % 50 == 0)
            {
                std::cout << "\nDisplayed " << count << " students. Press Enter to continue (or Ctrl+C to stop)...";
                std::cin.get();
            }
        }

        std::cout << "\n=> Total students displayed: " << count << " (in insertion order)\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

void AdminPortal::viewStudentsSortedOrder()
{
    std::cout << "\n";
    std::cout << "=================================================================\n";
    std::cout << "   Students in Sorted Order\n";
    std::cout << "   (Displays students after sorting by name/roll/CGPA)\n";
    std::cout << "=================================================================\n";
    std::cout << "\nUsing SortedOrderIterator to traverse sorted records.\n";
    std::cout << "Note: You must run the Sort option (Menu 9) before using this.\n\n";

    try
    {
        if (!erpSystem->getIsSorted())
        {
            std::cout << "\nX Students are not sorted yet.\n";
            std::cout << "   Please use Menu Option 9 to sort students first.\n";
            return;
        }

        int count = 0;
        std::cout << std::left << std::setw(5) << "#"
                  << std::setw(16) << "Roll No"
                  << std::setw(30) << "Name"
                  << std::setw(45) << "Branch"
                  << std::right << std::setw(6) << "CGPA" << "\n";
        std::cout << std::string(102, '-') << "\n";

        for (auto it = erpSystem->beginSorted(); it != erpSystem->endSorted(); ++it)
        {
            auto student = *it;
            count++;
            std::cout << std::left << std::setw(5) << count
                      << std::setw(16) << student->getRollNumber()
                      << std::setw(30) << student->getName()
                      << std::setw(45) << student->getBranch().getBranchName()
                      << std::right << std::setw(6) << std::fixed << std::setprecision(2)
                      << student->getLoadedCGPA() << "\n";

            // Show first 50 only, then ask to continue
            if (count % 50 == 0)
            {
                std::cout << "\nDisplayed " << count << " students. Press Enter to continue (or Ctrl+C to stop)...";
                std::cin.get();
            }
        }

        std::cout << "\n=> Total students displayed: " << count << " (in sorted order)\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

void AdminPortal::findStudentsByGrade()
{
    std::cout << "\n";
    std::cout << "=================================================================\n";
    std::cout << "   Find Students by Grade in a Course\n";
    std::cout << "=================================================================\n";
    std::cout << "\nThis search uses a grade index for fast O(log n) lookups.\n";
    std::cout << "Perfect for placement queries like 'Find all A-grade students'.\n\n";

    std::cout << "Enter Course Code (e.g., CSE101, CSE201, MAT101, ECE101): ";
    std::string courseCode;
    std::getline(std::cin, courseCode);
    courseCode = toUpperCase(courseCode);

    std::cout << "Enter Minimum Grade Points (e.g., 9.0, 8.0, 7.0): ";
    double minGrade;
    if (!(std::cin >> minGrade))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nError: Invalid input. Please enter a number.\n";
        return;
    }
    std::cin.ignore();

    try
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        auto results = erpSystem->findStudentsWithGrade(courseCode, minGrade);
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        if (results.empty())
        {
            std::cout << "\nNo students found with grade >= " << std::fixed << std::setprecision(1)
                      << minGrade << " in course " << courseCode << ".\n\n";
            std::cout << "Possible reasons:\n";
            std::cout << "  - No students have completed this course yet\n";
            std::cout << "  - No students achieved the minimum grade threshold\n";
            std::cout << "  - The course code might be incorrect (check spelling)\n\n";
            std::cout << "Hint: Try popular courses like CSE101, CSE201, MAT101, ECE101\n";
            return;
        }

        std::cout << "\nSearch completed in " << duration << " microseconds";
        std::cout << " (demonstrates O(log n) efficiency)\n";
        std::cout << "\nFound " << results.size() << " student(s) with grade >= "
                  << std::fixed << std::setprecision(1) << minGrade
                  << " in " << courseCode << ":\n\n";

        std::cout << std::left << std::setw(4) << "No."
                  << std::setw(18) << "Roll Number"
                  << std::setw(28) << "Name"
                  << std::setw(10) << "Grade"
                  << std::setw(15) << "Branch" << "\n";
        std::cout << std::string(75, '-') << "\n";

        int count = 1;
        for (const auto &student : results)
        {
            auto previousCourses = student->getPreviousCourses();
            auto previousGrades = student->getPreviousGrades();

            std::string gradeStr = "N/A";
            for (size_t i = 0; i < previousCourses.size(); ++i)
            {
                if (previousCourses[i].getCourseCode() == courseCode && i < previousGrades.size())
                {
                    double pts = previousGrades[i].getGradePoint();
                    std::string letter = previousGrades[i].getLetterGrade();
                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(1) << pts;
                    gradeStr = oss.str() + " (" + letter + ")";
                    break;
                }
            }

            std::cout << std::left << std::setw(4) << count++
                      << std::setw(18) << student->getRollNumber()
                      << std::setw(28) << student->getName()
                      << std::setw(10) << gradeStr
                      << std::setw(15) << student->getBranch().getBranchName() << "\n";
        }

        std::cout << "\n"
                  << std::string(75, '-') << "\n";
        std::cout << "Total: " << results.size() << " student(s) qualified\n";
        std::cout << "\nThis fast lookup is useful for:\n";
        std::cout << "  - Placement companies screening candidates\n";
        std::cout << "  - Merit list generation\n";
        std::cout << "  - Scholarship eligibility checks\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nError: " << e.what() << "\n";
    }
}

void AdminPortal::rebuildGradeIndex()
{
    std::cout << "\n";
    std::cout << "=================================================================\n";
    std::cout << "   Rebuild Grade Index\n";
    std::cout << "=================================================================\n";
    std::cout << "\nThis rebuilds the internal index used for fast grade searches.\n";
    std::cout << "Useful after bulk grade updates or importing new data.\n\n";

    std::cout << "Do you want to rebuild the index now? (yes/no): ";
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm != "yes" && confirm != "YES")
    {
        std::cout << "\nCancelled.\n";
        return;
    }

    try
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        erpSystem->rebuildGradeIndex();
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

        std::cout << "\nGrade index rebuilt successfully in " << duration << " ms.\n";
        std::cout << "Grade searches (Menu Option 27) are now up-to-date.\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nError: " << e.what() << "\n";
    }
}

void AdminPortal::viewPendingCourseRequests()
{
    std::cout << "\n=== Pending Course Registration Requests ===\n";

    try
    {
        auto pendingRequests = erpSystem->getPendingRequests();

        if (pendingRequests.empty())
        {
            std::cout << "\nNo pending course registration requests found.\n";
            return;
        }

        // Filter only pending requests
        std::vector<PendingCourseRequest<std::string, std::string>> pending;
        for (const auto &req : pendingRequests)
        {
            if (req.getStatus() == RequestStatus::PENDING)
            {
                pending.push_back(req);
            }
        }

        if (pending.empty())
        {
            std::cout << "\nNo pending requests. All requests have been processed.\n";
            return;
        }

        std::cout << "\nTotal Pending Requests: " << pending.size() << "\n\n";
        std::cout << std::left << std::setw(8) << "Req ID"
                  << std::setw(15) << "Roll Number"
                  << std::setw(25) << "Student Name"
                  << std::setw(12) << "Course"
                  << std::setw(20) << "Request Date" << "\n";
        std::cout << std::string(80, '-') << "\n";

        for (const auto &request : pending)
        {
            std::cout << std::left << std::setw(8) << request.getRequestId()
                      << std::setw(15) << request.getRollNumber()
                      << std::setw(25) << request.getStudentName()
                      << std::setw(12) << request.getCourseCode()
                      << std::setw(20) << request.getRequestTimeString() << "\n";
        }

        std::cout << "\n=> Use option 12 to approve or reject these requests.\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

void AdminPortal::approveCourseRequest()
{
    std::cout << "\n=== Approve/Reject Course Registration Request ===\n";

    // First, show pending requests
    try
    {
        auto pendingRequests = erpSystem->getPendingRequests();

        if (pendingRequests.empty())
        {
            std::cout << "\nNo pending course registration requests found.\n";
            return;
        }

        // Filter only pending requests
        std::vector<PendingCourseRequest<std::string, std::string>> pending;
        for (const auto &req : pendingRequests)
        {
            if (req.getStatus() == RequestStatus::PENDING)
            {
                pending.push_back(req);
            }
        }

        if (pending.empty())
        {
            std::cout << "\nNo pending requests. All requests have been processed.\n";
            return;
        }

        std::cout << "\nPending Requests:\n";
        std::cout << std::left << std::setw(8) << "Req ID"
                  << std::setw(15) << "Roll Number"
                  << std::setw(25) << "Student Name"
                  << std::setw(12) << "Course" << "\n";
        std::cout << std::string(60, '-') << "\n";

        for (const auto &request : pending)
        {
            std::cout << std::left << std::setw(8) << request.getRequestId()
                      << std::setw(15) << request.getRollNumber()
                      << std::setw(25) << request.getStudentName()
                      << std::setw(12) << request.getCourseCode() << "\n";
        }

        std::cout << "\nEnter Request ID to process (0 to cancel): ";
        int requestId;
        std::cin >> requestId;
        std::cin.ignore();

        if (requestId == 0)
        {
            std::cout << "\nOperation cancelled.\n";
            return;
        }

        // Find the request
        auto *request = erpSystem->findPendingRequest(requestId);
        if (!request)
        {
            std::cout << "\nX Request ID not found.\n";
            return;
        }

        if (request->getStatus() != RequestStatus::PENDING)
        {
            std::cout << "\nX This request has already been processed.\n";
            std::cout << "  Status: " << requestStatusToString(request->getStatus()) << "\n";
            return;
        }

        // Display request details
        std::cout << "\n--- Request Details ---\n";
        request->displayInfo();

        std::cout << "\n1. Approve\n";
        std::cout << "2. Reject\n";
        std::cout << "3. Cancel\n";
        std::cout << "Enter your choice: ";

        int action;
        std::cin >> action;
        std::cin.ignore();

        if (action == 3)
        {
            std::cout << "\nOperation cancelled.\n";
            return;
        }

        std::cout << "Enter remarks (optional, press Enter to skip): ";
        std::string remarks;
        std::getline(std::cin, remarks);

        if (action == 1)
        {
            // Approve - add course to student's enrollment
            if (erpSystem->approvePendingRequest(requestId, adminUsername, remarks))
            {
                // Now actually add the course to the student
                auto student = erpSystem->findStudent(request->getRollNumber());
                if (student)
                {
                    // Get course details from catalog
                    if (courseCatalog->courseExists(request->getCourseCode()))
                    {
                        auto courseInfo = courseCatalog->getCourseInfo(request->getCourseCode());
                        Course<std::string> course(
                            courseInfo.courseCode,
                            courseInfo.courseName,
                            courseInfo.instructor,
                            courseInfo.credits,
                            "Monsoon 2025");

                        student->addCurrentCourse(course);

                        std::cout << "\n=> Request APPROVED successfully!\n";
                        std::cout << "  Course " << request->getCourseCode()
                                  << " has been added to student's enrollment.\n";
                    }
                    else
                    {
                        std::cout << "\nX Warning: Request approved but course not found in catalog.\n";
                    }
                }
            }
            else
            {
                std::cout << "\nX Failed to approve request.\n";
            }
        }
        else if (action == 2)
        {
            // Reject
            if (erpSystem->rejectPendingRequest(requestId, adminUsername, remarks))
            {
                std::cout << "\n=> Request REJECTED.\n";
                if (!remarks.empty())
                {
                    std::cout << "  Remarks sent to student: " << remarks << "\n";
                }
            }
            else
            {
                std::cout << "\nX Failed to reject request.\n";
            }
        }
        else
        {
            std::cout << "\nX Invalid choice.\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

void AdminPortal::viewCourseEnrollments()
{
    std::cout << "\n";
    std::cout << "=================================================================\n";
    std::cout << "   Course Enrollments - Students per Course\n";
    std::cout << "=================================================================\n\n";

    // Get all students
    auto students = erpSystem->getAllStudents();

    if (students.empty())
    {
        std::cout << "No students found in the system.\n";
        return;
    }

    // Map to store course code -> list of students
    std::map<std::string, std::vector<std::shared_ptr<Student<std::string, std::string>>>> courseEnrollments;

    // Collect all course enrollments
    for (const auto &student : students)
    {
        // Get previous courses (completed courses with grades)
        auto previousCourses = student->getPreviousCourses();
        for (const auto &course : previousCourses)
        {
            std::string courseCode = course.getCourseCode();
            courseEnrollments[courseCode].push_back(student);
        }

        // Get current courses (ongoing)
        auto currentCourses = student->getCurrentCourses();
        for (const auto &course : currentCourses)
        {
            std::string courseCode = course.getCourseCode();
            // Avoid duplicates if same course is in both
            if (std::find_if(courseEnrollments[courseCode].begin(),
                             courseEnrollments[courseCode].end(),
                             [&student](const auto &s)
                             { return s->getRollNumber() == student->getRollNumber(); }) == courseEnrollments[courseCode].end())
            {
                courseEnrollments[courseCode].push_back(student);
            }
        }
    }

    if (courseEnrollments.empty())
    {
        std::cout << "No course enrollments found.\n";
        return;
    }

    // Display summary of all courses
    std::cout << "Total Courses with Enrollments: " << courseEnrollments.size() << "\n\n";
    std::cout << std::left << std::setw(15) << "Course Code"
              << std::setw(40) << "Course Name"
              << std::setw(15) << "Students"
              << "\n";
    std::cout << "---------------------------------------------------------------------------\n";

    for (const auto &entry : courseEnrollments)
    {
        std::string courseCode = entry.first;
        size_t studentCount = entry.second.size();

        // Get course name from catalog if available
        std::string courseName = "N/A";
        if (courseCatalog->courseExists(courseCode))
        {
            auto courseInfo = courseCatalog->getCourseInfo(courseCode);
            courseName = courseInfo.courseName;
        }

        std::cout << std::left << std::setw(15) << courseCode
                  << std::setw(40) << courseName
                  << std::setw(15) << studentCount
                  << "\n";
    }

    std::cout << "---------------------------------------------------------------------------\n\n";

    // Ask which course to view or all
    std::cout << "Which course students would you like to view?\n";
    std::cout << "  - Enter course code (e.g., CSE101, MAT101)\n";
    std::cout << "  - Enter 'all' to view all courses\n";
    std::cout << "  - Press Enter to skip\n";
    std::cout << "\nYour choice: ";
    std::string choice;
    std::getline(std::cin, choice);

    // Trim whitespace
    choice.erase(0, choice.find_first_not_of(" \t\n\r"));
    choice.erase(choice.find_last_not_of(" \t\n\r") + 1);

    if (choice.empty())
    {
        return;
    }

    // Convert to uppercase for comparison
    std::string choiceUpper = choice;
    for (char &c : choiceUpper)
    {
        c = std::toupper(c);
    }

    // Check if viewing all courses
    bool viewAll = (choiceUpper == "ALL");
    std::string targetCourse = "";

    if (!viewAll)
    {
        // Check if the entered course exists
        targetCourse = choiceUpper;
        if (courseEnrollments.find(targetCourse) == courseEnrollments.end())
        {
            std::cout << "\nError: Course '" << targetCourse << "' not found or has no enrollments.\n";
            std::cout << "Please check the course code from the list above.\n";
            return;
        }
    }

    // Display students for selected course(s)
    std::cout << "\n";
    std::cout << "=================================================================\n";
    if (viewAll)
    {
        std::cout << "   Detailed Course-wise Student List (All Courses)\n";
    }
    else
    {
        std::cout << "   Student List for " << targetCourse << "\n";
    }
    std::cout << "=================================================================\n\n";

    for (const auto &entry : courseEnrollments)
    {
        std::string courseCode = entry.first;
        const auto &enrolledStudents = entry.second;

        // Skip if user requested a specific course and this isn't it
        if (!viewAll && courseCode != targetCourse)
        {
            continue;
        }

        // Get course details
        std::string courseName = "N/A";
        std::string instructor = "N/A";
        int credits = 0;
        if (courseCatalog->courseExists(courseCode))
        {
            auto courseInfo = courseCatalog->getCourseInfo(courseCode);
            courseName = courseInfo.courseName;
            instructor = courseInfo.instructor;
            credits = courseInfo.credits;
        }

        std::cout << "\n-----------------------------------------------------------------\n";
        std::cout << "Course: " << courseCode << " - " << courseName << "\n";
        std::cout << "Instructor: " << instructor << " | Credits: " << credits << "\n";
        std::cout << "Total Enrolled: " << enrolledStudents.size() << " student(s)\n";
        std::cout << "-----------------------------------------------------------------\n\n";

        // Display students in a table
        std::cout << std::left
                  << std::setw(6) << "No."
                  << std::setw(18) << "Roll Number"
                  << std::setw(30) << "Name"
                  << std::setw(12) << "Grade"
                  << std::setw(10) << "Branch"
                  << "\n";
        std::cout << "---------------------------------------------------------------------------\n";

        int count = 1;
        for (const auto &student : enrolledStudents)
        {
            // Find the grade for this course
            std::string gradeStr = "Enrolled";
            bool found = false;

            // Check in previous courses
            auto prevCourses = student->getPreviousCourses();
            auto prevGrades = student->getPreviousGrades();
            for (size_t i = 0; i < prevCourses.size(); ++i)
            {
                if (prevCourses[i].getCourseCode() == courseCode)
                {
                    double pts = prevGrades[i].getGradePoint();
                    std::string letter = prevGrades[i].getLetterGrade();
                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(1) << pts << " (" << letter << ")";
                    gradeStr = oss.str();
                    found = true;
                    break;
                }
            }

            // If not found, check current courses
            if (!found)
            {
                auto currCourses = student->getCurrentCourses();
                auto currGrades = student->getCurrentGrades();
                for (size_t i = 0; i < currCourses.size(); ++i)
                {
                    if (currCourses[i].getCourseCode() == courseCode)
                    {
                        if (i < currGrades.size())
                        {
                            double pts = currGrades[i].getGradePoint();
                            if (pts > 0.0)
                            {
                                std::string letter = currGrades[i].getLetterGrade();
                                std::ostringstream oss;
                                oss << std::fixed << std::setprecision(1) << pts << " (" << letter << ")";
                                gradeStr = oss.str();
                            }
                        }
                        break;
                    }
                }
            }

            std::cout << std::left
                      << std::setw(6) << count
                      << std::setw(18) << student->getRollNumber()
                      << std::setw(30) << student->getName()
                      << std::setw(12) << gradeStr
                      << std::setw(10) << student->getBranch().getBranchCode()
                      << "\n";
            count++;
        }

        std::cout << "---------------------------------------------------------------------------\n";
    }

    std::cout << "\n=================================================================\n";
    std::cout << "End of course enrollment report\n";
    std::cout << "=================================================================\n";
}

void AdminPortal::demoIIITIITCourseIntegration()
{
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║     IIIT-DELHI & IIT-DELHI COURSE INTEGRATION DEMO            ║\n";
    std::cout << "║     Demonstrating Template Flexibility with Different         ║\n";
    std::cout << "║     Course Code Types (String vs Integer)                     ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

    std::cout << "Background:\n";
    std::cout << "  IIIT-Delhi students can take courses from IIT-Delhi.\n";
    std::cout << "  IIIT-Delhi uses string course codes (e.g., 'CSE201', 'MTH101')\n";
    std::cout << "  IIT-Delhi uses integer course codes (e.g., 101, 201, 305)\n\n";

    std::cout << "This demo showcases:\n";
    std::cout << "  1. Templates handling both string and integer course codes\n";
    std::cout << "  2. Same student managing courses from both institutions\n";
    std::cout << "  3. Type-safe operations across different course systems\n\n";

    std::cout << "Press Enter to start demo...";
    std::cin.get();

    // Demo 1: IIIT-Delhi Student with String Course Codes
    std::cout << "\n"
              << std::string(68, '=') << "\n";
    std::cout << "DEMO 1: IIIT-Delhi Student (String Course Codes)\n";
    std::cout << std::string(68, '=') << "\n";

    Branch cse("CSE", "Computer Science and Engineering", "Engineering");
    Student<std::string, std::string> iiitStudent("Rahul Kumar", "2023CSE101", cse, StudentLevel::BTECH, 2023);

    // Add IIIT-Delhi courses with string codes
    Course<std::string> iiitCourse1("CSE201", "Data Structures", "Dr. Sharma", 4, "Monsoon 2024");
    Course<std::string> iiitCourse2("CSE202", "Object Oriented Programming", "Dr. Verma", 4, "Monsoon 2024");
    Course<std::string> iiitCourse3("MTH101", "Linear Algebra", "Prof. Gupta", 3, "Monsoon 2024");

    iiitStudent.addCurrentCourse(iiitCourse1);
    iiitStudent.addCurrentCourse(iiitCourse2);
    iiitStudent.addCurrentCourse(iiitCourse3);

    std::cout << "\n📚 Student: " << iiitStudent.getName() << " (" << iiitStudent.getRollNumber() << ")\n";
    std::cout << "🏫 Institution: IIIT-Delhi\n";
    std::cout << "📖 Enrolled IIIT-Delhi Courses:\n";
    auto iiitCourses = iiitStudent.getCurrentCourses();
    for (const auto &course : iiitCourses)
    {
        std::cout << "   • " << course.getCourseCode() << " - " << course.getCourseName()
                  << " (" << course.getCredits() << " credits)\n";
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.get();

    // Demo 2: Same Student Concept with Integer Course Codes (IIT-Delhi)
    std::cout << "\n"
              << std::string(68, '=') << "\n";
    std::cout << "DEMO 2: IIT-Delhi Student (Integer Course Codes)\n";
    std::cout << std::string(68, '=') << "\n";

    Student<std::string, int> iitStudent("Priya Singh", "2023CSE102", cse, StudentLevel::BTECH, 2023);

    // Add IIT-Delhi courses with integer codes
    Course<int> iitCourse1(101, "Introduction to Computing", "Prof. Mehta", 3, "Semester 1");
    Course<int> iitCourse2(201, "Advanced Algorithms", "Dr. Joshi", 4, "Semester 1");
    Course<int> iitCourse3(305, "Database Systems", "Prof. Reddy", 4, "Semester 1");

    iitStudent.addCurrentCourse(iitCourse1);
    iitStudent.addCurrentCourse(iitCourse2);
    iitStudent.addCurrentCourse(iitCourse3);

    std::cout << "\n📚 Student: " << iitStudent.getName() << " (" << iitStudent.getRollNumber() << ")\n";
    std::cout << "🏫 Institution: IIT-Delhi\n";
    std::cout << "📖 Enrolled IIT-Delhi Courses:\n";
    auto iitCourses = iitStudent.getCurrentCourses();
    for (const auto &course : iitCourses)
    {
        std::cout << "   • " << course.getCourseCode() << " - " << course.getCourseName()
                  << " (" << course.getCredits() << " credits)\n";
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.get();

    // Demo 3: IIIT Student taking IIT courses (Cross-institutional enrollment)
    std::cout << "\n"
              << std::string(68, '=') << "\n";
    std::cout << "DEMO 3: IIIT-Delhi Student Taking IIT-Delhi Courses\n";
    std::cout << "       (Demonstrates Template Flexibility)\n";
    std::cout << std::string(68, '=') << "\n";

    Student<std::string, int> crossStudent("Arjun Patel", "2023CSE103", cse, StudentLevel::BTECH, 2023);

    // Enroll in both IIIT (string) and IIT (int) style courses
    // But Student template is <string, int> so it handles integer course codes
    Course<int> mixCourse1(401, "Machine Learning (IIT-Delhi)", "Prof. Kumar", 4, "Winter 2024");
    Course<int> mixCourse2(505, "Computer Vision (IIT-Delhi)", "Dr. Saxena", 3, "Winter 2024");
    Course<int> mixCourse3(301, "Operating Systems (IIT-Delhi)", "Prof. Bhatia", 4, "Winter 2024");

    crossStudent.addCurrentCourse(mixCourse1);
    crossStudent.addCurrentCourse(mixCourse2);
    crossStudent.addCurrentCourse(mixCourse3);

    std::cout << "\n📚 Student: " << crossStudent.getName() << " (" << crossStudent.getRollNumber() << ")\n";
    std::cout << "🏫 Home Institution: IIIT-Delhi (Roll format: string)\n";
    std::cout << "🎓 Taking courses at: IIT-Delhi (Course codes: integer)\n";
    std::cout << "📖 Cross-Institutional Enrollment:\n";
    auto mixCourses = crossStudent.getCurrentCourses();
    for (const auto &course : mixCourses)
    {
        std::cout << "   • Course " << course.getCourseCode() << " - " << course.getCourseName()
                  << " (" << course.getCredits() << " credits)\n";
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.get();

    // Demo 4: Comparison and Template Demonstration
    std::cout << "\n"
              << std::string(68, '=') << "\n";
    std::cout << "DEMO 4: Template Type Comparison\n";
    std::cout << std::string(68, '=') << "\n";

    std::cout << "\n✓ Template Instantiations Demonstrated:\n\n";

    std::cout << "1. Student<string, string> - IIIT-Delhi student\n";
    std::cout << "   • Roll Number Type: std::string\n";
    std::cout << "   • Course Code Type: std::string\n";
    std::cout << "   • Example Roll: " << iiitStudent.getRollNumber() << "\n";
    std::cout << "   • Example Course: " << iiitCourse1.getCourseCode() << "\n\n";

    std::cout << "2. Student<string, int> - IIT-Delhi student\n";
    std::cout << "   • Roll Number Type: std::string\n";
    std::cout << "   • Course Code Type: int\n";
    std::cout << "   • Example Roll: " << iitStudent.getRollNumber() << "\n";
    std::cout << "   • Example Course: " << iitCourse1.getCourseCode() << "\n\n";

    std::cout << "3. Student<string, int> - Cross-institutional\n";
    std::cout << "   • Roll Number Type: std::string (IIIT format)\n";
    std::cout << "   • Course Code Type: int (IIT format)\n";
    std::cout << "   • Example Roll: " << crossStudent.getRollNumber() << "\n";
    std::cout << "   • Example Course: " << mixCourse1.getCourseCode() << "\n\n";

    std::cout << "Press Enter to continue...";
    std::cin.get();

    // Demo 5: Grade Assignment and CGPA Calculation
    std::cout << "\n"
              << std::string(68, '=') << "\n";
    std::cout << "DEMO 5: Grade Assignment (Works with Both Systems)\n";
    std::cout << std::string(68, '=') << "\n";

    // Assign grades to IIIT student
    Grade g1(9.0); // A
    Grade g2(8.5); // A-
    Grade g3(9.5); // A+

    iiitStudent.updateGrade(iiitCourse1.getCourseCode(), g1);
    iiitStudent.updateGrade(iiitCourse2.getCourseCode(), g2);
    iiitStudent.updateGrade(iiitCourse3.getCourseCode(), g3);

    std::cout << "\n📊 " << iiitStudent.getName() << " (IIIT-Delhi) - Grades:\n";
    auto iiitGrades = iiitStudent.getCurrentGrades();
    for (size_t i = 0; i < iiitCourses.size(); ++i)
    {
        std::cout << "   • " << iiitCourses[i].getCourseCode() << ": "
                  << iiitGrades[i].getLetterGrade() << " (GP: "
                  << std::fixed << std::setprecision(1) << iiitGrades[i].getGradePoint() << ")\n";
    }
    std::cout << "   CGPA: " << std::fixed << std::setprecision(2) << iiitStudent.calculateCGPA() << "\n";

    // Assign grades to IIT student
    Grade g4(8.0); // B+
    Grade g5(9.0); // A
    Grade g6(7.5); // B

    iitStudent.updateGrade(iitCourse1.getCourseCode(), g4);
    iitStudent.updateGrade(iitCourse2.getCourseCode(), g5);
    iitStudent.updateGrade(iitCourse3.getCourseCode(), g6);

    std::cout << "\n📊 " << iitStudent.getName() << " (IIT-Delhi) - Grades:\n";
    auto iitGrades = iitStudent.getCurrentGrades();
    for (size_t i = 0; i < iitCourses.size(); ++i)
    {
        std::cout << "   • Course " << iitCourses[i].getCourseCode() << ": "
                  << iitGrades[i].getLetterGrade() << " (GP: "
                  << std::fixed << std::setprecision(1) << iitGrades[i].getGradePoint() << ")\n";
    }
    std::cout << "   CGPA: " << std::fixed << std::setprecision(2) << iitStudent.calculateCGPA() << "\n";

    std::cout << "\nPress Enter to continue...";
    std::cin.get();

    // Summary
    std::cout << "\n"
              << std::string(68, '=') << "\n";
    std::cout << "SUMMARY: Template Benefits Demonstrated\n";
    std::cout << std::string(68, '=') << "\n\n";

    std::cout << "✓ Type Safety: Compile-time type checking prevents mixing incompatible types\n";
    std::cout << "✓ Flexibility: Same Student class handles multiple course code formats\n";
    std::cout << "✓ Reusability: No code duplication for different institutional systems\n";
    std::cout << "✓ Scalability: Easy to add new institutions with different formats\n";
    std::cout << "✓ Performance: Zero runtime overhead compared to runtime polymorphism\n\n";

    std::cout << "Real-world Application:\n";
    std::cout << "  • IIIT-Delhi students can seamlessly take IIT-Delhi courses\n";
    std::cout << "  • System tracks courses from both institutions correctly\n";
    std::cout << "  • Type system ensures data integrity across institutions\n";
    std::cout << "  • Same codebase serves multiple course numbering systems\n\n";

    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║            Demo Complete - Press Enter to return              ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    std::cin.get();
}
