#include "StudentPortal.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>
#include <algorithm>
#include <cctype>

// Helper function to convert string to uppercase
static std::string toUpperCase(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c)
                   { return std::toupper(c); });
    return result;
}

StudentPortal::StudentPortal(const std::string &roll,
                             ERPSystem<std::string, std::string> *erp,
                             CourseCatalog *catalog,
                             SemesterManager *semMgr)
    : rollNumber(roll), erpSystem(erp), courseCatalog(catalog),
      semesterManager(semMgr)
{
}

Student<std::string, std::string> StudentPortal::getCurrentStudent()
{
    auto studentPtr = erpSystem->findStudent(rollNumber);
    if (!studentPtr)
    {
        throw std::runtime_error("Student not found in ERP system");
    }
    return *studentPtr;
}

void StudentPortal::displayMenu()
{
    auto student = getCurrentStudent();
    double cgpa = semesterManager->calculateCGPA(student);
    int creditsCompleted = semesterManager->getTotalCreditsCompleted(student);

    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                STUDENT PORTAL - IIIT-Delhi ERP                 ║\n";
    std::cout << "║  " << std::left << std::setw(59) << student.getName() << "  ║\n";
    std::cout << "║  Roll: " << std::left << std::setw(52) << rollNumber << "     ║\n";
    std::cout << "║  CGPA: " << std::fixed << std::setprecision(2) << std::left << std::setw(10) << cgpa
              << "  Credits: " << std::left << std::setw(35) << creditsCompleted << "║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  PROFILE & ENROLLMENT                                          ║\n";
    std::cout << "║   1. View Personal Information                                 ║\n";
    std::cout << "║   2. View My Enrollments                                       ║\n";
    std::cout << "║   3. View Pending Course Requests                              ║\n";
    std::cout << "║   4. View My Grades                                            ║\n";
    std::cout << "║   5. View Transcript                                           ║\n";
    std::cout << "║   6. Download Transcript (Save to file)                        ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  COURSE REGISTRATION                                           ║\n";
    std::cout << "║   7. Browse Course Catalog                                     ║\n";
    std::cout << "║   8. Search Courses                                            ║\n";
    std::cout << "║   9. View Course Details & Syllabus                            ║\n";
    std::cout << "║  10. Register for Course "
              << (semesterManager->isCourseAddEnabled() ? "(OPEN)  " : "(CLOSED)")
              << "                              ║\n";
    std::cout << "║  11. Drop Course "
              << (semesterManager->isCourseDropEnabled() ? "(ENABLED) " : "(DISABLED)")
              << "                                    ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  ACCOUNT                                                       ║\n";
    std::cout << "║  12. Change Password                                           ║\n";
    std::cout << "║  13. Logout                                                    ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    std::cout << "Enter your choice: ";
}

void StudentPortal::run()
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
            viewPersonalInfo();
            break;
        case 2:
            viewEnrollments();
            break;
        case 3:
            viewPendingRequests();
            break;
        case 4:
            viewGrades();
            break;
        case 5:
            viewTranscript();
            break;
        case 6:
            downloadTranscript();
            break;
        case 7:
            viewCoursesList();
            break;
        case 8:
            searchCourses();
            break;
        case 9:
            viewCourseDetails();
            break;
        case 10:
            registerForCourse();
            break;
        case 11:
            dropCourse();
            break;
        case 12:
            changePassword();
            break;
        case 13:
            std::cout << "\nLogging out from student portal...\n";
            return;
        default:
            std::cout << "\nInvalid choice. Please try again.\n";
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
}

void StudentPortal::viewPersonalInfo()
{
    auto student = getCurrentStudent();

    // Convert StudentLevel enum to string
    std::string levelStr;
    switch (student.getLevel())
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

    std::cout << "\n=== Personal Information ===\n";
    std::cout << "Roll Number: " << student.getRollNumber() << "\n";
    std::cout << "Name: " << student.getName() << "\n";
    std::cout << "Branch: " << student.getBranch().getBranchName() << "\n";
    std::cout << "Level: " << levelStr << "\n";
    std::cout << "Start Year: " << student.getStartingYear() << "\n";

    int creditsCompleted = semesterManager->getTotalCreditsCompleted(student);
    double cgpa = semesterManager->calculateCGPA(student);

    std::cout << "\nAcademic Summary:\n";
    std::cout << "Credits Completed: " << creditsCompleted << "\n";
    std::cout << "CGPA: " << std::fixed << std::setprecision(2) << cgpa << "\n";
}

void StudentPortal::viewEnrollments()
{
    auto student = getCurrentStudent();
    auto currentCourses = student.getCurrentCourses();
    auto previousCourses = student.getPreviousCourses();

    std::cout << "\n=== My Enrollments ===\n";

    std::cout << "\nCurrent Courses: " << currentCourses.size() << "\n";
    if (!currentCourses.empty())
    {
        std::cout << std::left << std::setw(12) << "Course Code"
                  << std::setw(35) << "Course Name"
                  << std::setw(10) << "Credits"
                  << std::setw(12) << "Instructor" << "\n";
        std::cout << std::string(69, '-') << "\n";

        for (const auto &course : currentCourses)
        {
            std::cout << std::left << std::setw(12) << course.getCourseCode()
                      << std::setw(35) << course.getCourseName()
                      << std::setw(10) << course.getCredits()
                      << std::setw(12) << course.getInstructor() << "\n";
        }
    }

    std::cout << "\nCompleted Courses: " << previousCourses.size() << "\n";
    if (!previousCourses.empty())
    {
        auto previousGrades = student.getPreviousGrades();
        std::cout << std::left << std::setw(12) << "Course Code"
                  << std::setw(35) << "Course Name"
                  << std::setw(10) << "Credits"
                  << std::setw(8) << "Grade" << "\n";
        std::cout << std::string(65, '-') << "\n";

        for (size_t i = 0; i < previousCourses.size(); ++i)
        {
            const auto &course = previousCourses[i];
            std::cout << std::left << std::setw(12) << course.getCourseCode()
                      << std::setw(35) << course.getCourseName()
                      << std::setw(10) << course.getCredits();

            if (i < previousGrades.size())
            {
                std::cout << std::setw(8) << previousGrades[i].getLetterGrade();
            }
            std::cout << "\n";
        }
    }

    if (currentCourses.empty() && previousCourses.empty())
    {
        std::cout << "No course enrollments found.\n";
        return;
    }
}

void StudentPortal::viewPendingRequests()
{
    std::cout << "\n=== My Pending Course Requests ===\n";

    try
    {
        auto pendingRequests = erpSystem->getPendingRequestsByStudent(rollNumber);

        if (pendingRequests.empty())
        {
            std::cout << "\nNo pending course registration requests found.\n";
            return;
        }

        std::cout << "\nTotal Requests: " << pendingRequests.size() << "\n\n";
        std::cout << std::left << std::setw(8) << "Req ID"
                  << std::setw(12) << "Course"
                  << std::setw(35) << "Course Name"
                  << std::setw(12) << "Status"
                  << std::setw(20) << "Request Date" << "\n";
        std::cout << std::string(87, '-') << "\n";

        for (const auto &request : pendingRequests)
        {
            std::cout << std::left << std::setw(8) << request.getRequestId()
                      << std::setw(12) << request.getCourseCode()
                      << std::setw(35) << request.getCourseName()
                      << std::setw(12) << requestStatusToString(request.getStatus())
                      << std::setw(20) << request.getRequestTimeString() << "\n";

            // Show remarks if available
            if (!request.getRemarks().empty())
            {
                std::cout << "  └─ Remarks: " << request.getRemarks() << "\n";
            }
        }

        // Count by status
        int pending = 0, approved = 0, rejected = 0;
        for (const auto &req : pendingRequests)
        {
            switch (req.getStatus())
            {
            case RequestStatus::PENDING:
                pending++;
                break;
            case RequestStatus::APPROVED:
                approved++;
                break;
            case RequestStatus::REJECTED:
                rejected++;
                break;
            }
        }

        std::cout << "\n─ Summary ────────────────────────\n";
        std::cout << "Pending: " << pending << " | Approved: " << approved << " | Rejected: " << rejected << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error: " << e.what() << "\n";
    }
}

void StudentPortal::registerForCourse()
{
    if (!semesterManager->isCourseAddEnabled())
    {
        std::cout << "\nX Course registration is currently closed.\n";
        std::cout << "   Please contact administration for more information.\n";
        return;
    }

    auto student = getCurrentStudent();
    auto currentCourses = student.getCurrentCourses();

    std::cout << "\n=== Register for Course ===\n";
    std::cout << "Enter Course Code: ";
    std::string courseCode;
    std::getline(std::cin, courseCode);
    courseCode = toUpperCase(courseCode); // Convert to uppercase

    // Check if course exists in catalog
    if (!courseCatalog->courseExists(courseCode))
    {
        std::cout << "\nX Course not found in catalog.\n";
        return;
    }

    // Check if already enrolled
    for (const auto &course : currentCourses)
    {
        if (course.getCourseCode() == courseCode)
        {
            std::cout << "\nX You are already enrolled in this course.\n";
            return;
        }
    }

    // Check if already have a pending request for this course
    auto pendingRequests = erpSystem->getPendingRequestsByStudent(rollNumber);
    for (const auto &req : pendingRequests)
    {
        if (req.getCourseCode() == courseCode && req.getStatus() == RequestStatus::PENDING)
        {
            std::cout << "\nX You already have a pending request for this course.\n";
            std::cout << "  Request ID: " << req.getRequestId() << "\n";
            std::cout << "  Status: " << requestStatusToString(req.getStatus()) << "\n";
            return;
        }
    }

    // Display course info
    courseCatalog->displayCourse(courseCode);

    // Calculate current credits
    int currentCredits = 0;
    for (const auto &course : currentCourses)
    {
        currentCredits += course.getCredits();
    }

    int courseCredits = courseCatalog->getCourseCredits(courseCode);
    std::cout << "\nYour current credits: " << currentCredits << "/16\n";
    std::cout << "Course credits: " << courseCredits << "\n";
    std::cout << "Credits after registration: " << (currentCredits + courseCredits) << "/16\n";

    if (currentCredits + courseCredits > 16)
    {
        std::cout << "\nX Not enough credits available for this semester.\n";
        std::cout << "   Maximum 16 credits per semester.\n";
        return;
    }

    // Confirm registration
    std::cout << "\nDo you want to submit the course registration request? (yes/no): ";
    std::string confirm;
    std::getline(std::cin, confirm);
    confirm = toUpperCase(confirm);

    if (confirm != "YES")
    {
        std::cout << "\nRegistration cancelled.\n";
        return;
    }

    // Submit pending request
    try
    {
        int requestId = erpSystem->addPendingRequest(rollNumber, courseCode);
        std::cout << "\n=> Course registration request submitted successfully!\n";
        std::cout << "  Request ID: " << requestId << "\n";
        std::cout << "  Course Code: " << courseCode << "\n";
        std::cout << "  Status: PENDING\n";
        std::cout << "\n  Please wait for admin approval.\n";
        std::cout << "  You can check the status in 'View My Enrollments' menu.\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\nX Error submitting request: " << e.what() << "\n";
    }
}

void StudentPortal::dropCourse()
{
    if (!semesterManager->isCourseDropEnabled())
    {
        std::cout << "\nX Course drop is currently disabled.\n";
        std::cout << "   Please contact administration for more information.\n";
        return;
    }

    std::cout << "\n=== Drop Course ===\n";
    std::cout << "Enter Course Code to drop: ";
    std::string courseCode;
    std::getline(std::cin, courseCode);
    courseCode = toUpperCase(courseCode); // Convert to uppercase

    std::cout << "\nAre you sure you want to drop this course? (yes/no): ";
    std::string confirm;
    std::getline(std::cin, confirm);
    confirm = toUpperCase(confirm); // Convert to uppercase

    if (confirm == "YES")
    {
        std::cout << "\n⚠ Course drop functionality is under development.\n";
        std::cout << "   Please contact administration to drop courses.\n";
    }
    else
    {
        std::cout << "Drop cancelled.\n";
    }
}

void StudentPortal::viewCoursesList()
{
    auto student = getCurrentStudent();

    // Convert StudentLevel enum to string
    std::string levelStr;
    switch (student.getLevel())
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

    std::cout << "\n=== Available Courses for " << levelStr << " ===\n";

    auto courses = courseCatalog->getCoursesByLevel(levelStr);

    if (courses.empty())
    {
        std::cout << "No courses available for your level.\n";
        return;
    }

    std::cout << "\nTotal Courses: " << courses.size() << "\n\n";
    std::cout << std::left << std::setw(12) << "Code"
              << std::setw(40) << "Name"
              << std::setw(10) << "Credits"
              << std::setw(12) << "Type" << "\n";
    std::cout << std::string(74, '-') << "\n";

    for (const auto &course : courses)
    {
        std::cout << std::left << std::setw(12) << course.courseCode
                  << std::setw(40) << course.courseName
                  << std::setw(10) << course.credits
                  << std::setw(12) << course.courseType << "\n";
    }
}

void StudentPortal::viewCourseDetails()
{
    std::cout << "\n=== Course Details ===\n";
    std::cout << "Enter Course Code: ";
    std::string courseCode;
    std::getline(std::cin, courseCode);
    courseCode = toUpperCase(courseCode); // Convert to uppercase

    if (!courseCatalog->courseExists(courseCode))
    {
        std::cout << "\nX Course not found.\n";
        return;
    }

    courseCatalog->displayCourse(courseCode);
}

void StudentPortal::searchCourses()
{
    std::cout << "\n=== Search Courses ===\n";
    std::cout << "Enter search query: ";
    std::string query;
    std::getline(std::cin, query);

    auto results = courseCatalog->searchCourses(query);

    if (results.empty())
    {
        std::cout << "\nNo courses found matching your query.\n";
        return;
    }

    std::cout << "\nFound " << results.size() << " course(s):\n\n";
    std::cout << std::left << std::setw(12) << "Code"
              << std::setw(40) << "Name"
              << std::setw(10) << "Credits" << "\n";
    std::cout << std::string(62, '-') << "\n";

    for (const auto &course : results)
    {
        std::cout << std::left << std::setw(12) << course.courseCode
                  << std::setw(40) << course.courseName
                  << std::setw(10) << course.credits << "\n";
    }
}

void StudentPortal::viewGrades()
{
    auto student = getCurrentStudent();
    auto currentCourses = student.getCurrentCourses();
    auto currentGrades = student.getCurrentGrades();
    auto previousCourses = student.getPreviousCourses();
    auto previousGrades = student.getPreviousGrades();

    std::cout << "\n=== My Grades ===\n";

    if (previousCourses.empty() && currentCourses.empty())
    {
        std::cout << "No enrollments found.\n";
        return;
    }

    // Show completed courses with grades
    if (!previousCourses.empty())
    {
        std::cout << "\nCompleted Courses:\n";
        std::cout << std::left << std::setw(12) << "Course"
                  << std::setw(35) << "Name"
                  << std::setw(10) << "Credits"
                  << std::setw(8) << "Grade"
                  << std::setw(8) << "Points" << "\n";
        std::cout << std::string(73, '-') << "\n";

        for (size_t i = 0; i < previousCourses.size(); ++i)
        {
            const auto &course = previousCourses[i];
            std::cout << std::left << std::setw(12) << course.getCourseCode()
                      << std::setw(35) << course.getCourseName()
                      << std::setw(10) << course.getCredits();

            if (i < previousGrades.size())
            {
                const auto &grade = previousGrades[i];
                std::cout << std::setw(8) << grade.getLetterGrade()
                          << std::setw(8) << std::fixed << std::setprecision(2)
                          << grade.getGradePoint();
            }
            else
            {
                std::cout << std::setw(8) << "N/A"
                          << std::setw(8) << "N/A";
            }
            std::cout << "\n";
        }
    }

    // Show current courses (usually without grades yet)
    if (!currentCourses.empty())
    {
        std::cout << "\nCurrent Courses:\n";
        std::cout << std::left << std::setw(12) << "Course"
                  << std::setw(35) << "Name"
                  << std::setw(10) << "Credits"
                  << std::setw(8) << "Grade"
                  << std::setw(8) << "Points" << "\n";
        std::cout << std::string(73, '-') << "\n";

        for (size_t i = 0; i < currentCourses.size(); ++i)
        {
            const auto &course = currentCourses[i];
            std::cout << std::left << std::setw(12) << course.getCourseCode()
                      << std::setw(35) << course.getCourseName()
                      << std::setw(10) << course.getCredits();

            if (i < currentGrades.size())
            {
                const auto &grade = currentGrades[i];
                std::cout << std::setw(8) << grade.getLetterGrade()
                          << std::setw(8) << std::fixed << std::setprecision(2)
                          << grade.getGradePoint();
            }
            else
            {
                std::cout << std::setw(8) << "Pending"
                          << std::setw(8) << "-";
            }
            std::cout << "\n";
        }
    }

    // Calculate and show CGPA
    double cgpa = student.calculateCGPA();
    int creditsCompleted = semesterManager->getTotalCreditsCompleted(student);

    std::cout << "\n"
              << std::string(73, '=') << "\n";
    std::cout << "CGPA: " << std::fixed << std::setprecision(2) << cgpa
              << " | Credits Completed: " << creditsCompleted << "\n";
}

void StudentPortal::viewTranscript()
{
    auto student = getCurrentStudent();
    std::string transcript = semesterManager->generateTranscript(student);
    std::cout << transcript;
}

void StudentPortal::downloadTranscript()
{
    auto student = getCurrentStudent();
    std::string transcript = semesterManager->generateTranscript(student);

    std::string filename = "transcript_" + rollNumber + ".txt";
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cout << "\nX Error: Could not create transcript file.\n";
        return;
    }

    file << transcript;
    file.close();

    std::cout << "\n=> Transcript saved successfully!\n";
    std::cout << "   File: " << filename << "\n";
}

void StudentPortal::changePassword()
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

    if (newPassword.length() < 6)
    {
        std::cout << "\nX Password must be at least 6 characters long.\n";
        return;
    }

    if (Auth::changePassword(rollNumber, oldPassword, newPassword, UserType::STUDENT))
    {
        std::cout << "\n=> Password changed successfully!\n";
    }
    else
    {
        std::cout << "\nX Failed to change password. Check your current password.\n";
    }
}
