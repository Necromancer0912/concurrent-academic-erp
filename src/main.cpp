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

void displayWelcome()
{
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                               ║\n";
    std::cout << "║         IIIT-DELHI UNIVERSITY ERP SYSTEM                      ║\n";
    std::cout << "║         Object-Oriented Programming & Design                  ║\n";
    std::cout << "║         Assignment 4 - Templates and Threads                  ║\n";
    std::cout << "║                                                               ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n";
}

UserType getUserType()
{
    int choice;

    while (true)
    {
        std::cout << "\n┌─ Select User Type ───────────────────┐\n";
        std::cout << "│ 1. Student Portal                    │\n";
        std::cout << "│ 2. Admin/Authority Portal            │\n";
        std::cout << "│ 3. Exit                              │\n";
        std::cout << "└──────────────────────────────────────┘\n";
        std::cout << "Enter your choice: ";

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
            std::cout << "Invalid input. Please enter a number.\n";
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
            std::cout << "\nThank you for using IIIT-Delhi ERP System!\n";
            exit(0);
        }
        else
        {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

bool performLogin(UserType userType, std::string &username)
{
    int attempts = 0;
    const int maxAttempts = 3;

    std::cout << "\n┌─ " << (userType == UserType::STUDENT ? "Student" : "Admin")
              << " Login ────────────────────────┐\n";

    while (attempts < maxAttempts)
    {
        std::cout << "│ Username/Roll No: ";
        std::getline(std::cin, username);

        std::cout << "│ Password: ";
        std::string password;
        std::getline(std::cin, password);

        if (Auth::authenticate(username, password, userType))
        {
            std::cout << "└────────────────────────────────────────┘\n";
            std::cout << "\nLogin successful. Welcome, " << username << ".\n";
            return true;
        }
        else
        {
            attempts++;
            std::cout << "│ ERROR: Invalid credentials. ";
            if (attempts < maxAttempts)
            {
                std::cout << "Attempts remaining: " << (maxAttempts - attempts) << "\n";
            }
            else
            {
                std::cout << "Maximum attempts reached.\n";
                std::cout << "└────────────────────────────────────────┘\n";
            }
        }
    }

    return false;
}

void initializeSystem()
{
    std::cout << "\nInitializing system...\n";

    // Initialize authentication files
    Auth::initializeCredentialFiles();

    // Initialize database
    Database::initializeDatabaseFiles();

    std::cout << "System initialized successfully.\n";
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
                std::cout << "\nNo students found in database. Loading demo_students.csv...\n";
                try
                {
                    CSVHandler<std::string, std::string> csvHandler("demo_students.csv");
                    auto csvStudentsPtr = csvHandler.readStudents();

                    std::vector<Student<std::string, std::string>> csvStudents;
                    csvStudents.reserve(csvStudentsPtr.size());

                    if (!csvStudentsPtr.empty())
                    {
                        // Convert shared_ptr to values and add to ERP system
                        for (const auto &studentPtr : csvStudentsPtr)
                        {
                            csvStudents.push_back(*studentPtr);
                            erpSystem->addStudent(studentPtr);
                        }

                        // Save to database
                        Database::saveStudents(csvStudents);

                        // Save insertion order to file
                        erpSystem->saveInsertionOrderToFile();

                        std::cout << "Successfully loaded " << csvStudents.size()
                                  << " students from demo_students.csv.\n";
                        students = csvStudents; // Update for the rest of the loading logic
                    }
                }
                catch (const std::exception &csvError)
                {
                    std::cout << "WARNING: Could not load demo_students.csv: "
                              << csvError.what() << "\n";
                }
            }
            else
            {
                std::cout << "\nLoaded " << students.size() << " students from database.\n";
            }

            // Convert to shared_ptr and maintain the order from database
            std::vector<std::shared_ptr<Student<std::string, std::string>>> studentPtrs;
            studentPtrs.reserve(students.size());

            for (auto &student : students)
            {
                // Only add if not already added (to avoid duplicates from CSV loading above)
                if (erpSystem->findStudent(student.getRollNumber()) == nullptr)
                {
                    auto ptr = std::make_shared<Student<std::string, std::string>>(student);
                    erpSystem->addStudent(ptr);
                    studentPtrs.push_back(ptr);
                }
                else
                {
                    // Already added, just get the pointer
                    studentPtrs.push_back(erpSystem->findStudent(student.getRollNumber()));
                }
            }

            // Don't set as sorted when loading from database
            // This ensures insertion order iterator shows the load order, not sorted order
            // Users must explicitly sort (option 9) to enable sorted order viewing
            if (!studentPtrs.empty())
            {
                // Load insertion order from file if it exists
                // This will reorganize enrollmentRecords to match the saved insertion order
                erpSystem->loadInsertionOrderFromFile();

                std::cout << "Students loaded from database.\n";
                std::cout << "Note: Use Sort option (9) to enable sorted order viewing.\n";
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "NOTE: Could not load existing students: " << e.what() << "\n";
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
                        auto students = erpSystem->getAllStudents();
                        std::vector<Student<std::string, std::string>> studentVec;
                        for (const auto &studentPtr : students)
                        {
                            studentVec.push_back(*studentPtr);
                        }
                        Database::saveStudents(studentVec);
                        std::cout << "\nDatabase saved successfully.\n";
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "\nWARNING: Could not save database: " << e.what() << "\n";
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
                std::cout << "\nLogin failed. Returning to main menu...\n";
            }

            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "\nFATAL ERROR: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
