#include <iostream>
#include <memory>
#include <limits>
#include "Auth.h"
#include "Database.h"
#include "CourseCatalog.h"
#include "SemesterManager.h"
#include "ERPSystem.h"
#include "AdminPortal.h"
#include "StudentPortal.h"
#include "CSVHandler.h"
#include "Colors.h"
#include "OutputFormatter.h"

void displayWelcome()
{
    std::cout << Colors::BOLD << Colors::CYAN << "\n";
    std::cout << std::string(80, '=') << "\n";
    std::cout << std::string(22, ' ') << "UNIVERSITY ERP SYSTEM\n";
    std::cout << std::string(21, ' ') << "Academic Records & Enrollment\n";
    std::cout << std::string(80, '=') << "\n";
    std::cout << Colors::RESET;
}

UserType getUserType()
{
    int choice;

    while (true)
    {
        OutputFormatter::print_sub_header("SELECT USER TYPE");

        std::cout << "\n";
        OutputFormatter::print_menu_item(1, "Student Portal", Colors::CYAN);
        OutputFormatter::print_menu_item(2, "Admin/Authority Portal", Colors::YELLOW);
        OutputFormatter::print_menu_item(3, "Exit", Colors::RED);

        std::cout << "\n"
                  << Colors::CYAN << "Enter your choice: " << Colors::RESET;

        if (!(std::cin >> choice))
        {
            // Check if we've reached EOF (for piped input)
            if (std::cin.eof())
            {
                std::cout << "\nEnd of input reached. Exiting...\n";
                exit(0);
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << Colors::RED << "[X] Invalid input. Please enter a number." << Colors::RESET << "\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1)
        {
            return UserType::STUDENT;
        }
        else if (choice == 2)
        {
            return UserType::ADMIN;
        }
        else if (choice == 3)
        {
            std::cout << "\n"
                      << Colors::GREEN << "[OK] Thank you for using University ERP System!" << Colors::RESET << "\n";
            exit(0);
        }
        else
        {
            std::cout << Colors::YELLOW << "[!] Invalid choice. Please try again." << Colors::RESET << "\n";
        }
    }
}

bool performLogin(UserType userType, std::string &username)
{
    int attempts = 0;
    const int maxAttempts = 3;

    std::string title = (userType == UserType::STUDENT ? "STUDENT" : "ADMIN");
    OutputFormatter::print_section(title + " LOGIN");

    while (attempts < maxAttempts)
    {
        std::cout << Colors::CYAN << "  Username/Roll No: " << Colors::RESET;
        std::getline(std::cin, username);

        std::cout << Colors::CYAN << "  Password: " << Colors::RESET;
        std::string password;
        std::getline(std::cin, password);

        if (Auth::authenticate(username, password, userType))
        {
            std::cout << "\n"
                      << Colors::GREEN << Colors::BOLD
                      << "[OK] Login successful. Welcome, " << username << "."
                      << Colors::RESET << "\n";
            return true;
        }
        else
        {
            attempts++;
            std::cout << "  " << Colors::RED << "[X] Invalid credentials. " << Colors::RESET;
            if (attempts < maxAttempts)
            {
                std::cout << Colors::YELLOW << "Attempts remaining: "
                          << (maxAttempts - attempts) << Colors::RESET << "\n\n";
            }
            else
            {
                std::cout << Colors::RED << "Maximum attempts reached." << Colors::RESET << "\n";
            }
        }
    }

    return false;
}

void initializeSystem()
{
    std::cout << "\n"
              << Colors::CYAN << "[i] Initializing system..." << Colors::RESET << "\n";

    // Initialize authentication files
    Auth::initializeCredentialFiles();

    // Initialize database
    Database::initializeDatabaseFiles();

    std::cout << Colors::GREEN << "[OK] System initialized successfully." << Colors::RESET << "\n";
}

int main()
{
    try
    {
        displayWelcome();
        initializeSystem();

        // Initialize core components
        auto erpSystem = std::make_shared<ERPSystem<std::string, std::string>>();
        auto courseCatalog = std::make_shared<CourseCatalog>();
        auto semesterManager = std::make_shared<SemesterManager>(courseCatalog.get());

        // Load existing students from database (if any)
        try
        {
            auto students = Database::loadStudents<std::string, std::string>();

            // If no students in database, auto-load demo_students.csv
            if (students.empty())
            {
                std::cout << "\n"
                          << Colors::YELLOW << "[i] No students found in database. Loading demo_students.csv..." << Colors::RESET << "\n";
                try
                {
                    CSVHandler<std::string, std::string> csvHandler("demo_students.csv");
                    auto csvStudentsPtr = csvHandler.read_students();

                    std::vector<Student<std::string, std::string>> csvStudents;
                    csvStudents.reserve(csvStudentsPtr.size());

                    if (!csvStudentsPtr.empty())
                    {
                        // Convert shared_ptr to values and add to ERP system
                        for (const auto &studentPtr : csvStudentsPtr)
                        {
                            csvStudents.push_back(*studentPtr);
                            erpSystem->add_student(studentPtr);
                        }

                        // Save to database
                        Database::saveStudents(csvStudents);

                        // Save insertion order to file
                        erpSystem->save_insertion_order_to_file();

                        std::cout << Colors::GREEN << "[OK] Successfully loaded " << csvStudents.size()
                                  << " students from demo_students.csv." << Colors::RESET << "\n";
                        students = csvStudents; // Update for the rest of the loading logic
                    }
                }
                catch (const std::exception &csvError)
                {
                    std::cout << Colors::YELLOW << "[!] WARNING: Could not load demo_students.csv: "
                              << csvError.what() << Colors::RESET << "\n";
                }
            }
            else
            {
                std::cout << "\n"
                          << Colors::CYAN << "[i] Loaded " << students.size() << " students from database." << Colors::RESET << "\n";
            }

            // Convert to shared_ptr and maintain the order from database
            std::vector<std::shared_ptr<Student<std::string, std::string>>> studentPtrs;
            studentPtrs.reserve(students.size());

            for (auto &student : students)
            {
                // Only add if not already added (to avoid duplicates from CSV loading above)
                if (erpSystem->find_student(student.get_roll_number()) == nullptr)
                {
                    auto ptr = std::make_shared<Student<std::string, std::string>>(student);
                    erpSystem->add_student(ptr);
                    studentPtrs.push_back(ptr);
                }
                else
                {
                    // Already added, just get the pointer
                    studentPtrs.push_back(erpSystem->find_student(student.get_roll_number()));
                }
            }

            // Don't set as sorted when loading from database
            // This ensures insertion order iterator shows the load order, not sorted order
            // Users must explicitly sort (option 9) to enable sorted order viewing
            if (!studentPtrs.empty())
            {
                // Load insertion order from file if it exists
                // This will reorganize enrollmentRecords to match the saved insertion order
                erpSystem->load_insertion_order_from_file();

                std::cout << Colors::GREEN << "[OK] Students loaded from database." << Colors::RESET << "\n";
                std::cout << Colors::DIM << "Note: Use Sort option (9) to enable sorted order viewing." << Colors::RESET << "\n";
            }
        }
        catch (const std::exception &e)
        {
            std::cout << Colors::YELLOW << "[!] NOTE: Could not load existing students: " << e.what() << Colors::RESET << "\n";
        }

        // Main application loop
        while (true)
        {
            UserType userType = getUserType();
            std::string username;

            if (performLogin(userType, username))
            {
                if (userType == UserType::ADMIN)
                {
                    // Launch Admin Portal
                    AdminPortal adminPortal(username, erpSystem.get(),
                                            courseCatalog.get(), semesterManager.get());
                    adminPortal.run();

                    // Save students to database after admin session
                    try
                    {
                        auto students = erpSystem->get_all_students();
                        std::vector<Student<std::string, std::string>> studentVec;
                        for (const auto &studentPtr : students)
                        {
                            studentVec.push_back(*studentPtr);
                        }
                        Database::saveStudents(studentVec);
                        std::cout << "\n"
                                  << Colors::GREEN << "[OK] Database saved successfully." << Colors::RESET << "\n";
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "\n"
                                  << Colors::RED << "[X] WARNING: Could not save database: " << e.what() << Colors::RESET << "\n";
                    }
                }
                else
                {
                    // Launch Student Portal
                    StudentPortal studentPortal(username, erpSystem.get(),
                                                courseCatalog.get(), semesterManager.get());
                    studentPortal.run();
                }
            }
            else
            {
                std::cout << "\n"
                          << Colors::RED << "[X] Login failed. Returning to main menu..." << Colors::RESET << "\n";
            }

            std::cout << "\n"
                      << Colors::DIM << "Press " << Colors::BOLD << "[Enter]" << Colors::RESET
                      << Colors::DIM << " to continue" << Colors::RESET;
            std::cin.get();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "\n"
                  << Colors::RED << Colors::BOLD << "[X] FATAL ERROR: " << e.what() << Colors::RESET << "\n";
        return 1;
    }

    return 0;
}
