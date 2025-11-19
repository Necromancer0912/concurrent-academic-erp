#include "StudentPortal.h"
#include "Colors.h"
#include "OutputFormatter.h"
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
    : __roll_number(roll), __erp_system(erp), courseCatalog(catalog),
      semesterManager(semMgr)
{
}

Student<std::string, std::string> StudentPortal::getCurrentStudent()
{
    auto studentPtr = __erp_system->find_student(__roll_number);
    if (!studentPtr)
    {
        throw std::runtime_error("Student not found in ERP system");
    }
    return *studentPtr;
}

void StudentPortal::display_menu()
{
    auto student = getCurrentStudent();
    double cgpa = semesterManager->calculate_cgpa(student);
    int creditsCompleted = semesterManager->get_total_credits_completed(student);

    OutputFormatter::print_header("STUDENT PORTAL - IIIT-Delhi ERP");

    std::cout << "  " << Colors::BOLD << Colors::CYAN << student.get_name() << Colors::RESET << "\n";
    std::cout << Colors::DIM << "  Roll: " << Colors::RESET << __roll_number << "\n";
    std::cout << Colors::DIM << "  CGPA: " << Colors::RESET << Colors::GREEN << Colors::BOLD
              << std::fixed << std::setprecision(2) << cgpa << Colors::RESET
              << Colors::DIM << "  Credits: " << Colors::RESET << Colors::CYAN << creditsCompleted << Colors::RESET << "\n\n";

    // profile & enrollment section
    std::cout << Colors::YELLOW << "  PROFILE & ENROLLMENT" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(1, "View Personal Information", Colors::CYAN);
    OutputFormatter::print_menu_item(2, "View My Enrollments", Colors::CYAN);
    OutputFormatter::print_menu_item(3, "View Pending Course Requests", Colors::CYAN);
    OutputFormatter::print_menu_item(4, "View My Grades", Colors::CYAN);
    OutputFormatter::print_menu_item(5, "View Transcript", Colors::GREEN);
    OutputFormatter::print_menu_item(6, "Download Transcript (Save to file)", Colors::GREEN);

    OutputFormatter::print_menu_separator();

    // course registration section
    std::cout << Colors::YELLOW << "  COURSE REGISTRATION" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(7, "Browse Course Catalog", Colors::CYAN);
    OutputFormatter::print_menu_item(8, "Search Courses", Colors::CYAN);
    OutputFormatter::print_menu_item(9, "View Course Details & Syllabus", Colors::CYAN);

    std::string regStatus = semesterManager->is_course_add_enabled() ? (Colors::GREEN + "OPEN" + Colors::RESET) : (Colors::RED + "CLOSED" + Colors::RESET);
    std::cout << "  " << Colors::GREEN << "[10]" << Colors::RESET
              << "  Register for Course (" << regStatus << ")\n";

    std::string dropStatus = semesterManager->is_course_drop_enabled() ? (Colors::GREEN + "ENABLED" + Colors::RESET) : (Colors::RED + "DISABLED" + Colors::RESET);
    std::cout << "  " << Colors::RED << "[11]" << Colors::RESET
              << "  Drop Course (" << dropStatus << ")\n";

    OutputFormatter::print_menu_separator();

    // account section
    std::cout << Colors::YELLOW << "  ACCOUNT" << Colors::RESET << "\n";
    OutputFormatter::print_menu_item(12, "Change Password", Colors::YELLOW);
    OutputFormatter::print_menu_item(13, "Logout", Colors::DIM);

    std::cout << "\n";
    OutputFormatter::print_double_line(80);
    std::cout << Colors::CYAN << "Enter your choice: " << Colors::RESET;
}

void StudentPortal::run()
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
            view_personal_info();
            break;
        case 2:
            view_enrollments();
            break;
        case 3:
            view_pending_requests();
            break;
        case 4:
            view_grades();
            break;
        case 5:
            view_transcript();
            break;
        case 6:
            download_transcript();
            break;
        case 7:
            view_courses_list();
            break;
        case 8:
            search_courses();
            break;
        case 9:
            view_course_details();
            break;
        case 10:
            register_for_course();
            break;
        case 11:
            drop_course();
            break;
        case 12:
            change_password();
            break;
        case 13:
            std::cout << "\nLogging out from student portal...\n";
            return;
        default:
            std::cout << Colors::RED << "\n[X] Invalid choice. Please try again." << Colors::RESET << "\n";
        }

        std::cout << "\n"
                  << Colors::DIM << "Press " << Colors::BOLD << "[Enter]" << Colors::RESET
                  << Colors::DIM << " to continue..." << Colors::RESET;
        std::cin.get();
    }
}

void StudentPortal::view_personal_info()
{
    auto student = getCurrentStudent();

    // Convert StudentLevel enum to string
    std::string levelStr;
    switch (student.get_level())
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

    OutputFormatter::print_header("PERSONAL INFORMATION");
    std::cout << "\n";
    std::cout << Colors::BOLD << "Roll Number: " << Colors::RESET << Colors::CYAN << student.get_roll_number() << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "Name: " << Colors::RESET << Colors::BRIGHT_CYAN << student.get_name() << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "Branch: " << Colors::RESET << student.get_branch().get_branch_name() << "\n";
    std::cout << Colors::BOLD << "Level: " << Colors::RESET << Colors::MAGENTA << levelStr << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "Start Year: " << Colors::RESET << student.get_starting_year() << "\n";

    int creditsCompleted = semesterManager->get_total_credits_completed(student);
    double cgpa = semesterManager->calculate_cgpa(student);

    std::cout << "\n"
              << Colors::CYAN << "> Academic Summary" << Colors::RESET << "\n";
    OutputFormatter::print_line();
    std::cout << Colors::BOLD << "Credits Completed: " << Colors::RESET << Colors::GREEN << creditsCompleted << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "CGPA: " << Colors::RESET << Colors::BRIGHT_GREEN << std::fixed << std::setprecision(2) << cgpa << Colors::RESET << "\n";
}

void StudentPortal::view_enrollments()
{
    auto student = getCurrentStudent();
    auto currentCourses = student.get_current_courses();
    auto previousCourses = student.get_previous_courses();

    OutputFormatter::print_header("MY ENROLLMENTS");

    std::cout << "\n"
              << Colors::GREEN << "> Current Courses: " << Colors::BOLD << currentCourses.size() << Colors::RESET << "\n";
    if (!currentCourses.empty())
    {
        OutputFormatter::print_line(90);
        std::cout << Colors::BOLD << Colors::CYAN << std::left << std::setw(15) << "Course Code"
                  << std::setw(45) << "Course Name"
                  << std::setw(10) << "Credits"
                  << std::setw(20) << "Instructor" << Colors::RESET << "\n";
        OutputFormatter::print_line(90);

        for (const auto &course : currentCourses)
        {
            std::cout << Colors::BOLD << Colors::CYAN << std::left << std::setw(15) << course.get_course_code() << Colors::RESET
                      << std::setw(45) << course.get_course_name()
                      << Colors::GREEN << std::setw(10) << course.get_credits() << Colors::RESET
                      << Colors::DIM << std::setw(20) << course.get_instructor() << Colors::RESET << "\n";
        }
        OutputFormatter::print_line(90);
    }

    std::cout << "\n"
              << Colors::CYAN << "> Completed Courses: " << Colors::BOLD << previousCourses.size() << Colors::RESET << "\n";
    if (!previousCourses.empty())
    {
        OutputFormatter::print_line(75);
        auto previousGrades = student.get_previous_grades();
        std::cout << Colors::BOLD << Colors::CYAN << std::left << std::setw(15) << "Course Code"
                  << std::setw(45) << "Course Name"
                  << std::setw(10) << "Credits"
                  << std::setw(10) << "Grade" << Colors::RESET << "\n";
        OutputFormatter::print_line(75);

        for (size_t i = 0; i < previousCourses.size(); ++i)
        {
            const auto &course = previousCourses[i];
            std::cout << Colors::BOLD << Colors::CYAN << std::left << std::setw(15) << course.get_course_code() << Colors::RESET
                      << std::setw(45) << course.get_course_name()
                      << Colors::GREEN << std::setw(10) << course.get_credits() << Colors::RESET;

            if (i < previousGrades.size())
            {
                std::string grade = previousGrades[i].get_letter_grade();
                std::string gradeColor = Colors::GREEN;
                if (grade[0] == 'A')
                    gradeColor = Colors::BRIGHT_GREEN;
                else if (grade[0] == 'B')
                    gradeColor = Colors::CYAN;
                else if (grade[0] == 'C')
                    gradeColor = Colors::YELLOW;
                else if (grade[0] == 'D' || grade[0] == 'F')
                    gradeColor = Colors::RED;

                std::cout << Colors::BOLD << gradeColor << std::setw(10) << grade << Colors::RESET;
            }
            std::cout << "\n";
        }
        OutputFormatter::print_line(75);
    }

    if (currentCourses.empty() && previousCourses.empty())
    {
        std::cout << Colors::YELLOW << "\n[!] No course enrollments found." << Colors::RESET << "\n";
        return;
    }
}

void StudentPortal::view_pending_requests()
{
    OutputFormatter::print_header("MY PENDING COURSE REQUESTS");

    try
    {
        auto pendingRequests = __erp_system->get_pending_requests_by_student(__roll_number);

        if (pendingRequests.empty())
        {
            std::cout << Colors::YELLOW << "\n[!] No pending course registration requests found." << Colors::RESET << "\n";
            return;
        }

        std::cout << "\n"
                  << Colors::CYAN << "> " << Colors::BOLD << "Total Requests: "
                  << Colors::GREEN << pendingRequests.size() << Colors::RESET << "\n\n";
        OutputFormatter::print_line(100);
        std::cout << Colors::BOLD << Colors::CYAN << std::left << std::setw(10) << "Req ID"
                  << std::setw(15) << "Course"
                  << std::setw(45) << "Course Name"
                  << std::setw(15) << "Status"
                  << std::setw(20) << "Request Date" << Colors::RESET << "\n";
        OutputFormatter::print_line(100);

        for (const auto &request : pendingRequests)
        {
            std::string statusColor = Colors::YELLOW;
            if (request.get_status() == RequestStatus::APPROVED)
                statusColor = Colors::GREEN;
            else if (request.get_status() == RequestStatus::REJECTED)
                statusColor = Colors::RED;

            std::cout << Colors::DIM << std::left << std::setw(10) << request.get_request_id() << Colors::RESET
                      << Colors::BOLD << Colors::CYAN << std::setw(15) << request.get_course_code() << Colors::RESET
                      << std::setw(45) << request.get_course_name()
                      << Colors::BOLD << statusColor << std::setw(15) << request_status_to_string(request.get_status()) << Colors::RESET
                      << Colors::DIM << std::setw(20) << request.get_request_time_string() << Colors::RESET << "\n";

            // Show remarks if available
            if (!request.get_remarks().empty())
            {
                std::cout << Colors::DIM << "  └─ Remarks: " << request.get_remarks() << Colors::RESET << "\n";
            }
        }
        OutputFormatter::print_line(100);

        // Count by status
        int pending = 0, approved = 0, rejected = 0;
        for (const auto &req : pendingRequests)
        {
            switch (req.get_status())
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

        std::cout << "\n"
                  << Colors::CYAN << "─ Summary " << Colors::RESET << OutputFormatter::get_status_indicator(false) << "\n";
        std::cout << Colors::BOLD << "Pending: " << Colors::YELLOW << pending << Colors::RESET
                  << " | " << Colors::BOLD << "Approved: " << Colors::GREEN << approved << Colors::RESET
                  << " | " << Colors::BOLD << "Rejected: " << Colors::RED << rejected << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << e.what() << "\n";
    }
}

void StudentPortal::register_for_course()
{
    if (!semesterManager->is_course_add_enabled())
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Course registration is currently closed.\n";
        std::cout << Colors::DIM << "   Please contact administration for more information." << Colors::RESET << "\n";
        return;
    }

    auto student = getCurrentStudent();
    auto currentCourses = student.get_current_courses();

    OutputFormatter::print_header("REGISTER FOR COURSE");
    std::cout << Colors::CYAN << "Enter Course Code: " << Colors::RESET;
    std::string courseCode;
    std::getline(std::cin, courseCode);
    courseCode = toUpperCase(courseCode); // Convert to uppercase

    // Check if course exists in catalog
    if (!courseCatalog->courseExists(courseCode))
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Course not found in catalog.\n";
        return;
    }

    // Check if already enrolled
    for (const auto &course : currentCourses)
    {
        if (course.get_course_code() == courseCode)
        {
            std::cout << Colors::YELLOW << "\n[!] ERROR: " << Colors::RESET << "You are already enrolled in this course.\n";
            return;
        }
    }

    // Check if already have a pending request for this course
    auto pendingRequests = __erp_system->get_pending_requests_by_student(__roll_number);
    for (const auto &req : pendingRequests)
    {
        if (req.get_course_code() == courseCode && req.get_status() == RequestStatus::PENDING)
        {
            std::cout << Colors::YELLOW << "\n[!] ERROR: " << Colors::RESET << "You already have a pending request for this course.\n";
            std::cout << Colors::DIM << "  Request ID: " << req.get_request_id() << "\n";
            std::cout << "  Status: " << request_status_to_string(req.get_status()) << Colors::RESET << "\n";
            return;
        }
    }

    // Display course info
    courseCatalog->displayCourse(courseCode);

    // Calculate current credits
    int currentCredits = 0;
    for (const auto &course : currentCourses)
    {
        currentCredits += course.get_credits();
    }

    int courseCredits = courseCatalog->getCourseCredits(courseCode);
    std::cout << "\n"
              << Colors::CYAN << "> Credit Summary" << Colors::RESET << "\n";
    OutputFormatter::print_line();
    std::cout << Colors::BOLD << "Your current credits: " << Colors::GREEN << currentCredits << Colors::DIM << "/16" << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "Course credits: " << Colors::CYAN << courseCredits << Colors::RESET << "\n";
    std::cout << Colors::BOLD << "Credits after registration: " << Colors::MAGENTA << (currentCredits + courseCredits) << Colors::DIM << "/16" << Colors::RESET << "\n";

    if (currentCredits + courseCredits > 16)
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Not enough credits available for this semester.\n";
        std::cout << Colors::DIM << "   Maximum 16 credits per semester." << Colors::RESET << "\n";
        return;
    }

    // Confirm registration
    std::cout << "\n"
              << Colors::YELLOW << "Do you want to submit the course registration request? (yes/no): " << Colors::RESET;
    std::string confirm;
    std::getline(std::cin, confirm);
    confirm = toUpperCase(confirm);

    if (confirm != "YES")
    {
        std::cout << Colors::DIM << "\nRegistration cancelled." << Colors::RESET << "\n";
        return;
    }

    // Submit pending request
    try
    {
        int requestId = __erp_system->addPendingRequest(__roll_number, courseCode);
        std::cout << Colors::GREEN << "\n[OK] Course registration request submitted successfully." << Colors::RESET << "\n";
        std::cout << Colors::DIM << "  Request ID: " << Colors::BOLD << requestId << Colors::RESET << "\n";
        std::cout << Colors::DIM << "  Course Code: " << Colors::BOLD << courseCode << Colors::RESET << "\n";
        std::cout << Colors::DIM << "  Status: " << Colors::YELLOW << "PENDING" << Colors::RESET << "\n";
        std::cout << Colors::CYAN << "\n  [i] Please wait for admin approval." << Colors::RESET << "\n";
        std::cout << Colors::DIM << "  You can check the status in 'View My Enrollments' menu." << Colors::RESET << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Error submitting request: " << e.what() << "\n";
    }
}

void StudentPortal::drop_course()
{
    if (!semesterManager->is_course_drop_enabled())
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Course drop is currently disabled.\n";
        std::cout << Colors::DIM << "   Please contact administration for more information." << Colors::RESET << "\n";
        return;
    }

    OutputFormatter::print_header("DROP COURSE");
    std::cout << Colors::YELLOW << "[!] WARNING: This action may affect your academic record!" << Colors::RESET << "\n\n";
    std::cout << Colors::CYAN << "Enter Course Code to drop: " << Colors::RESET;
    std::string courseCode;
    std::getline(std::cin, courseCode);
    courseCode = toUpperCase(courseCode); // Convert to uppercase

    std::cout << "\n"
              << Colors::YELLOW << "Are you sure you want to drop this course? (yes/no): " << Colors::RESET;
    std::string confirm;
    std::getline(std::cin, confirm);
    confirm = toUpperCase(confirm); // Convert to uppercase

    if (confirm == "YES")
    {
        // This is a placeholder - full drop implementation would need additional logic
        std::cout << Colors::YELLOW << "\n[!] WARNING: " << Colors::RESET << "Course drop functionality is under development.\n";
        std::cout << Colors::DIM << "   Please contact administration to drop courses." << Colors::RESET << "\n";
    }
    else
    {
        std::cout << Colors::DIM << "\nDrop cancelled." << Colors::RESET << "\n";
    }
}

void StudentPortal::view_courses_list()
{
    auto student = getCurrentStudent();

    // Convert StudentLevel enum to string
    std::string levelStr;
    switch (student.get_level())
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

    std::string header = "AVAILABLE COURSES FOR " + levelStr;
    OutputFormatter::print_header(header);

    auto courses = courseCatalog->get_courses_by_level(levelStr);

    if (courses.empty())
    {
        std::cout << Colors::YELLOW << "\n[!] No courses available for your level." << Colors::RESET << "\n";
        return;
    }

    std::cout << "\n"
              << Colors::CYAN << "[i] Total Courses: " << Colors::BOLD << courses.size() << Colors::RESET << "\n\n";
    std::cout << Colors::BOLD << std::left << std::setw(12) << "Code"
              << std::setw(40) << "Name"
              << std::setw(10) << "Credits"
              << std::setw(12) << "Type" << Colors::RESET << "\n";
    OutputFormatter::print_line();

    for (const auto &course : courses)
    {
        std::cout << Colors::CYAN << std::left << std::setw(12) << course.courseCode << Colors::RESET
                  << std::setw(40) << course.courseName
                  << Colors::GREEN << std::setw(10) << course.credits << Colors::RESET
                  << Colors::DIM << std::setw(12) << course.courseType << Colors::RESET << "\n";
    }
}

void StudentPortal::view_course_details()
{
    OutputFormatter::print_header("COURSE DETAILS");
    std::cout << Colors::CYAN << "Enter Course Code: " << Colors::RESET;
    std::string courseCode;
    std::getline(std::cin, courseCode);
    courseCode = toUpperCase(courseCode); // Convert to uppercase

    if (!courseCatalog->courseExists(courseCode))
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Course not found.\n";
        return;
    }

    courseCatalog->displayCourse(courseCode);
}

void StudentPortal::search_courses()
{
    OutputFormatter::print_header("SEARCH COURSES");
    std::cout << Colors::CYAN << "Enter search query: " << Colors::RESET;
    std::string query;
    std::getline(std::cin, query);

    auto results = courseCatalog->search_courses(query);

    if (results.empty())
    {
        std::cout << Colors::YELLOW << "\n[!] No courses found matching your query." << Colors::RESET << "\n";
        return;
    }

    std::cout << Colors::GREEN << "\n[OK] Found " << Colors::BOLD << results.size() << Colors::RESET << Colors::GREEN << " course(s)" << Colors::RESET << "\n\n";
    std::cout << Colors::BOLD << std::left << std::setw(12) << "Code"
              << std::setw(40) << "Name"
              << std::setw(10) << "Credits" << Colors::RESET << "\n";
    OutputFormatter::print_line();

    for (const auto &course : results)
    {
        std::cout << Colors::CYAN << std::left << std::setw(12) << course.courseCode << Colors::RESET
                  << std::setw(40) << course.courseName
                  << Colors::GREEN << std::setw(10) << course.credits << Colors::RESET << "\n";
    }
}

void StudentPortal::view_grades()
{
    auto student = getCurrentStudent();
    auto currentCourses = student.get_current_courses();
    auto currentGrades = student.get_current_grades();
    auto previousCourses = student.get_previous_courses();
    auto previousGrades = student.get_previous_grades();

    OutputFormatter::print_header("MY GRADES");

    if (previousCourses.empty() && currentCourses.empty())
    {
        std::cout << Colors::YELLOW << "\n[!] No enrollments found." << Colors::RESET << "\n";
        return;
    }

    // Show completed courses with grades
    if (!previousCourses.empty())
    {
        std::cout << "\n"
                  << Colors::GREEN << "> Completed Courses" << Colors::RESET << "\n";
        OutputFormatter::print_line();
        std::cout << Colors::BOLD << std::left << std::setw(12) << "Course"
                  << std::setw(35) << "Name"
                  << std::setw(10) << "Credits"
                  << std::setw(8) << "Grade"
                  << std::setw(8) << "Points" << Colors::RESET << "\n";
        OutputFormatter::print_line();

        for (size_t i = 0; i < previousCourses.size(); ++i)
        {
            const auto &course = previousCourses[i];
            std::cout << Colors::CYAN << std::left << std::setw(12) << course.get_course_code() << Colors::RESET
                      << std::setw(35) << course.get_course_name()
                      << Colors::GREEN << std::setw(10) << course.get_credits() << Colors::RESET;

            if (i < previousGrades.size())
            {
                const auto &grade = previousGrades[i];
                std::string gradeStr = grade.get_letter_grade();
                std::string gradeColor = Colors::GREEN;
                if (gradeStr[0] == 'A')
                    gradeColor = Colors::BRIGHT_GREEN;
                else if (gradeStr[0] == 'B')
                    gradeColor = Colors::CYAN;
                else if (gradeStr[0] == 'C')
                    gradeColor = Colors::YELLOW;
                else if (gradeStr[0] == 'D' || gradeStr[0] == 'F')
                    gradeColor = Colors::RED;

                std::cout << Colors::BOLD << gradeColor << std::setw(8) << gradeStr << Colors::RESET
                          << Colors::DIM << std::setw(8) << std::fixed << std::setprecision(2)
                          << grade.get_grade_point() << Colors::RESET;
            }
            else
            {
                std::cout << Colors::DIM << std::setw(8) << "N/A"
                          << std::setw(8) << "N/A" << Colors::RESET;
            }
            std::cout << "\n";
        }
    }

    // Show current courses (usually without grades yet)
    if (!currentCourses.empty())
    {
        std::cout << "\n"
                  << Colors::YELLOW << "> Current Courses" << Colors::RESET << "\n";
        OutputFormatter::print_line();
        std::cout << Colors::BOLD << std::left << std::setw(12) << "Course"
                  << std::setw(35) << "Name"
                  << std::setw(10) << "Credits"
                  << std::setw(8) << "Grade"
                  << std::setw(8) << "Points" << Colors::RESET << "\n";
        OutputFormatter::print_line();

        for (size_t i = 0; i < currentCourses.size(); ++i)
        {
            const auto &course = currentCourses[i];
            std::cout << Colors::CYAN << std::left << std::setw(12) << course.get_course_code() << Colors::RESET
                      << std::setw(35) << course.get_course_name()
                      << Colors::GREEN << std::setw(10) << course.get_credits() << Colors::RESET;

            if (i < currentGrades.size())
            {
                const auto &grade = currentGrades[i];
                std::string gradeStr = grade.get_letter_grade();
                std::string gradeColor = Colors::GREEN;
                if (gradeStr[0] == 'A')
                    gradeColor = Colors::BRIGHT_GREEN;
                else if (gradeStr[0] == 'B')
                    gradeColor = Colors::CYAN;
                else if (gradeStr[0] == 'C')
                    gradeColor = Colors::YELLOW;
                else if (gradeStr[0] == 'D' || gradeStr[0] == 'F')
                    gradeColor = Colors::RED;

                std::cout << Colors::BOLD << gradeColor << std::setw(8) << gradeStr << Colors::RESET
                          << Colors::DIM << std::setw(8) << std::fixed << std::setprecision(2)
                          << grade.get_grade_point() << Colors::RESET;
            }
            else
            {
                std::cout << Colors::DIM << std::setw(8) << "Pending"
                          << std::setw(8) << "-" << Colors::RESET;
            }
            std::cout << "\n";
        }
    }

    // Calculate and show CGPA
    double cgpa = student.calculate_cgpa();
    int creditsCompleted = semesterManager->get_total_credits_completed(student);

    std::cout << "\n";
    OutputFormatter::print_double_line();
    std::cout << Colors::BOLD << "CGPA: " << Colors::BRIGHT_GREEN << std::fixed << std::setprecision(2) << cgpa << Colors::RESET
              << Colors::BOLD << " | Credits Completed: " << Colors::GREEN << creditsCompleted << Colors::RESET << "\n";
    OutputFormatter::print_double_line();
}

void StudentPortal::view_transcript()
{
    auto student = getCurrentStudent();
    std::string transcript = semesterManager->generate_transcript(student);
    std::cout << transcript;
}

void StudentPortal::download_transcript()
{
    auto student = getCurrentStudent();
    std::string transcript = semesterManager->generate_transcript(student);

    std::string filename = "transcript_" + __roll_number + ".txt";
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cout << "\nERROR: Could not create transcript file.\n";
        return;
    }

    file << transcript;
    file.close();

    std::cout << "\nTranscript saved successfully.\n";
    std::cout << "   File: " << filename << "\n";
}

void StudentPortal::change_password()
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
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Passwords do not match.\n";
        return;
    }

    if (newPassword.length() < 6)
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Password must be at least 6 characters long.\n";
        return;
    }

    if (Auth::change_password(__roll_number, oldPassword, newPassword, UserType::STUDENT))
    {
        std::cout << Colors::GREEN << "\n[OK] Password changed successfully." << Colors::RESET << "\n";
    }
    else
    {
        std::cout << Colors::RED << "\n[X] ERROR: " << Colors::RESET << "Failed to change password. Check your current password.\n";
    }
}
