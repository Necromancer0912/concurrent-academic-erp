#include "SemesterManager.h"
#include <iostream>

SemesterManager::SemesterManager(CourseCatalog *cat)
    : courseAddEnabled(false), courseDropEnabled(false)
{
    // Catalog parameter kept for future use
    (void)cat; // Suppress unused parameter warning
}

void SemesterManager::setCourseAddEnabled(bool enabled)
{
    courseAddEnabled = enabled;
    if (enabled)
    {
        std::cout << "Course registration is now OPEN.\n";
    }
    else
    {
        std::cout << "Course registration is now CLOSED.\n";
    }
}

void SemesterManager::setCourseDropEnabled(bool enabled)
{
    courseDropEnabled = enabled;
    if (enabled)
    {
        std::cout << "Course drop is now ENABLED.\n";
    }
    else
    {
        std::cout << "Course drop is now DISABLED.\n";
    }
}

bool SemesterManager::isCourseAddEnabled() const
{
    return courseAddEnabled;
}

bool SemesterManager::isCourseDropEnabled() const
{
    return courseDropEnabled;
}

std::string SemesterManager::getStatus() const
{
    std::ostringstream oss;
    oss << "\n┌─ Semester Manager Status ────────────────┐\n";
    oss << "│ Course Registration: "
        << (courseAddEnabled ? "OPEN   =>" : "CLOSED X") << std::string(17, ' ') << "│\n";
    oss << "│ Course Drop:         "
        << (courseDropEnabled ? "ENABLED =>" : "DISABLED X") << std::string(15, ' ') << "│\n";
    oss << "│ Max Credits/Sem:     16" << std::string(19, ' ') << "│\n";
    oss << "│ Capstone Credits:    8" << std::string(20, ' ') << "│\n";
    oss << "│ Research Credits:    16" << std::string(19, ' ') << "│\n";
    oss << "└───────────────────────────────────────────┘\n";
    return oss.str();
}
