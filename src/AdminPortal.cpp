#include "AdminPortal.h"
#include "CSVHandler.h"
#include "Database.h"
#include "SortingManager.h"
#include "Colors.h"
#include "OutputFormatter.h"
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
    : __admin_username(username), erpSystem(erp), courseCatalog(catalog),
      semesterManager(semMgr)
{
}

void AdminPortal::display_menu()
{
    OutputFormatter::print_header("ADMIN PORTAL - Apex University ERP");

    std::cout << Colors::DIM << "  Logged in as: " << Colors::BOLD << Colors::CYAN
              << __admin_username << Colors::RESET << "\n\n";

    // student management section
    std::cout << Colors::YELLOW << "  STUDENT MANAGEMENT" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(1, "Add Single Student", Colors::GREEN);
    OutputFormatter::print_menu_item(2, "Add Students from CSV (Bulk)", Colors::GREEN);
    OutputFormatter::print_menu_item(3, "View All Students", Colors::CYAN);
    OutputFormatter::print_menu_item(4, "Search Student", Colors::CYAN);
    OutputFormatter::print_menu_item(5, "Update Student Marks", Colors::CYAN);
    OutputFormatter::print_menu_item(6, "Delete Single Student", Colors::RED);
    OutputFormatter::print_menu_item(7, "Delete Students from CSV (Bulk)", Colors::RED);
    OutputFormatter::print_menu_item(8, "Delete ALL Students (DANGEROUS)", Colors::BRIGHT_RED);
    OutputFormatter::print_menu_item(9, "Sort Students (By Name/Roll/CGPA + Threading)", Colors::MAGENTA);
    OutputFormatter::print_menu_item(10, "Create Student Account (Generate Credentials)", Colors::GREEN);
    OutputFormatter::print_menu_item(11, "Bulk Create All Student Accounts (abc123)", Colors::GREEN);

    OutputFormatter::print_menu_separator();

    // course management section
    std::cout << Colors::YELLOW << "  COURSE MANAGEMENT" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(12, "Manage Course Catalog", Colors::CYAN);
    OutputFormatter::print_menu_item(13, "View Course Statistics", Colors::CYAN);

    OutputFormatter::print_menu_separator();

    // course registration approval
    std::cout << Colors::YELLOW << "  COURSE REGISTRATION APPROVAL" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(14, "View Pending Course Requests", Colors::CYAN);
    OutputFormatter::print_menu_item(15, "Approve/Reject Course Requests", Colors::YELLOW);

    OutputFormatter::print_menu_separator();

    // semester management
    std::cout << Colors::YELLOW << "  SEMESTER MANAGEMENT" << Colors::RESET << "\n";

    std::string regStatus = semesterManager->is_course_add_enabled() ? (Colors::GREEN + "OPEN" + Colors::RESET) : (Colors::RED + "CLOSED" + Colors::RESET);
    std::cout << "    " << Colors::CYAN << "[16]" << Colors::RESET
              << "  Toggle Course Registration (Currently: " << regStatus << ")\n";

    std::string dropStatus = semesterManager->is_course_drop_enabled() ? (Colors::GREEN + "ENABLED" + Colors::RESET) : (Colors::RED + "DISABLED" + Colors::RESET);
    std::cout << "    " << Colors::CYAN << "[17]" << Colors::RESET
              << "  Toggle Course Drop (Currently: " << dropStatus << ")\n";

    OutputFormatter::print_menu_item(18, "View Semester Manager Status", Colors::CYAN);

    OutputFormatter::print_menu_separator();

    // __database & backup
    std::cout << Colors::YELLOW << "  DATABASE & BACKUP" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(19, "View Backup Information", Colors::CYAN);
    OutputFormatter::print_menu_item(20, "Restore from Backup", Colors::YELLOW);
    OutputFormatter::print_menu_item(21, "Export Data to CSV", Colors::GREEN);

    OutputFormatter::print_menu_separator();

    // course enrollment & analysis
    std::cout << Colors::YELLOW << "  COURSE ENROLLMENT & ANALYSIS" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(22, "View Course Enrollments (Students per Course)", Colors::CYAN);
    OutputFormatter::print_menu_item(23, "Apex-Vertex Course Integration Demo (Template Demo)", Colors::MAGENTA);

    OutputFormatter::print_menu_separator();

    // iterators & grade search
    std::cout << Colors::YELLOW << "  ITERATORS & GRADE SEARCH (Assignment Features)" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(25, "View Students (Insertion Order)", Colors::BLUE);
    OutputFormatter::print_menu_item(26, "View Students (Sorted Order)", Colors::BLUE);
    OutputFormatter::print_menu_item(27, "Find Students by Grade (Quick Search)", Colors::MAGENTA);
    OutputFormatter::print_menu_item(28, "Rebuild Grade Index", Colors::YELLOW);

    OutputFormatter::print_menu_separator();

    // system
    std::cout << Colors::YELLOW << "  SYSTEM" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(29, "View System Statistics", Colors::CYAN);
    OutputFormatter::print_menu_item(30, "Change Password", Colors::YELLOW);
    OutputFormatter::print_menu_item(31, "Logout", Colors::DIM);

    std::cout << "\n";
    OutputFormatter::print_double_line(80);
    std::cout << Colors::CYAN << "Enter your choice: " << Colors::RESET;
}

void AdminPortal::run()
{
    int choice;

    while (true)
    {
        display_menu();

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << Colors::RED << "[X] Invalid input. Please enter a number." << Colors::RESET << "\n";
            continue;
        }
        std::cin.ignore();

        switch (choice)
        {
        case 1:
            add_single_student();
            break;
        case 2:
            add_bulk_students();
            break;
        case 3:
            view_all_students();
            break;
        case 4:
            search_student();
            break;
        case 5:
            update_student_marks();
            break;
        case 6:
            delete_student();
            break;
        case 7:
            delete_bulk_students();
            break;
        case 8:
            delete_all_students();
            break;
        case 9:
            sort_students_menu();
            break;
        case 10:
            create_student_account();
            break;
        case 11:
            bulk_create_student_accounts();
            break;
        case 12:
            manage_courses();
            break;
        case 13:
            std::cout << "\nCourse statistics feature - coming soon!\n";
            break;
        case 14:
            view_pending_course_requests();
            break;
        case 15:
            approve_course_request();
            break;
        case 16:
            semesterManager->set_course_add_enabled(!semesterManager->is_course_add_enabled());
            break;
        case 17:
            semesterManager->set_course_drop_enabled(!semesterManager->is_course_drop_enabled());
            break;
        case 18:
            std::cout << semesterManager->get_status();
            break;
        case 19:
            view_backups();
            break;
        case 20:
            restore_backup();
            break;
        case 21:
            export_data();
            break;
        case 22:
            view_course_enrollments();
            break;
        case 23:
            demo_apex_vertex_course_integration();
            break;
        case 24:
            change_own_password();
            break;
        case 25:
            view_students_insertion_order();
            break;
        case 26:
            view_students_sorted_order();
            break;
        case 27:
            find_students_by_grade();
            break;
        case 28:
            rebuild_grade_index();
            break;
        case 29:
            view_statistics();
            break;
        case 30:
            change_own_password();
            break;
        case 31:
            std::cout << "\nLogging out from admin portal...\n";
            return;
        case 32:
            std::cout << "\nLogging out from admin portal...\n";
            return;
        default:
            std::cout << "\n"
                      << Colors::RED << "[X] Invalid choice. Please try again." << Colors::RESET << "\n";
        }

        std::cout << "\n"
                  << Colors::DIM << "Press " << Colors::BOLD << "[Enter]"
                  << Colors::RESET << Colors::DIM << " to continue..." << Colors::RESET;
        std::cin.get();
    }
}

void AdminPortal::add_single_student()
{
    OutputFormatter::print_header("ADD SINGLE STUDENT");

    std::string rollNumber, name, branchCode, level;
    int startYear;

    std::cout << Colors::CYAN << "Enter Roll Number (max 15 chars, no spaces): " << Colors::RESET;
    std::getline(std::cin, rollNumber);
    rollNumber = toUpperCase(rollNumber); // Convert to uppercase

    std::cout << Colors::CYAN << "Enter Name (FirstName LastName format): " << Colors::RESET;
    std::getline(std::cin, name);
    name = toUpperCase(name); // Convert to uppercase

    std::cout << Colors::CYAN << "Enter Level (BTECH/MTECH/PHD/DUAL_DEGREE): " << Colors::RESET;
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
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Invalid level. Use BTECH/MTECH/PHD/DUAL_DEGREE.\n";
        return;
    }

    // Display branches based on level
    std::cout << "\n"
              << Colors::CYAN << "> Available Branches for " << level << Colors::RESET << "\n";
    OutputFormatter::print_line();

    if (studentLevel == StudentLevel::BTECH || studentLevel == StudentLevel::DUAL_DEGREE)
    {
        std::cout << Colors::DIM << "1.  " << Colors::RESET << Colors::BOLD << "CSE" << Colors::RESET << "     - Computer Science and Engineering\n";
        std::cout << Colors::DIM << "2.  " << Colors::RESET << Colors::BOLD << "ECE" << Colors::RESET << "     - Electronics and Communications Engineering\n";
        std::cout << Colors::DIM << "3.  " << Colors::RESET << Colors::BOLD << "CSAI" << Colors::RESET << "    - Computer Science & Artificial Intelligence\n";
        std::cout << Colors::DIM << "4.  " << Colors::RESET << Colors::BOLD << "CSAM" << Colors::RESET << "    - Computer Science and Applied Mathematics\n";
        std::cout << Colors::DIM << "5.  " << Colors::RESET << Colors::BOLD << "CSD" << Colors::RESET << "     - Computer Science and Design\n";
        std::cout << Colors::DIM << "6.  " << Colors::RESET << Colors::BOLD << "CSB" << Colors::RESET << "     - Computer Science and Biosciences\n";
        std::cout << Colors::DIM << "7.  " << Colors::RESET << Colors::BOLD << "CSSS" << Colors::RESET << "    - Computer Science and Social Sciences\n";
        std::cout << Colors::DIM << "8.  " << Colors::RESET << Colors::BOLD << "EVE" << Colors::RESET << "     - Electronics & VLSI Engineering\n";
        std::cout << Colors::DIM << "9.  " << Colors::RESET << Colors::BOLD << "CSEcon" << Colors::RESET << "  - Computer Science and Economics\n";
    }
    else if (studentLevel == StudentLevel::MTECH)
    {
        std::cout << Colors::DIM << "1. " << Colors::RESET << Colors::BOLD << "MTCSE" << Colors::RESET << " - M.Tech Computer Science & Engineering\n";
        std::cout << Colors::DIM << "   Specializations: " << Colors::RESET << "AI, Data Engineering, Information Security, Mobile Computing\n";
        std::cout << Colors::DIM << "2. " << Colors::RESET << Colors::BOLD << "MTECE" << Colors::RESET << " - M.Tech Electronics & Communications Engineering\n";
        std::cout << Colors::DIM << "   Specializations: " << Colors::RESET << "VLSI, Embedded Systems, Communication, Machine Learning\n";
        std::cout << Colors::DIM << "3. " << Colors::RESET << Colors::BOLD << "MTCB" << Colors::RESET << "  - M.Tech Computational Biology\n";
    }
    else if (studentLevel == StudentLevel::PHD)
    {
        std::cout << Colors::DIM << "1. " << Colors::RESET << Colors::BOLD << "PHDCSE" << Colors::RESET << "   - PhD Computer Science & Engineering\n";
        std::cout << Colors::DIM << "2. " << Colors::RESET << Colors::BOLD << "PHDECE" << Colors::RESET << "   - PhD Electronics & Communications Engineering\n";
        std::cout << Colors::DIM << "3. " << Colors::RESET << Colors::BOLD << "PHDCB" << Colors::RESET << "    - PhD Computational Biology\n";
        std::cout << Colors::DIM << "4. " << Colors::RESET << Colors::BOLD << "PHDMATH" << Colors::RESET << "  - PhD Mathematics\n";
        std::cout << Colors::DIM << "5. " << Colors::RESET << Colors::BOLD << "PHDHCD" << Colors::RESET << "   - PhD Human-Centred & Design\n";
        std::cout << Colors::DIM << "6. " << Colors::RESET << Colors::BOLD << "PHDSSH" << Colors::RESET << "   - PhD Social Sciences & Humanities\n";
    }

    std::cout << "\n"
              << Colors::CYAN << "Enter Branch Code: " << Colors::RESET;
    std::getline(std::cin, branchCode);
    branchCode = toUpperCase(branchCode); // Convert to uppercase

    std::cout << Colors::CYAN << "Enter Start Year: " << Colors::RESET;
    std::cin >> startYear;
    std::cin.ignore();

    try
    {
        // Create branch based on code
        Branch branch(branchCode, branchCode, "Department"); // Will be replaced by proper mapping
        auto student = std::make_shared<Student<std::string, std::string>>(name, rollNumber, branch, studentLevel, startYear);
        erpSystem->add_student(student);

        // Save insertion order to file
        erpSystem->save_insertion_order_to_file();

        std::cout << Colors::GREEN << "\n[OK] Student added successfully." << Colors::RESET << "\n";
        std::cout << "  " << Colors::BOLD << "Roll Number:" << Colors::RESET << " " << rollNumber << "\n";
        std::cout << "  " << Colors::BOLD << "Name:" << Colors::RESET << " " << name << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << e.what() << "\n";
    }
}

void AdminPortal::add_bulk_students()
{
    OutputFormatter::print_header("ADD STUDENTS FROM CSV");
    std::cout << Colors::CYAN << "Enter CSV filename" << Colors::RESET << " (default: demo_students.csv): ";

    std::string filename;
    std::getline(std::cin, filename);

    // Use default if user pressed Enter without typing
    if (filename.empty())
    {
        filename = "demo_students.csv";
        std::cout << Colors::DIM << "[i] Using default file: demo_students.csv" << Colors::RESET << "\n";
    }

    try
    {
        CSVHandler<std::string, std::string> csvHandler(filename);
        auto students = csvHandler.read_students();

        std::cout << Colors::CYAN << "\n[i] Found " << Colors::BOLD << students.size() << Colors::RESET << Colors::CYAN << " students in CSV." << Colors::RESET << "\n";
        std::cout << Colors::YELLOW << " Adding to system..." << Colors::RESET << "\n";

        int added = 0;
        for (const auto &student : students)
        {
            try
            {
                erpSystem->add_student(student);
                added++;
            }
            catch (const std::exception &e)
            {
                std::cout << Colors::YELLOW << "[!] Warning: " << Colors::RESET << "Could not add student "
                          << student->get_roll_number() << ": " << e.what() << "\n";
            }
        }

        std::cout << Colors::GREEN << "\n[OK] Successfully added " << Colors::BOLD << added << Colors::RESET << Colors::GREEN
                  << " out of " << students.size() << " students." << Colors::RESET << "\n";

        // Save insertion order to file after bulk add
        if (added > 0)
        {
            erpSystem->save_insertion_order_to_file();
        }
    }
    catch (const std::exception &e)
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << e.what() << "\n";
    }
}

// =============================================================================
// COMPREHENSIVE SORTING SYSTEM - By Name, Roll Number, or CGPA
// =============================================================================

void AdminPortal::sort_students_menu()
{
    OutputFormatter::print_header("STUDENT SORTING SYSTEM");
    std::cout << "\n"
              << Colors::CYAN << "> Select Sorting Criteria:" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(1, "Sort by Name (Dictionary Order)", Colors::CYAN);
    OutputFormatter::print_menu_item(2, "Sort by Roll Number", Colors::CYAN);
    OutputFormatter::print_menu_item(3, "Sort by CGPA (Descending)", Colors::CYAN);
    OutputFormatter::print_menu_item(0, "Back to Main Menu", Colors::DIM);
    OutputFormatter::print_line();
    std::cout << "Enter your choice: ";

    int criteria;
    if (!(std::cin >> criteria))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << Colors::RED << "[X] Invalid input." << Colors::RESET << "\n";
        return;
    }
    std::cin.ignore();

    if (criteria == 0)
        return;

    if (criteria < 1 || criteria > 3)
    {
        std::cout << Colors::RED << "[X] Invalid choice." << Colors::RESET << "\n";
        return;
    }

    // Now ask for threading option
    std::cout << "\n"
              << Colors::MAGENTA << "> Select Threading Mode:" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(1, "Single-threaded (Sequential)", Colors::CYAN);
    OutputFormatter::print_menu_item(2, "Multi-threaded (Parallel - 2 threads)", Colors::MAGENTA);
    OutputFormatter::print_line();
    std::cout << "Enter your choice: ";

    int threadChoice;
    if (!(std::cin >> threadChoice))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << Colors::RED << "[X] Invalid input." << Colors::RESET << "\n";
        return;
    }
    std::cin.ignore();

    if (threadChoice < 1 || threadChoice > 2)
    {
        std::cout << Colors::RED << "[X] Invalid choice." << Colors::RESET << "\n";
        return;
    }

    bool multiThreaded = (threadChoice == 2);

    // Perform the sort
    sort_students(criteria, multiThreaded);
}

void AdminPortal::sort_students(int criteria, bool multiThreaded)
{
    const char *criteriaNames[] = {"", "Name", "Roll Number", "CGPA"};
    const char *threadMode = multiThreaded ? "Multi-threaded" : "Single-threaded";

    std::string title = std::string(threadMode) + " Sort by " + criteriaNames[criteria];
    OutputFormatter::print_header(title);
    std::cout << "\n";

    try
    {
        // Load all students from memory
        auto loadStart = std::chrono::high_resolution_clock::now();
        auto allStudents = erpSystem->get_all_students();
        auto loadEnd = std::chrono::high_resolution_clock::now();
        long long loadMs = std::chrono::duration_cast<std::chrono::milliseconds>(loadEnd - loadStart).count();

        if (allStudents.empty())
        {
            std::cout << Colors::YELLOW << "[!] No students in the system to sort." << Colors::RESET << "\n";
            return;
        }

        int totalCount = allStudents.size();
        std::cout << Colors::CYAN << "[i] Loaded " << Colors::BOLD << totalCount << Colors::RESET
                  << Colors::CYAN << " students from memory in " << Colors::BOLD << loadMs << "ms"
                  << Colors::RESET << "\n";

        if (!multiThreaded)
        {
            // ========== SINGLE-THREADED SORT ==========
            auto sortStart = std::chrono::high_resolution_clock::now();

            switch (criteria)
            {
            case 1: // Sort by Name
                std::sort(allStudents.begin(), allStudents.end(),
                          [](const auto &a, const auto &b)
                          { return a->get_name() < b->get_name(); });
                break;
            case 2: // Sort by Roll Number
                std::sort(allStudents.begin(), allStudents.end(),
                          [](const auto &a, const auto &b)
                          { return a->get_roll_number() < b->get_roll_number(); });
                break;
            case 3: // Sort by CGPA (descending)
                std::sort(allStudents.begin(), allStudents.end(),
                          [](const auto &a, const auto &b)
                          { return a->get_loaded_cgpa() > b->get_loaded_cgpa(); });
                break;
            }

            auto sortEnd = std::chrono::high_resolution_clock::now();
            long long sortMs = std::chrono::duration_cast<std::chrono::milliseconds>(sortEnd - sortStart).count();

            std::cout << "Sorted " << totalCount << " students in " << sortMs << " ms\n";

            // Update ERP system with sorted list (only in-memory, not persisted)
            erpSystem->set_sorted_students(allStudents);

            std::cout << "\n--- Performance Summary ---\n";
            std::cout << "Sort time: " << sortMs << " ms\n";
            std::cout << "Note: Sorted order is in-memory only, not saved to __database.\n";
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
            erpSystem->set_sorted_students(sortedStudents);

            // Display thread performance logs
            sortingManager.displayThreadLogs();

            std::cout << "Note: Sorted order is in-memory only, not saved to __database.\n";
            std::cout << "      This preserves insertion order for the next program run.\n";
        }

        // Print first 10 students after sorting
        std::cout << "\n";
        OutputFormatter::print_double_line(80);
        std::cout << Colors::BOLD << "  First 10 Students After Sorting:" << Colors::RESET << "\n";
        OutputFormatter::print_double_line(80);
        auto sortedStudents = erpSystem->get_all_students();
        for (size_t i = 0; i < sortedStudents.size() && i < 10; ++i)
        {
            std::cout << Colors::CYAN << "  " << std::setw(3) << (i + 1) << ". " << Colors::RESET
                      << Colors::BOLD << std::setw(20) << sortedStudents[i]->get_roll_number() << Colors::RESET
                      << " | " << std::setw(25) << sortedStudents[i]->get_name()
                      << " | CGPA: " << Colors::GREEN << std::fixed << std::setprecision(2) << sortedStudents[i]->get_loaded_cgpa() << Colors::RESET << "\n";
        }

        std::cout << "\n"
                  << Colors::GREEN << "[OK] Students sorted successfully." << Colors::RESET << "\n";
        std::cout << Colors::DIM << "   [i] Insertion order preserved. Database unchanged." << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nERROR: " << e.what() << "\n";
    }
}

void AdminPortal::view_all_students()
{
    OutputFormatter::print_header("ALL STUDENTS");

    auto students = erpSystem->get_all_students();

    if (students.empty())
    {
        std::cout << Colors::YELLOW << "\n[!] No students in the system." << Colors::RESET << "\n";
        return;
    }

    std::cout << "\n"
              << Colors::CYAN << "> " << Colors::BOLD << "Total Students: "
              << Colors::GREEN << students.size() << Colors::RESET << "\n\n";
    OutputFormatter::print_line(121);
    std::cout << Colors::BOLD << Colors::CYAN << std::left << std::setw(16) << "Roll No"
              << std::setw(33) << "Name"
              << std::setw(48) << "Branch"
              << std::setw(14) << "Level"
              << std::setw(10) << "Year" << Colors::RESET << "\n";
    OutputFormatter::print_line(121);

    for (const auto &student : students)
    {
        // Convert StudentLevel enum to string
        std::string levelStr;
        switch (student->get_level())
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

        std::cout << Colors::BOLD << std::left << std::setw(16) << student->get_roll_number() << Colors::RESET
                  << std::setw(33) << student->get_name()
                  << Colors::DIM << std::setw(48) << student->get_branch().get_branch_name() << Colors::RESET
                  << Colors::CYAN << std::setw(14) << levelStr << Colors::RESET
                  << Colors::DIM << std::setw(10) << student->get_starting_year() << Colors::RESET << "\n";
    }
    OutputFormatter::print_line(121);
}

void AdminPortal::search_student()
{
    OutputFormatter::print_header("SEARCH STUDENT");
    std::cout << Colors::CYAN << "Enter Roll Number: " << Colors::RESET;

    std::string rollNumber;
    std::getline(std::cin, rollNumber);

    try
    {
        auto student = erpSystem->find_student(rollNumber);
        if (!student)
        {
            std::cout << "\n"
                      << Colors::RED << "[X] ERROR: " << Colors::RESET << "Student not found.\n";
            return;
        }

        // Convert StudentLevel enum to string
        std::string levelStr;
        switch (student->get_level())
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

        std::cout << "\n"
                  << Colors::GREEN << "[OK] Student Found:" << Colors::RESET << "\n";
        OutputFormatter::print_line();
        std::cout << Colors::BOLD << "Roll Number:" << Colors::RESET << " " << student->get_roll_number() << "\n";
        std::cout << Colors::BOLD << "Name:" << Colors::RESET << " " << student->get_name() << "\n";
        std::cout << Colors::BOLD << "Branch:" << Colors::RESET << " " << student->get_branch().get_branch_name() << "\n";
        std::cout << Colors::BOLD << "Level:" << Colors::RESET << " " << levelStr << "\n";
        std::cout << Colors::BOLD << "Start Year:" << Colors::RESET << " " << student->get_starting_year() << "\n";

        // Combine current and previous courses
        auto currentCourses = student->get_current_courses();
        auto previousCourses = student->get_previous_courses();

        std::cout << "\n"
                  << Colors::CYAN << "> Course Enrollments" << Colors::RESET << "\n";
        std::cout << Colors::BOLD << "Current Courses: " << Colors::RESET << Colors::GREEN << currentCourses.size() << Colors::RESET << "\n";
        std::cout << Colors::BOLD << "Previous Courses: " << Colors::RESET << Colors::CYAN << previousCourses.size() << Colors::RESET << "\n";
        std::cout << Colors::BOLD << "Total Enrollments: " << Colors::RESET << Colors::MAGENTA << (currentCourses.size() + previousCourses.size()) << Colors::RESET << "\n";

        if (!currentCourses.empty())
        {
            std::cout << "\n"
                      << Colors::GREEN << "> Current Courses" << Colors::RESET << "\n";
            OutputFormatter::print_line();
            for (const auto &course : currentCourses)
            {
                std::cout << Colors::BOLD << course.get_course_code() << Colors::RESET << " - "
                          << course.get_course_name() << " "
                          << Colors::DIM << "(" << course.get_credits() << " credits)" << Colors::RESET << "\n";
            }
        }

        if (!previousCourses.empty())
        {
            std::cout << "\n"
                      << Colors::CYAN << "> Previous Courses" << Colors::RESET << "\n";
            OutputFormatter::print_line();
            auto previousGrades = student->get_previous_grades();
            for (size_t i = 0; i < previousCourses.size(); ++i)
            {
                const auto &course = previousCourses[i];
                std::cout << Colors::BOLD << course.get_course_code() << Colors::RESET << " - "
                          << course.get_course_name() << " "
                          << Colors::DIM << "(" << course.get_credits() << " credits)" << Colors::RESET;

                if (i < previousGrades.size())
                {
                    const auto &grade = previousGrades[i];
                    std::cout << " - " << Colors::BOLD << "Grade: " << Colors::GREEN << grade.get_letter_grade() << Colors::RESET
                              << Colors::DIM << " (" << std::fixed << std::setprecision(2)
                              << grade.get_grade_point() << ")" << Colors::RESET;
                }
                std::cout << "\n";
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << e.what() << "\n";
    }
}

void AdminPortal::update_student_marks()
{
    OutputFormatter::print_header("UPDATE STUDENT MARKS");
    std::cout << Colors::CYAN << "Enter Roll Number: " << Colors::RESET;

    std::string rollNumber;
    std::getline(std::cin, rollNumber);

    try
    {
        auto student = erpSystem->find_student(rollNumber);
        if (!student)
        {
            std::cout << "\n"
                      << Colors::RED << "[X] ERROR: " << Colors::RESET << "Student not found.\n";
            return;
        }

        auto currentCourses = student->get_current_courses();

        if (currentCourses.empty())
        {
            std::cout << Colors::YELLOW << "[!] WARNING: " << Colors::RESET << "Student has no current course enrollments.\n";
            return;
        }

        std::cout << "\n"
                  << Colors::CYAN << "> Current Courses" << Colors::RESET << "\n";
        OutputFormatter::print_line();
        for (size_t i = 0; i < currentCourses.size(); ++i)
        {
            std::cout << Colors::BOLD << "  " << i + 1 << ". " << Colors::RESET
                      << Colors::CYAN << currentCourses[i].get_course_code() << Colors::RESET
                      << " - " << currentCourses[i].get_course_name() << "\n";
        }

        std::cout << "\n"
                  << Colors::CYAN << "Select course number: " << Colors::RESET;
        int courseNum;
        std::cin >> courseNum;
        std::cin.ignore();

        if (courseNum < 1 || courseNum > static_cast<int>(currentCourses.size()))
        {
            std::cout << Colors::RED << "[X] ERROR: " << Colors::RESET << "Invalid course number.\n";
            return;
        }

        std::cout << "\n"
                  << Colors::CYAN << "> Enter Marks" << Colors::RESET << "\n";
        OutputFormatter::print_line();
        std::cout << Colors::BOLD << "Mid Sem marks (0-100): " << Colors::RESET;
        double midSem;
        std::cin >> midSem;

        std::cout << Colors::BOLD << "End Sem marks (0-100): " << Colors::RESET;
        double endSem;
        std::cin >> endSem;

        std::cout << Colors::BOLD << "Assignment marks (0-100): " << Colors::RESET;
        double assignment;
        std::cin >> assignment;
        std::cin.ignore();

        // Create grade from individual marks (constructor will calculate total and grade point)
        Grade grade(midSem, endSem, assignment);
        double total = grade.get_total_marks();

        // Update grade through ERP system
        erpSystem->update_student_grade(rollNumber, currentCourses[courseNum - 1].get_course_code(), grade);

        std::cout << "\n"
                  << Colors::GREEN << "[OK] Marks updated successfully." << Colors::RESET << "\n";
        std::cout << Colors::BOLD << "  Total: " << Colors::RESET << std::fixed << std::setprecision(2) << total << "\n";
        std::cout << Colors::BOLD << "  Grade: " << Colors::GREEN << grade.get_letter_grade() << Colors::RESET
                  << Colors::DIM << " (" << grade.get_grade_point() << ")" << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nERROR: " << e.what() << "\n";
    }
}

void AdminPortal::delete_student()
{
    OutputFormatter::print_header("DELETE SINGLE STUDENT");
    std::cout << Colors::CYAN << "Enter Roll Number: " << Colors::RESET;

    std::string rollNumber;
    std::getline(std::cin, rollNumber);
    rollNumber = toUpperCase(rollNumber);

    // Check if student exists
    auto student = erpSystem->find_student(rollNumber);
    if (!student)
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Student with roll number '" << rollNumber << "' not found.\n";
        return;
    } // Display student info
    std::cout << "\n"
              << Colors::YELLOW << "> Student Details" << Colors::RESET << "\n";
    OutputFormatter::print_line();
    std::cout << Colors::BOLD << "  Roll Number: " << Colors::RESET << student->get_roll_number() << "\n";
    std::cout << Colors::BOLD << "  Name: " << Colors::RESET << student->get_name() << "\n";
    std::cout << Colors::BOLD << "  Branch: " << Colors::RESET << student->get_branch().get_branch_name() << "\n";

    std::cout << "\n"
              << Colors::YELLOW << "[!] Are you sure you want to delete this student? (yes/no): " << Colors::RESET;
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm == "yes" || confirm == "YES")
    {
        if (erpSystem->remove_student(rollNumber))
        {
            // Save insertion order to file after removal
            erpSystem->save_insertion_order_to_file();

            std::cout << "\n"
                      << Colors::GREEN << "[OK] Student deleted successfully from __database." << Colors::RESET << "\n";
            std::cout << Colors::DIM << "   [i] Note: Student account credentials (if any) remain and must be manually removed." << Colors::RESET << "\n";
        }
        else
        {
            std::cout << "\n"
                      << Colors::RED << "[X] ERROR: " << Colors::RESET << "Student could not be deleted from __database.\n";
        }
    }
    else
    {
        std::cout << "\n"
                  << Colors::CYAN << "[i] Deletion cancelled." << Colors::RESET << "\n";
    }
}

void AdminPortal::delete_bulk_students()
{
    OutputFormatter::print_header("DELETE STUDENTS FROM CSV (BULK)");
    std::cout << Colors::CYAN << "Enter CSV filename (must contain roll numbers): " << Colors::RESET;

    std::string filename;
    std::getline(std::cin, filename);

    std::cout << "\n"
              << Colors::YELLOW << "[!] This will delete all students whose roll numbers are listed in the CSV file." << Colors::RESET << "\n";
    std::cout << Colors::YELLOW << "Are you sure you want to proceed? (yes/no): " << Colors::RESET;
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm != "yes" && confirm != "YES")
    {
        std::cout << "\n"
                  << Colors::CYAN << "[i] Bulk deletion cancelled." << Colors::RESET << "\n";
        return;
    }

    try
    {
        // Open CSV file
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cout << "\nERROR: Could not open file '" << filename << "'.\n";
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
            std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "No valid roll numbers found in CSV file.\n";
            return;
        }

        std::cout << "\n"
                  << Colors::CYAN << "[i] Found " << Colors::BOLD << rollNumbers.size() << Colors::RESET
                  << Colors::CYAN << " roll number(s) in file." << Colors::RESET << "\n";
        std::cout << Colors::YELLOW << " Processing deletions..." << Colors::RESET << "\n\n";

        int successCount = 0;
        int notFoundCount = 0;

        for (const auto &rollNumber : rollNumbers)
        {
            // Check if student exists
            auto student = erpSystem->find_student(rollNumber);

            if (!student)
            {
                std::cout << Colors::RED << "  [X] " << Colors::RESET << "Roll " << rollNumber << " - not found\n";
                notFoundCount++;
                continue;
            }

            // Delete from __database
            if (erpSystem->remove_student(rollNumber))
            {
                std::cout << Colors::GREEN << "  [OK] " << Colors::RESET << "Roll " << rollNumber << " - deleted "
                          << Colors::DIM << "(" << student->get_name() << ")" << Colors::RESET << "\n";
                successCount++;
            }
            else
            {
                std::cout << Colors::RED << "  [X] " << Colors::RESET << "Roll " << rollNumber << " - could not delete\n";
            }
        }

        OutputFormatter::print_header("BULK DELETION SUMMARY");
        std::cout << Colors::BOLD << "Total roll numbers in file: " << Colors::RESET << rollNumbers.size() << "\n";
        std::cout << Colors::GREEN << "Successfully deleted: " << Colors::RESET << successCount << "\n";
        std::cout << Colors::YELLOW << "Not found: " << Colors::RESET << notFoundCount << "\n";

        // Save insertion order to file after bulk deletions
        if (successCount > 0)
        {
            erpSystem->save_insertion_order_to_file();
        }

        std::cout << Colors::GREEN << "\n[OK] Bulk deletion completed." << Colors::RESET << "\n";
        std::cout << Colors::DIM << "   [i] Note: Student account credentials (if any) remain and must be manually removed." << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Error during bulk deletion: " << e.what() << "\n";
    }
}

void AdminPortal::delete_all_students()
{
    std::cout << "\n";
    OutputFormatter::print_double_line(80);
    std::cout << Colors::BRIGHT_RED << Colors::BOLD << "               WARNING: DELETE ALL STUDENTS" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(80);
    std::cout << Colors::RED << "  This action will PERMANENTLY DELETE ALL STUDENTS from the" << Colors::RESET << "\n";
    std::cout << Colors::RED << "  __database. This operation CANNOT be undone!" << Colors::RESET << "\n";
    std::cout << "\n";
    std::cout << Colors::YELLOW << "  All student records, enrollment data, and grades will be" << Colors::RESET << "\n";
    std::cout << Colors::YELLOW << "  removed from the system." << Colors::RESET << "\n";
    OutputFormatter::print_double_line(80);

    // Get current student count
    auto students = erpSystem->get_all_students();
    size_t totalStudents = students.size();

    if (totalStudents == 0)
    {
        std::cout << "\n"
                  << Colors::CYAN << "[i] No students in the __database to delete." << Colors::RESET << "\n";
        return;
    }

    std::cout << "\n"
              << Colors::BOLD << "Current number of students in __database: " << Colors::BRIGHT_RED << totalStudents << Colors::RESET << "\n";

    // First confirmation
    std::cout << "\n"
              << Colors::YELLOW << "Are you absolutely sure you want to delete ALL students? (yes/no): " << Colors::RESET;
    std::string confirm1;
    std::getline(std::cin, confirm1);

    if (confirm1 != "yes" && confirm1 != "YES")
    {
        std::cout << "\n"
                  << Colors::GREEN << "[OK] Deletion cancelled." << Colors::RESET << "\n";
        return;
    }

    // Second confirmation with exact phrase
    std::cout << "\n"
              << Colors::BRIGHT_RED << Colors::BOLD << "[!] FINAL WARNING!" << Colors::RESET << "\n";
    std::cout << Colors::YELLOW << "To confirm, type exactly: " << Colors::BOLD << "DELETE ALL" << Colors::RESET << "\n";
    std::cout << Colors::CYAN << "> " << Colors::RESET;
    std::string confirm2;
    std::getline(std::cin, confirm2);

    if (confirm2 != "DELETE ALL")
    {
        std::cout << "\n"
                  << Colors::GREEN << "[OK] Deletion cancelled. " << Colors::DIM << "(Confirmation phrase did not match)" << Colors::RESET << "\n";
        return;
    }

    std::cout << "\n"
              << Colors::YELLOW << " Starting deletion of all students..." << Colors::RESET << "\n\n";

    int successCount = 0;
    int failCount = 0;

    // Create a copy of roll numbers to delete (to avoid iterator invalidation)
    std::vector<std::string> rollNumbers;
    rollNumbers.reserve(totalStudents);
    for (const auto &student : students)
    {
        rollNumbers.push_back(student->get_roll_number());
    }

    // Progress indicator
    std::cout << Colors::CYAN << "Progress: [" << Colors::RESET;
    size_t progressBarWidth = 50;
    size_t lastProgress = 0;

    for (size_t i = 0; i < rollNumbers.size(); ++i)
    {
        const auto &rollNumber = rollNumbers[i];

        if (erpSystem->remove_student(rollNumber))
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
                std::cout << Colors::GREEN << "=" << Colors::RESET;
                std::cout.flush();
            }
            lastProgress = currentProgress;
        }
    }
    std::cout << Colors::CYAN << "] " << Colors::GREEN << "100%" << Colors::RESET << "\n\n";

    // Summary
    OutputFormatter::print_double_line(80);
    std::cout << Colors::BOLD << "               DELETE ALL STUDENTS - SUMMARY" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(80);
    std::cout << Colors::BOLD << "  Total students processed:  " << Colors::RESET << Colors::CYAN << totalStudents << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "  Successfully deleted:      " << Colors::RESET << Colors::GREEN << successCount << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "  Failed to delete:          " << Colors::RESET << Colors::RED << failCount << Colors::RESET << "\n";
    OutputFormatter::print_double_line(80);

    if (successCount > 0)
    {
        // Clear insertion order file
        Database db;
        db.clearInsertionOrder();

        std::cout << "\n"
                  << Colors::GREEN << "[OK] All students have been deleted from the __database." << Colors::RESET << "\n";
        std::cout << Colors::DIM << "   [i] Note: Student account credentials (if any) remain and must be manually removed." << Colors::RESET << "\n";
    }

    if (failCount > 0)
    {
        std::cout << "\n"
                  << Colors::YELLOW << "[!] Warning: " << Colors::RESET << failCount << " student(s) could not be deleted.\n";
    }
}

void AdminPortal::create_student_account()
{
    OutputFormatter::print_header("CREATE STUDENT ACCOUNT");
    std::cout << Colors::CYAN << "Enter Roll Number: " << Colors::RESET;

    std::string rollNumber;
    std::getline(std::cin, rollNumber);

    try
    {
        // Check if student exists in ERP
        auto student = erpSystem->find_student(rollNumber);
        if (!student)
        {
            std::cout << "\n"
                      << Colors::RED << "[X] ERROR: " << Colors::RESET << "Student not found in ERP system.\n";
            std::cout << Colors::DIM << "  [i] Please add the student first before creating account." << Colors::RESET << "\n";
            return;
        }

        // Create credentials
        std::string password = Auth::create_student_account(rollNumber);

        std::cout << "\n"
                  << Colors::GREEN << "[OK] Student account created successfully." << Colors::RESET << "\n";
        OutputFormatter::print_line();
        std::cout << Colors::BOLD << "  Roll Number (Login ID): " << Colors::RESET << Colors::CYAN << rollNumber << Colors::RESET << "\n";
        std::cout << Colors::BOLD << "  Generated Password:     " << Colors::RESET << Colors::YELLOW << password << Colors::RESET << "\n";
        OutputFormatter::print_line();
        std::cout << Colors::DIM << "\n  [i] Please share these credentials with the student" << Colors::RESET << "\n";
        std::cout << Colors::DIM << "  [i] Student should change password after first login" << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\n"
                  << Colors::RED << "[X] ERROR: " << Colors::RESET << e.what() << "\n";
    }
}

void AdminPortal::bulk_create_student_accounts()
{
    OutputFormatter::print_header("BULK CREATE STUDENT ACCOUNTS");
    std::cout << "\n"
              << Colors::CYAN << "[i] This will create accounts for ALL students in the __database" << Colors::RESET << "\n";
    std::cout << Colors::CYAN << "  who do not already have credentials." << Colors::RESET << "\n";
    std::cout << "\n"
              << Colors::BOLD << "Default Password: " << Colors::YELLOW << "abc123" << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "User ID: " << Colors::CYAN << "Student's Roll Number" << Colors::RESET << "\n";
    std::cout << "\n"
              << Colors::YELLOW << "Do you want to proceed? (YES/NO): " << Colors::RESET;

    std::string confirm;
    std::getline(std::cin, confirm);
    confirm = toUpperCase(confirm);

    if (confirm != "YES")
    {
        std::cout << "\n"
                  << Colors::CYAN << "[i] Bulk account creation cancelled." << Colors::RESET << "\n";
        return;
    }

    std::cout << "\n"
              << Colors::YELLOW << " Processing bulk account creation..." << Colors::RESET << "\n\n";

    // Get all students from ERP system
    auto students = erpSystem->get_all_students();

    if (students.empty())
    {
        std::cout << Colors::RED << "[X] " << Colors::RESET << "No students found in the __database.\n";
        return;
    }

    int created = 0;
    int skipped = 0;
    const std::string defaultPassword = "abc123";

    std::cout << Colors::BOLD << std::left << std::setw(20) << "Roll Number"
              << std::setw(35) << "Name"
              << "Status" << Colors::RESET << "\n";
    OutputFormatter::print_line(70);

    for (const auto &student : students)
    {
        std::string rollNumber = student->get_roll_number();
        std::string name = student->get_name();

        try
        {
            bool success = Auth::create_student_account_with_password(rollNumber, defaultPassword);

            if (success)
            {
                created++;
                std::cout << std::left << std::setw(20) << rollNumber
                          << std::setw(35) << name
                          << Colors::GREEN << "[OK] Created" << Colors::RESET << "\n";
            }
            else
            {
                skipped++;
                std::cout << std::left << std::setw(20) << rollNumber
                          << std::setw(35) << name
                          << Colors::DIM << "[i] Skipped (Already exists)" << Colors::RESET << "\n";
            }
        }
        catch (const std::exception &e)
        {
            skipped++;
            std::cout << std::left << std::setw(20) << rollNumber
                      << std::setw(35) << name
                      << Colors::RED << "[X] Error: " << e.what() << Colors::RESET << "\n";
        }
    }

    OutputFormatter::print_double_line(70);
    std::cout << Colors::GREEN << "[OK] Bulk account creation completed!" << Colors::RESET << "\n\n";
    std::cout << Colors::BOLD << "Total Students: " << Colors::RESET << students.size() << "\n";
    std::cout << Colors::GREEN << "Accounts Created: " << Colors::RESET << created << "\n";
    std::cout << Colors::YELLOW << "Skipped (Already existed): " << Colors::RESET << skipped << "\n";

    if (created > 0)
    {
        std::cout << "\n"
                  << Colors::BOLD << "[i] All new accounts created with password: " << Colors::YELLOW << "abc123" << Colors::RESET << "\n";
        std::cout << Colors::DIM << "[i] Students should change their password after first login" << Colors::RESET << "\n";
    }
}

void AdminPortal::manage_courses()
{
    OutputFormatter::print_header("COURSE CATALOG MANAGEMENT");
    OutputFormatter::print_menu_item(1, "View All Courses", Colors::CYAN);
    OutputFormatter::print_menu_item(2, "Add New Course", Colors::GREEN);
    OutputFormatter::print_menu_item(3, "View Course Details", Colors::CYAN);
    OutputFormatter::print_menu_item(4, "Search Courses", Colors::CYAN);
    OutputFormatter::print_menu_item(5, "Back to Main Menu", Colors::DIM);
    std::cout << "\n"
              << Colors::CYAN << "Enter choice: " << Colors::RESET;

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice)
    {
    case 1:
    {
        auto courses = courseCatalog->get_all_courses();
        OutputFormatter::print_header("ALL COURSES");
        std::cout << "\n"
                  << Colors::DIM << "> " << Colors::RESET << "Total Courses: "
                  << Colors::BOLD << courses.size() << Colors::RESET << "\n\n";
        OutputFormatter::print_line(120);
        std::cout << Colors::BOLD << std::left << std::setw(12) << "Code"
                  << std::setw(48) << "Name"
                  << std::setw(10) << "Credits"
                  << std::setw(38) << "Instructor"
                  << std::setw(12) << "Type" << Colors::RESET << "\n";
        OutputFormatter::print_line(120);

        for (const auto &course : courses)
        {
            std::cout << Colors::CYAN << std::left << std::setw(12) << course.courseCode << Colors::RESET
                      << std::left << std::setw(48) << course.courseName
                      << Colors::DIM << std::setw(10) << course.credits << Colors::RESET
                      << Colors::DIM << std::setw(38) << course.instructor << Colors::RESET
                      << std::left << std::setw(12) << course.courseType << "\n";
        }
        OutputFormatter::print_line(120);
        break;
    }
    case 2:
    {
        OutputFormatter::print_header("ADD NEW COURSE");
        CourseInfo course;

        std::cout << Colors::DIM << "Course Code: " << Colors::RESET;
        std::getline(std::cin, course.courseCode);

        std::cout << Colors::DIM << "Course Name: " << Colors::RESET;
        std::getline(std::cin, course.courseName);

        std::cout << Colors::DIM << "Credits: " << Colors::RESET;
        std::cin >> course.credits;
        std::cin.ignore();

        std::cout << Colors::DIM << "Instructor: " << Colors::RESET;
        std::getline(std::cin, course.instructor);

        std::cout << Colors::DIM << "Type (Regular/Capstone/Research): " << Colors::RESET;
        std::getline(std::cin, course.courseType);

        std::cout << Colors::DIM << "Eligible Levels (BTech MTech PhD, space-separated): " << Colors::RESET;
        std::string levels;
        std::getline(std::cin, levels);
        std::istringstream iss(levels);
        std::string level;
        while (iss >> level)
        {
            course.eligibleLevels.insert(level);
        }

        courseCatalog->add_course(course);
        std::cout << "\n"
                  << Colors::GREEN << "[OK] Course added successfully." << Colors::RESET << "\n";
        break;
    }
    case 3:
    {
        std::cout << "\n"
                  << Colors::DIM << "Enter Course Code: " << Colors::RESET;
        std::string code;
        std::getline(std::cin, code);
        code = toUpperCase(code); // Convert to uppercase
        courseCatalog->displayCourse(code);
        break;
    }
    case 4:
    {
        std::cout << "\n"
                  << Colors::DIM << "Enter search query: " << Colors::RESET;
        std::string query;
        std::getline(std::cin, query);
        query = toUpperCase(query); // Convert to uppercase

        auto results = courseCatalog->search_courses(query);
        std::cout << "\n"
                  << Colors::DIM << "> Found " << Colors::BOLD << results.size() << Colors::RESET
                  << Colors::DIM << " course(s)" << Colors::RESET << "\n\n";

        if (!results.empty())
        {
            OutputFormatter::print_line(70);
            for (const auto &course : results)
            {
                std::cout << Colors::CYAN << "  " << std::left << std::setw(12) << course.courseCode << Colors::RESET
                          << course.courseName << "\n";
            }
            OutputFormatter::print_line(70);
        }
        break;
    }
    }
}

void AdminPortal::view_backups()
{
    OutputFormatter::print_header("DATABASE BACKUPS");
    auto backups = Database::getBackupInfo();

    for (const auto &backup : backups)
    {
        std::cout << Colors::CYAN << "• " << Colors::RESET << backup << "\n";
    }
}

void AdminPortal::restore_backup()
{
    OutputFormatter::print_header("RESTORE FROM BACKUP");
    std::cout << Colors::CYAN << "Enter backup number (1 or 2): " << Colors::RESET;

    int backupNum;
    std::cin >> backupNum;
    std::cin.ignore();

    std::cout << Colors::YELLOW << "\n[!] Are you sure you want to restore from backup "
              << backupNum << "? (yes/no): " << Colors::RESET;
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm == "yes")
    {
        if (Database::restoreFromBackup(backupNum))
        {
            std::cout << "\n"
                      << Colors::GREEN << "[OK] Database restored successfully from backup "
                      << backupNum << "." << Colors::RESET << "\n";
            std::cout << Colors::DIM << "  [i] Please restart the application to load restored data." << Colors::RESET << "\n";
        }
        else
        {
            std::cout << "\n"
                      << Colors::RED << "[X] ERROR: " << Colors::RESET << "Failed to restore backup.\n";
        }
    }
    else
    {
        std::cout << "\n"
                  << Colors::CYAN << "[i] Restore cancelled." << Colors::RESET << "\n";
    }
}

void AdminPortal::export_data()
{
    OutputFormatter::print_header("EXPORT DATA TO CSV");
    std::cout << Colors::CYAN << "Enter output filename: " << Colors::RESET;

    std::string filename;
    std::getline(std::cin, filename);

    try
    {
        Database::exportToCSV(filename);
        std::cout << "\n"
                  << Colors::GREEN << "[OK] Data exported successfully to " << Colors::BOLD << filename << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\n"
                  << Colors::RED << "[X] ERROR: " << Colors::RESET << e.what() << "\n";
    }
}

void AdminPortal::view_statistics()
{
    OutputFormatter::print_header("SYSTEM STATISTICS");

    auto students = erpSystem->get_all_students();

    std::cout << "\n"
              << Colors::BOLD << Colors::GREEN << " OVERVIEW" << Colors::RESET << "\n";
    OutputFormatter::print_line();
    std::cout << Colors::CYAN << "  Total Students: " << Colors::BOLD << Colors::GREEN
              << students.size() << Colors::RESET << "\n";

    // Count by level
    std::map<std::string, int> levelCount;
    std::map<std::string, int> branchCount;

    for (const auto &student : students)
    {
        // Convert StudentLevel enum to string
        std::string levelStr;
        switch (student->get_level())
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
        branchCount[student->get_branch().get_branch_name()]++;
    }

    std::cout << "\n"
              << Colors::BOLD << Colors::YELLOW << " STUDENTS BY LEVEL" << Colors::RESET << "\n";
    OutputFormatter::print_line();
    for (const auto &pair : levelCount)
    {
        std::cout << "  " << Colors::BOLD << Colors::CYAN << pair.first << Colors::RESET
                  << ": " << Colors::GREEN << pair.second << Colors::RESET << "\n";
    }

    std::cout << "\n"
              << Colors::BOLD << Colors::YELLOW << " STUDENTS BY BRANCH" << Colors::RESET << "\n";
    OutputFormatter::print_line();
    for (const auto &pair : branchCount)
    {
        std::cout << "  " << Colors::DIM << "• " << Colors::RESET << Colors::BOLD
                  << pair.first << Colors::RESET << ": " << Colors::GREEN << pair.second << Colors::RESET << "\n";
    }

    std::cout << "\n"
              << Colors::BOLD << Colors::YELLOW << " COURSE INFORMATION" << Colors::RESET << "\n";
    OutputFormatter::print_line();
    std::cout << "  " << Colors::CYAN << "Total Courses in Catalog: " << Colors::BOLD << Colors::GREEN
              << courseCatalog->get_all_courses().size() << Colors::RESET << "\n";

    std::cout << semesterManager->get_status();
}

void AdminPortal::change_own_password()
{
    OutputFormatter::print_header("CHANGE PASSWORD");
    std::cout << Colors::CYAN << "Enter current password: " << Colors::RESET;

    std::string oldPassword, newPassword, confirmPassword;
    std::getline(std::cin, oldPassword);

    std::cout << Colors::CYAN << "Enter new password: " << Colors::RESET;
    std::getline(std::cin, newPassword);

    std::cout << Colors::CYAN << "Confirm new password: " << Colors::RESET;
    std::getline(std::cin, confirmPassword);

    if (newPassword != confirmPassword)
    {
        std::cout << "\n"
                  << Colors::RED << "[X] ERROR: " << Colors::RESET << "Passwords do not match.\n";
        return;
    }

    if (Auth::change_password(__admin_username, oldPassword, newPassword, UserType::ADMIN))
    {
        std::cout << "\n"
                  << Colors::GREEN << "[OK] Password changed successfully." << Colors::RESET << "\n";
    }
    else
    {
        std::cout << "\n"
                  << Colors::RED << "[X] ERROR: " << Colors::RESET << "Failed to change password. Check your current password.\n";
    }
}

void AdminPortal::view_students_insertion_order()
{
    std::cout << "\n";
    OutputFormatter::print_double_line(102);
    std::cout << Colors::BOLD << "   Students in Insertion Order" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "   (Shows the order in which students were added to the system)" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(102);
    std::cout << "\n"
              << Colors::CYAN << "[i] Using InsertionOrderIterator to traverse enrollment records." << Colors::RESET << "\n\n";

    try
    {
        int count = 0;
        std::cout << Colors::BOLD << std::left << std::setw(5) << "#"
                  << std::setw(16) << "Roll No"
                  << std::setw(30) << "Name"
                  << std::setw(45) << "Branch"
                  << std::right << std::setw(6) << "CGPA" << Colors::RESET << "\n";
        OutputFormatter::print_line(102);

        for (auto it = erpSystem->beginInsertionOrder(); it != erpSystem->endInsertionOrder(); ++it)
        {
            auto student = *it;
            count++;
            std::cout << Colors::CYAN << std::left << std::setw(5) << count << Colors::RESET
                      << std::setw(16) << student->get_roll_number()
                      << std::setw(30) << student->get_name()
                      << std::setw(45) << student->get_branch().get_branch_name()
                      << Colors::GREEN << std::right << std::setw(6) << std::fixed << std::setprecision(2)
                      << student->get_loaded_cgpa() << Colors::RESET << "\n";

            // Show first 50 only, then ask to continue
            if (count % 50 == 0)
            {
                std::cout << "\n"
                          << Colors::CYAN << "[i] Displayed " << count << " students. Press Enter to continue (or Ctrl+C to stop)..." << Colors::RESET;
                std::cin.get();
            }
        }

        OutputFormatter::print_line(102);
        std::cout << Colors::GREEN << "[OK] Total students displayed: " << Colors::BOLD << count << Colors::RESET
                  << Colors::GREEN << " (in insertion order)" << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\n"
                  << Colors::RED << "[X] ERROR: " << Colors::RESET << e.what() << "\n";
    }
}

void AdminPortal::view_students_sorted_order()
{
    std::cout << "\n";
    OutputFormatter::print_double_line(102);
    std::cout << Colors::BOLD << "   Students in Sorted Order" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "   (Displays students after sorting by name/roll/CGPA)" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(102);
    std::cout << "\n"
              << Colors::CYAN << "[i] Using SortedOrderIterator to traverse sorted records." << Colors::RESET << "\n";
    std::cout << Colors::DIM << "[i] Note: You must run the Sort option (Menu 9) before using this." << Colors::RESET << "\n\n";

    try
    {
        if (!erpSystem->get_is_sorted())
        {
            std::cout << "\n"
                      << Colors::RED << "[X] ERROR: " << Colors::RESET << "Students are not sorted yet.\n";
            std::cout << Colors::DIM << "   [i] Please use Menu Option 9 to sort students first." << Colors::RESET << "\n";
            return;
        }

        int count = 0;
        std::cout << Colors::BOLD << std::left << std::setw(5) << "#"
                  << std::setw(16) << "Roll No"
                  << std::setw(30) << "Name"
                  << std::setw(45) << "Branch"
                  << std::right << std::setw(6) << "CGPA" << Colors::RESET << "\n";
        OutputFormatter::print_line(102);

        for (auto it = erpSystem->beginSorted(); it != erpSystem->endSorted(); ++it)
        {
            auto student = *it;
            count++;
            std::cout << Colors::CYAN << std::left << std::setw(5) << count << Colors::RESET
                      << std::setw(16) << student->get_roll_number()
                      << std::setw(30) << student->get_name()
                      << std::setw(45) << student->get_branch().get_branch_name()
                      << Colors::GREEN << std::right << std::setw(6) << std::fixed << std::setprecision(2)
                      << student->get_loaded_cgpa() << Colors::RESET << "\n";

            // Show first 50 only, then ask to continue
            if (count % 50 == 0)
            {
                std::cout << "\n"
                          << Colors::CYAN << "[i] Displayed " << count << " students. Press Enter to continue (or Ctrl+C to stop)..." << Colors::RESET;
                std::cin.get();
            }
        }

        OutputFormatter::print_line(102);
        std::cout << Colors::GREEN << "[OK] Total students displayed: " << Colors::BOLD << count << Colors::RESET
                  << Colors::GREEN << " (in sorted order)" << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\n"
                  << Colors::RED << "[X] ERROR: " << Colors::RESET << e.what() << "\n";
    }
}

void AdminPortal::find_students_by_grade()
{
    std::cout << "\n";
    OutputFormatter::print_double_line(80);
    std::cout << Colors::BOLD << "   Find Students by Grade in a Course" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(80);
    std::cout << "\n"
              << Colors::CYAN << "[i] This search uses a grade index for fast O(log n) lookups." << Colors::RESET << "\n";
    std::cout << Colors::CYAN << "  Perfect for placement queries like 'Find all A-grade students'." << Colors::RESET << "\n\n";

    std::cout << Colors::CYAN << "Enter Course Code (e.g., CSE101, CSE201, MAT101, ECE101): " << Colors::RESET;
    std::string courseCode;
    std::getline(std::cin, courseCode);
    courseCode = toUpperCase(courseCode);

    std::cout << Colors::CYAN << "Enter Minimum Grade Points (e.g., 9.0, 8.0, 7.0): " << Colors::RESET;
    double minGrade;
    if (!(std::cin >> minGrade))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n"
                  << Colors::RED << "[X] Error: " << Colors::RESET << "Invalid input. Please enter a number.\n";
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
            std::cout << "\n"
                      << Colors::YELLOW << "[!] No students found with grade >= " << std::fixed << std::setprecision(1)
                      << minGrade << " in course " << courseCode << "." << Colors::RESET << "\n\n";
            std::cout << Colors::DIM << "Possible reasons:" << Colors::RESET << "\n";
            std::cout << Colors::DIM << "  • No students have completed this course yet" << Colors::RESET << "\n";
            std::cout << Colors::DIM << "  • No students achieved the minimum grade threshold" << Colors::RESET << "\n";
            std::cout << Colors::DIM << "  • The course code might be incorrect (check spelling)" << Colors::RESET << "\n\n";
            std::cout << Colors::CYAN << "[i] Hint: Try popular courses like CSE101, CSE201, MAT101, ECE101" << Colors::RESET << "\n";
            return;
        }

        std::cout << "\n"
                  << Colors::GREEN << "[OK] Search completed in " << Colors::BOLD << duration << " microseconds" << Colors::RESET;
        std::cout << Colors::DIM << " (demonstrates O(log n) efficiency)" << Colors::RESET << "\n";
        std::cout << "\n"
                  << Colors::CYAN << "[i] Found " << Colors::BOLD << results.size() << Colors::RESET
                  << Colors::CYAN << " student(s) with grade >= "
                  << std::fixed << std::setprecision(1) << minGrade
                  << " in " << courseCode << Colors::RESET << "\n\n";

        std::cout << Colors::BOLD << std::left << std::setw(4) << "No."
                  << std::setw(18) << "Roll Number"
                  << std::setw(28) << "Name"
                  << std::setw(10) << "Grade"
                  << std::setw(15) << "Branch" << Colors::RESET << "\n";
        OutputFormatter::print_line(75);

        int count = 1;
        for (const auto &student : results)
        {
            auto previousCourses = student->get_previous_courses();
            auto previousGrades = student->get_previous_grades();

            std::string gradeStr = "N/A";
            for (size_t i = 0; i < previousCourses.size(); ++i)
            {
                if (previousCourses[i].get_course_code() == courseCode && i < previousGrades.size())
                {
                    double pts = previousGrades[i].get_grade_point();
                    std::string letter = previousGrades[i].get_letter_grade();
                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(1) << pts;
                    gradeStr = oss.str() + " (" + letter + ")";
                    break;
                }
            }

            std::cout << std::left << std::setw(4) << count++
                      << Colors::CYAN << std::setw(18) << student->get_roll_number() << Colors::RESET
                      << std::setw(28) << student->get_name()
                      << Colors::GREEN << std::setw(10) << gradeStr << Colors::RESET
                      << std::setw(15) << student->get_branch().get_branch_name() << "\n";
        }

        OutputFormatter::print_line(75);
        std::cout << Colors::BOLD << "Total: " << Colors::GREEN << results.size() << Colors::RESET
                  << Colors::BOLD << " student(s) qualified" << Colors::RESET << "\n";
        std::cout << "\n"
                  << Colors::CYAN << "[i] This fast lookup is useful for:" << Colors::RESET << "\n";
        std::cout << Colors::DIM << "  • Placement companies screening candidates" << Colors::RESET << "\n";
        std::cout << Colors::DIM << "  • Merit list generation" << Colors::RESET << "\n";
        std::cout << Colors::DIM << "  • Scholarship eligibility checks" << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\n"
                  << Colors::RED << "[X] Error: " << Colors::RESET << e.what() << "\n";
    }
}

void AdminPortal::rebuild_grade_index()
{
    std::cout << "\n";
    OutputFormatter::print_double_line(80);
    std::cout << Colors::BOLD << "   Rebuild Grade Index" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(80);
    std::cout << "\n"
              << Colors::CYAN << "[i] This rebuilds the internal index used for fast grade searches." << Colors::RESET << "\n";
    std::cout << Colors::CYAN << "  Useful after bulk grade updates or importing new data." << Colors::RESET << "\n\n";

    std::cout << Colors::YELLOW << "Do you want to rebuild the index now? (yes/no): " << Colors::RESET;
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm != "yes" && confirm != "YES")
    {
        std::cout << "\n"
                  << Colors::CYAN << "[i] Cancelled." << Colors::RESET << "\n";
        return;
    }

    try
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        erpSystem->rebuild_grade_index();
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

        std::cout << "\n"
                  << Colors::GREEN << "[OK] Grade index rebuilt successfully in " << Colors::BOLD << duration << " ms" << Colors::RESET << "\n";
        std::cout << Colors::CYAN << "[i] Grade searches (Menu Option 27) are now up-to-date." << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\n"
                  << Colors::RED << "[X] Error: " << Colors::RESET << e.what() << "\n";
    }
}

void AdminPortal::view_pending_course_requests()
{
    OutputFormatter::print_header("PENDING COURSE REGISTRATION REQUESTS");

    try
    {
        auto pendingRequests = erpSystem->get_pending_requests();

        if (pendingRequests.empty())
        {
            std::cout << "\n"
                      << Colors::CYAN << "[i] No pending course registration requests found." << Colors::RESET << "\n";
            return;
        }

        // Filter only pending requests
        std::vector<PendingCourseRequest<std::string, std::string>> pending;
        for (const auto &req : pendingRequests)
        {
            if (req.get_status() == RequestStatus::PENDING)
            {
                pending.push_back(req);
            }
        }

        if (pending.empty())
        {
            std::cout << "\n"
                      << Colors::GREEN << "[OK] No pending requests. All requests have been processed." << Colors::RESET << "\n";
            return;
        }

        std::cout << "\n"
                  << Colors::YELLOW << "Total Pending Requests: " << Colors::BOLD << pending.size() << Colors::RESET << "\n\n";
        std::cout << Colors::BOLD << std::left << std::setw(8) << "Req ID"
                  << std::setw(15) << "Roll Number"
                  << std::setw(25) << "Student Name"
                  << std::setw(12) << "Course"
                  << std::setw(20) << "Request Date" << Colors::RESET << "\n";
        OutputFormatter::print_line(80);

        for (const auto &request : pending)
        {
            std::cout << Colors::CYAN << std::left << std::setw(8) << request.get_request_id() << Colors::RESET
                      << std::setw(15) << request.get_roll_number()
                      << std::setw(25) << request.get_student_name()
                      << Colors::YELLOW << std::setw(12) << request.get_course_code() << Colors::RESET
                      << Colors::DIM << std::setw(20) << request.get_request_time_string() << Colors::RESET << "\n";
        }

        OutputFormatter::print_line(80);
        std::cout << Colors::DIM << "[i] Use option 15 to approve or reject these requests." << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\n"
                  << Colors::RED << "[X] ERROR: " << Colors::RESET << e.what() << "\n";
    }
}

void AdminPortal::approve_course_request()
{
    OutputFormatter::print_header("APPROVE/REJECT COURSE REGISTRATION REQUEST");

    // First, show pending requests
    try
    {
        auto pendingRequests = erpSystem->get_pending_requests();

        if (pendingRequests.empty())
        {
            std::cout << "\n"
                      << Colors::CYAN << "[i] No pending course registration requests found." << Colors::RESET << "\n";
            return;
        }

        // Filter only pending requests
        std::vector<PendingCourseRequest<std::string, std::string>> pending;
        for (const auto &req : pendingRequests)
        {
            if (req.get_status() == RequestStatus::PENDING)
            {
                pending.push_back(req);
            }
        }

        if (pending.empty())
        {
            std::cout << "\n"
                      << Colors::GREEN << "[OK] No pending requests. All requests have been processed." << Colors::RESET << "\n";
            return;
        }

        std::cout << "\n"
                  << Colors::YELLOW << "> Pending Requests" << Colors::RESET << "\n";
        OutputFormatter::print_line(60);
        std::cout << Colors::BOLD << std::left << std::setw(8) << "Req ID"
                  << std::setw(15) << "Roll Number"
                  << std::setw(25) << "Student Name"
                  << std::setw(12) << "Course" << Colors::RESET << "\n";
        OutputFormatter::print_line(60);

        for (const auto &request : pending)
        {
            std::cout << Colors::CYAN << std::left << std::setw(8) << request.get_request_id() << Colors::RESET
                      << std::setw(15) << request.get_roll_number()
                      << std::setw(25) << request.get_student_name()
                      << Colors::YELLOW << std::setw(12) << request.get_course_code() << Colors::RESET << "\n";
        }

        OutputFormatter::print_line(60);
        std::cout << "\n"
                  << Colors::CYAN << "Enter Request ID to process (0 to cancel): " << Colors::RESET;
        int requestId;
        std::cin >> requestId;
        std::cin.ignore();

        if (requestId == 0)
        {
            std::cout << "\n"
                      << Colors::CYAN << "[i] Operation cancelled." << Colors::RESET << "\n";
            return;
        }

        // Find the request
        auto *request = erpSystem->findPendingRequest(requestId);
        if (!request)
        {
            std::cout << "\n"
                      << Colors::RED << "[X] ERROR: " << Colors::RESET << "Request ID not found.\n";
            return;
        }

        if (request->get_status() != RequestStatus::PENDING)
        {
            std::cout << "\n"
                      << Colors::RED << "[X] ERROR: " << Colors::RESET << "This request has already been processed.\n";
            std::cout << Colors::DIM << "  Status: " << request_status_to_string(request->get_status()) << Colors::RESET << "\n";
            return;
        }

        // Display request details
        std::cout << "\n"
                  << Colors::YELLOW << "> Request Details" << Colors::RESET << "\n";
        OutputFormatter::print_line();
        request->display_info();

        std::cout << "\n";
        OutputFormatter::print_menu_item(1, "Approve", Colors::GREEN);
        OutputFormatter::print_menu_item(2, "Reject", Colors::RED);
        OutputFormatter::print_menu_item(3, "Cancel", Colors::DIM);
        std::cout << Colors::CYAN << "Enter your choice: " << Colors::RESET;

        int action;
        std::cin >> action;
        std::cin.ignore();

        if (action == 3)
        {
            std::cout << "\n"
                      << Colors::CYAN << "[i] Operation cancelled." << Colors::RESET << "\n";
            return;
        }

        std::cout << Colors::CYAN << "Enter remarks (optional, press Enter to skip): " << Colors::RESET;
        std::string remarks;
        std::getline(std::cin, remarks);

        if (action == 1)
        {
            // Approve - add course to student's enrollment
            if (erpSystem->approve_pending_request(requestId, __admin_username, remarks))
            {
                // Now actually add the course to the student
                auto student = erpSystem->find_student(request->get_roll_number());
                if (student)
                {
                    // Get course details from catalog
                    if (courseCatalog->courseExists(request->get_course_code()))
                    {
                        auto courseInfo = courseCatalog->getCourseInfo(request->get_course_code());
                        Course<std::string> course(
                            courseInfo.courseCode,
                            courseInfo.courseName,
                            courseInfo.instructor,
                            courseInfo.credits,
                            "Monsoon 2025");

                        student->add_current_course(course);

                        std::cout << "\n"
                                  << Colors::GREEN << "[OK] Request APPROVED successfully." << Colors::RESET << "\n";
                        std::cout << Colors::CYAN << "  [i] Course " << Colors::BOLD << request->get_course_code() << Colors::RESET
                                  << Colors::CYAN << " has been added to student's enrollment." << Colors::RESET << "\n";
                    }
                    else
                    {
                        std::cout << "\n"
                                  << Colors::YELLOW << "[!] WARNING: " << Colors::RESET << "Request approved but course not found in catalog.\n";
                    }
                }
            }
            else
            {
                std::cout << "\n"
                          << Colors::RED << "[X] ERROR: " << Colors::RESET << "Failed to approve request.\n";
            }
        }
        else if (action == 2)
        {
            // Reject
            if (erpSystem->reject_pending_request(requestId, __admin_username, remarks))
            {
                std::cout << "\n"
                          << Colors::YELLOW << "[X] Request REJECTED." << Colors::RESET << "\n";
                if (!remarks.empty())
                {
                    std::cout << Colors::DIM << "  [i] Remarks sent to student: " << remarks << Colors::RESET << "\n";
                }
            }
            else
            {
                std::cout << "\n"
                          << Colors::RED << "[X] ERROR: " << Colors::RESET << "Failed to reject request.\n";
            }
        }
        else
        {
            std::cout << "\n"
                      << Colors::RED << "[X] ERROR: " << Colors::RESET << "Invalid choice.\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "\n"
                  << Colors::RED << "[X] ERROR: " << Colors::RESET << e.what() << "\n";
    }
}

void AdminPortal::view_course_enrollments()
{
    std::cout << "\n";
    OutputFormatter::print_double_line(102);
    std::cout << Colors::BOLD << "   Course Enrollments - Students per Course" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(102);
    std::cout << "\n";

    // Get all students
    auto students = erpSystem->get_all_students();

    if (students.empty())
    {
        std::cout << Colors::YELLOW << "[!] No students found in the system." << Colors::RESET << "\n";
        return;
    }

    // Map to store course code -> list of students
    std::map<std::string, std::vector<std::shared_ptr<Student<std::string, std::string>>>> courseEnrollments;

    // Collect all course enrollments
    for (const auto &student : students)
    {
        // Get previous courses (completed courses with grades)
        auto previousCourses = student->get_previous_courses();
        for (const auto &course : previousCourses)
        {
            std::string courseCode = course.get_course_code();
            courseEnrollments[courseCode].push_back(student);
        }

        // Get current courses (ongoing)
        auto currentCourses = student->get_current_courses();
        for (const auto &course : currentCourses)
        {
            std::string courseCode = course.get_course_code();
            // Avoid duplicates if same course is in both
            if (std::find_if(courseEnrollments[courseCode].begin(),
                             courseEnrollments[courseCode].end(),
                             [&student](const auto &s)
                             { return s->get_roll_number() == student->get_roll_number(); }) == courseEnrollments[courseCode].end())
            {
                courseEnrollments[courseCode].push_back(student);
            }
        }
    }

    if (courseEnrollments.empty())
    {
        std::cout << Colors::YELLOW << "[!] No course enrollments found." << Colors::RESET << "\n";
        return;
    }

    // Display summary of all courses
    std::cout << Colors::CYAN << "Total Courses with Enrollments: " << Colors::BOLD << courseEnrollments.size() << Colors::RESET << "\n\n";
    std::cout << Colors::BOLD << std::left << std::setw(15) << "Course Code"
              << std::setw(50) << "Course Name"
              << std::setw(20) << "Students" << Colors::RESET
              << "\n";
    OutputFormatter::print_line(75);

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

        std::cout << Colors::CYAN << std::left << std::setw(15) << courseCode << Colors::RESET
                  << std::left << std::setw(50) << courseName
                  << Colors::GREEN << std::setw(20) << studentCount << Colors::RESET
                  << "\n";
    }

    OutputFormatter::print_line(75);
    std::cout << "\n";

    // Ask which course to view or all
    std::cout << Colors::YELLOW << "> Which course students would you like to view?" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  • Enter course code (e.g., CSE101, MAT101)" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  • Enter 'all' to view all courses" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  • Press Enter to skip" << Colors::RESET << "\n";
    std::cout << "\n"
              << Colors::CYAN << "Your choice: " << Colors::RESET;
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
            std::cout << "\n"
                      << Colors::RED << "[X] Error: " << Colors::RESET << "Course '" << targetCourse << "' not found or has no enrollments.\n";
            std::cout << Colors::DIM << "[i] Please check the course code from the list above." << Colors::RESET << "\n";
            return;
        }
    }

    // Display students for selected course(s)
    std::cout << "\n";
    OutputFormatter::print_double_line(102);
    if (viewAll)
    {
        std::cout << Colors::BOLD << "   Detailed Course-wise Student List (All Courses)" << Colors::RESET << "\n";
    }
    else
    {
        std::cout << Colors::BOLD << "   Student List for " << Colors::CYAN << targetCourse << Colors::RESET << "\n";
    }
    OutputFormatter::print_double_line(102);
    std::cout << "\n";

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

        std::cout << "\n";
        OutputFormatter::print_line(75);
        std::cout << Colors::YELLOW << "Course: " << Colors::BOLD << courseCode << Colors::RESET << " - " << courseName << "\n";
        std::cout << Colors::CYAN << "Instructor: " << Colors::RESET << instructor << Colors::CYAN << " | Credits: " << Colors::RESET << credits << "\n";
        std::cout << Colors::GREEN << "Total Enrolled: " << Colors::BOLD << enrolledStudents.size() << Colors::RESET << Colors::GREEN << " student(s)" << Colors::RESET << "\n";
        OutputFormatter::print_line(75);
        std::cout << "\n";

        // Display students in a table
        std::cout << Colors::BOLD << std::left
                  << std::setw(6) << "No."
                  << std::setw(18) << "Roll Number"
                  << std::setw(30) << "Name"
                  << std::setw(12) << "Grade"
                  << std::setw(10) << "Branch" << Colors::RESET
                  << "\n";
        OutputFormatter::print_line(75);

        int count = 1;
        for (const auto &student : enrolledStudents)
        {
            // Find the grade for this course
            std::string gradeStr = "Enrolled";
            bool found = false;

            // Check in previous courses
            auto prevCourses = student->get_previous_courses();
            auto prevGrades = student->get_previous_grades();
            for (size_t i = 0; i < prevCourses.size(); ++i)
            {
                if (prevCourses[i].get_course_code() == courseCode)
                {
                    double pts = prevGrades[i].get_grade_point();
                    std::string letter = prevGrades[i].get_letter_grade();
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
                auto currCourses = student->get_current_courses();
                auto currGrades = student->get_current_grades();
                for (size_t i = 0; i < currCourses.size(); ++i)
                {
                    if (currCourses[i].get_course_code() == courseCode)
                    {
                        if (i < currGrades.size())
                        {
                            double pts = currGrades[i].get_grade_point();
                            if (pts > 0.0)
                            {
                                std::string letter = currGrades[i].get_letter_grade();
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
                      << Colors::CYAN << std::setw(6) << count << Colors::RESET
                      << std::setw(18) << student->get_roll_number()
                      << std::setw(30) << student->get_name()
                      << Colors::GREEN << std::setw(12) << gradeStr << Colors::RESET
                      << std::setw(10) << student->get_branch().get_branch_code()
                      << "\n";
            count++;
        }

        OutputFormatter::print_line(75);
    }

    std::cout << "\n";
    OutputFormatter::print_double_line(102);
    std::cout << Colors::GREEN << "[OK] End of course enrollment report" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(102);
}

void AdminPortal::demo_apex_vertex_course_integration()
{
    std::cout << "\n";
    OutputFormatter::print_double_line(80);
    std::cout << Colors::BOLD << Colors::MAGENTA << "     APEX UNIVERSITY & VERTEX INSTITUTE COURSE INTEGRATION DEMO" << Colors::RESET << "\n";
    std::cout << Colors::CYAN << "     Demonstrating Template Flexibility with Different" << Colors::RESET << "\n";
    std::cout << Colors::CYAN << "     Course Code Types (String vs Integer)" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(80);
    std::cout << "\n";
    std::cout << Colors::YELLOW << "Background:" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  • Apex University students can take courses from Vertex Institute." << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  • Apex University uses string course codes (e.g., 'CSE201', 'MTH101')" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  • Vertex Institute uses integer course codes (e.g., 101, 201, 305)" << Colors::RESET << "\n\n";

    std::cout << Colors::CYAN << "This demo showcases:" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  1. Templates handling both string and integer course codes" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  2. Same student managing courses from both institutions" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  3. Type-safe operations across different course systems" << Colors::RESET << "\n\n";

    std::cout << Colors::DIM << "Press " << Colors::BOLD << "[Enter]" << Colors::RESET
              << Colors::DIM << " to start demo..." << Colors::RESET;
    std::cin.get();

    // Demo 1: Apex University Student with String Course Codes
    std::cout << "\n";
    OutputFormatter::print_double_line(68);
    std::cout << Colors::BOLD << Colors::CYAN << "DEMO 1: Apex University Student (String Course Codes)" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(68);

    Branch cse("CSE", "Computer Science and Engineering", "Engineering");
    Student<std::string, std::string> apexStudent("Rahul Kumar", "2023CSE101", cse, StudentLevel::BTECH, 2023);

    // Add Apex University courses with string codes
    Course<std::string> apexCourse1("CSE201", "Data Structures", "Dr. Sharma", 4, "Monsoon 2024");
    Course<std::string> apexCourse2("CSE202", "Object Oriented Programming", "Dr. Verma", 4, "Monsoon 2024");
    Course<std::string> apexCourse3("MTH101", "Linear Algebra", "Prof. Gupta", 3, "Monsoon 2024");

    apexStudent.add_current_course(apexCourse1);
    apexStudent.add_current_course(apexCourse2);
    apexStudent.add_current_course(apexCourse3);

    std::cout << "\n"
              << Colors::BOLD << " Student: " << Colors::CYAN << apexStudent.get_name() << Colors::RESET
              << Colors::DIM << " (" << apexStudent.get_roll_number() << ")" << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "🏫 Institution: " << Colors::GREEN << "Apex University" << Colors::RESET << "\n";
    std::cout << Colors::YELLOW << " Enrolled Apex University Courses:" << Colors::RESET << "\n";
    auto apexCourses = apexStudent.get_current_courses();
    for (const auto &course : apexCourses)
    {
        std::cout << Colors::DIM << "   • " << Colors::RESET << Colors::CYAN << course.get_course_code() << Colors::RESET
                  << " - " << course.get_course_name()
                  << Colors::DIM << " (" << course.get_credits() << " credits)" << Colors::RESET << "\n";
    }

    std::cout << "\n"
              << Colors::YELLOW << "Press Enter to continue..." << Colors::RESET;
    std::cin.get();

    // Demo 2: Same Student Concept with Integer Course Codes (Vertex Institute)
    std::cout << "\n";
    OutputFormatter::print_double_line(68);
    std::cout << Colors::BOLD << Colors::MAGENTA << "DEMO 2: Vertex Institute Student (Integer Course Codes)" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(68);

    Student<std::string, int> vertexStudent("Priya Singh", "2023CSE102", cse, StudentLevel::BTECH, 2023);

    // Add Vertex Institute courses with integer codes
    Course<int> vertexCourse1(101, "Introduction to Computing", "Prof. Mehta", 3, "Semester 1");
    Course<int> vertexCourse2(201, "Advanced Algorithms", "Dr. Joshi", 4, "Semester 1");
    Course<int> vertexCourse3(305, "Database Systems", "Prof. Reddy", 4, "Semester 1");

    vertexStudent.add_current_course(vertexCourse1);
    vertexStudent.add_current_course(vertexCourse2);
    vertexStudent.add_current_course(vertexCourse3);

    std::cout << "\n"
              << Colors::BOLD << " Student: " << Colors::MAGENTA << vertexStudent.get_name() << Colors::RESET
              << Colors::DIM << " (" << vertexStudent.get_roll_number() << ")" << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "🏫 Institution: " << Colors::GREEN << "Vertex Institute" << Colors::RESET << "\n";
    std::cout << Colors::YELLOW << " Enrolled Vertex Institute Courses:" << Colors::RESET << "\n";
    auto vertexCourses = vertexStudent.get_current_courses();
    for (const auto &course : vertexCourses)
    {
        std::cout << Colors::DIM << "   • " << Colors::RESET << Colors::MAGENTA << course.get_course_code() << Colors::RESET
                  << " - " << course.get_course_name()
                  << Colors::DIM << " (" << course.get_credits() << " credits)" << Colors::RESET << "\n";
    }

    std::cout << "\n"
              << Colors::DIM << "Press " << Colors::BOLD << "[Enter]" << Colors::RESET
              << Colors::DIM << " to continue..." << Colors::RESET;
    std::cin.get();

    // Demo 3: Apex Student taking Vertex courses (Cross-institutional enrollment)
    std::cout << "\n";
    OutputFormatter::print_double_line(68);
    std::cout << Colors::BOLD << Colors::YELLOW << "DEMO 3: Apex University Student Taking Vertex Institute Courses" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "       (Demonstrates Template Flexibility)" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(68);

    Student<std::string, int> crossStudent("Arjun Patel", "2023CSE103", cse, StudentLevel::BTECH, 2023);

    // Enroll in both Apex (string) and Vertex (int) style courses
    // But Student template is <string, int> so it handles integer course codes
    Course<int> mixCourse1(401, "Machine Learning (Vertex Institute)", "Prof. Kumar", 4, "Winter 2024");
    Course<int> mixCourse2(505, "Computer Vision (Vertex Institute)", "Dr. Saxena", 3, "Winter 2024");
    Course<int> mixCourse3(301, "Operating Systems (Vertex Institute)", "Prof. Bhatia", 4, "Winter 2024");

    crossStudent.add_current_course(mixCourse1);
    crossStudent.add_current_course(mixCourse2);
    crossStudent.add_current_course(mixCourse3);

    std::cout << "\n"
              << Colors::BOLD << " Student: " << Colors::YELLOW << crossStudent.get_name() << Colors::RESET
              << Colors::DIM << " (" << crossStudent.get_roll_number() << ")" << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "🏫 Home Institution: " << Colors::CYAN << "Apex University" << Colors::RESET
              << Colors::DIM << " (Roll format: string)" << Colors::RESET << "\n";
    std::cout << Colors::BOLD << " Taking courses at: " << Colors::MAGENTA << "Vertex Institute" << Colors::RESET
              << Colors::DIM << " (Course codes: integer)" << Colors::RESET << "\n";
    std::cout << Colors::YELLOW << " Cross-Institutional Enrollment:" << Colors::RESET << "\n";
    auto mixCourses = crossStudent.get_current_courses();
    for (const auto &course : mixCourses)
    {
        std::cout << Colors::DIM << "   • " << Colors::RESET << Colors::YELLOW << "Course " << course.get_course_code() << Colors::RESET
                  << " - " << course.get_course_name()
                  << Colors::DIM << " (" << course.get_credits() << " credits)" << Colors::RESET << "\n";
    }

    std::cout << "\n"
              << Colors::DIM << "Press " << Colors::BOLD << "[Enter]" << Colors::RESET
              << Colors::DIM << " to continue..." << Colors::RESET;
    std::cin.get();

    // Demo 4: Comparison and Template Demonstration
    std::cout << "\n";
    OutputFormatter::print_double_line(68);
    std::cout << Colors::BOLD << Colors::BLUE << "DEMO 4: Template Type Comparison" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(68);

    std::cout << "\n"
              << Colors::GREEN << "[OK] Template Instantiations Demonstrated:" << Colors::RESET << "\n\n";

    std::cout << Colors::BOLD << "1. Student<string, string> (ApexStudent) - Apex University student" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "   • Roll Number Type: std::string" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "   • Course Code Type: std::string" << Colors::RESET << "\n";
    std::cout << Colors::CYAN << "   • Example Roll: " << apexStudent.get_roll_number() << Colors::RESET << "\n";
    std::cout << Colors::CYAN << "   • Example Course: " << apexCourse1.get_course_code() << Colors::RESET << "\n\n";

    std::cout << Colors::BOLD << "2. Student<string, int> (VertexStudent) - Vertex Institute student" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "   • Roll Number Type: std::string" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "   • Course Code Type: int" << Colors::RESET << "\n";
    std::cout << Colors::MAGENTA << "   • Example Roll: " << vertexStudent.get_roll_number() << Colors::RESET << "\n";
    std::cout << Colors::MAGENTA << "   • Example Course: " << vertexCourse1.get_course_code() << Colors::RESET << "\n\n";

    std::cout << Colors::BOLD << "3. Student<string, int> - Cross-institutional" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "   • Roll Number Type: std::string (Apex format)" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "   • Course Code Type: int (Vertex format)" << Colors::RESET << "\n";
    std::cout << Colors::YELLOW << "   • Example Roll: " << crossStudent.get_roll_number() << Colors::RESET << "\n";
    std::cout << Colors::YELLOW << "   • Example Course: " << mixCourse1.get_course_code() << Colors::RESET << "\n\n";

    std::cout << Colors::DIM << "Press " << Colors::BOLD << "[Enter]" << Colors::RESET
              << Colors::DIM << " to continue..." << Colors::RESET;
    std::cin.get();

    // Demo 5: Grade Assignment and CGPA Calculation
    std::cout << "\n";
    OutputFormatter::print_double_line(68);
    std::cout << Colors::BOLD << Colors::GREEN << "DEMO 5: Grade Assignment (Works with Both Systems)" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(68);

    // Assign grades to Apex student
    Grade g1(9.0); // A
    Grade g2(8.5); // A-
    Grade g3(9.5); // A+

    apexStudent.update_grade(apexCourse1.get_course_code(), g1);
    apexStudent.update_grade(apexCourse2.get_course_code(), g2);
    apexStudent.update_grade(apexCourse3.get_course_code(), g3);

    std::cout << "\n"
              << Colors::BOLD << " " << apexStudent.get_name() << " (Apex University) - Grades:" << Colors::RESET << "\n";
    auto apexGrades = apexStudent.get_current_grades();
    for (size_t i = 0; i < apexCourses.size(); ++i)
    {
        std::cout << Colors::CYAN << "   • " << apexCourses[i].get_course_code() << ": " << Colors::RESET
                  << Colors::GREEN << apexGrades[i].get_letter_grade() << Colors::RESET << Colors::DIM << " (GP: "
                  << std::fixed << std::setprecision(1) << apexGrades[i].get_grade_point() << ")" << Colors::RESET << "\n";
    }
    std::cout << Colors::BOLD << "   CGPA: " << Colors::GREEN << std::fixed << std::setprecision(2) << apexStudent.calculate_cgpa() << Colors::RESET << "\n";

    // Assign grades to Vertex student
    Grade g4(8.0); // B+
    Grade g5(9.0); // A
    Grade g6(7.5); // B

    vertexStudent.update_grade(vertexCourse1.get_course_code(), g4);
    vertexStudent.update_grade(vertexCourse2.get_course_code(), g5);
    vertexStudent.update_grade(vertexCourse3.get_course_code(), g6);

    std::cout << "\n"
              << Colors::BOLD << " " << vertexStudent.get_name() << " (Vertex Institute) - Grades:" << Colors::RESET << "\n";
    auto vertexGrades = vertexStudent.get_current_grades();
    for (size_t i = 0; i < vertexCourses.size(); ++i)
    {
        std::cout << Colors::MAGENTA << "   • Course " << vertexCourses[i].get_course_code() << ": " << Colors::RESET
                  << Colors::GREEN << vertexGrades[i].get_letter_grade() << Colors::RESET << Colors::DIM << " (GP: "
                  << std::fixed << std::setprecision(1) << vertexGrades[i].get_grade_point() << ")" << Colors::RESET << "\n";
    }
    std::cout << Colors::BOLD << "   CGPA: " << Colors::GREEN << std::fixed << std::setprecision(2) << vertexStudent.calculate_cgpa() << Colors::RESET << "\n";

    std::cout << "\n"
              << Colors::DIM << "Press " << Colors::BOLD << "[Enter]" << Colors::RESET
              << Colors::DIM << " to continue..." << Colors::RESET;
    std::cin.get();

    // Summary
    std::cout << "\n";
    OutputFormatter::print_double_line(68);
    std::cout << Colors::BOLD << Colors::MAGENTA << "SUMMARY: Template Benefits Demonstrated" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(68);
    std::cout << "\n";

    std::cout << Colors::GREEN << "[OK] " << Colors::BOLD << "Type Safety:" << Colors::RESET << " Compile-time type checking prevents mixing incompatible types\n";
    std::cout << Colors::GREEN << "[OK] " << Colors::BOLD << "Flexibility:" << Colors::RESET << " Same Student class handles multiple course code formats\n";
    std::cout << Colors::GREEN << "[OK] " << Colors::BOLD << "Reusability:" << Colors::RESET << " No code duplication for different institutional systems\n";
    std::cout << Colors::GREEN << "[OK] " << Colors::BOLD << "Scalability:" << Colors::RESET << " Easy to add new institutions with different formats\n";
    std::cout << Colors::GREEN << "[OK] " << Colors::BOLD << "Performance:" << Colors::RESET << " Zero runtime overhead compared to runtime polymorphism\n\n";

    std::cout << Colors::CYAN << "Real-world Application:" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  • Apex University students can seamlessly take Vertex Institute courses" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  • System tracks courses from both institutions correctly" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  • Type system ensures data integrity across institutions" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  • Same codebase serves multiple course numbering systems" << Colors::RESET << "\n\n";

    OutputFormatter::print_double_line(80);
    std::cout << "\n"
              << Colors::BOLD << Colors::GREEN << "[OK] Demo Complete" << Colors::RESET << " - "
              << Colors::DIM << "Press " << Colors::BOLD << "[Enter]" << Colors::RESET
              << Colors::DIM << " to return" << Colors::RESET << "\n";
    OutputFormatter::print_double_line(80);
    std::cin.get();
}
