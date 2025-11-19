#include "SemesterManager.h"
#include "Colors.h"
#include <iostream>

SemesterManager::SemesterManager(CourseCatalog *_cat)
    : __course_add_enabled(false), __course_drop_enabled(false)
{
    // Catalog parameter kept for future use
    (void)_cat; // Suppress unused parameter warning
}

void SemesterManager::set_course_add_enabled(bool _enabled)
{
    __course_add_enabled = _enabled;
    if (_enabled)
    {
        std::cout << "Course registration is now OPEN.\n";
    }
    else
    {
        std::cout << "Course registration is now CLOSED.\n";
    }
}

void SemesterManager::set_course_drop_enabled(bool _enabled)
{
    __course_drop_enabled = _enabled;
    if (_enabled)
    {
        std::cout << "Course drop is now ENABLED.\n";
    }
    else
    {
        std::cout << "Course drop is now DISABLED.\n";
    }
}

bool SemesterManager::is_course_add_enabled() const
{
    return __course_add_enabled;
}

bool SemesterManager::is_course_drop_enabled() const
{
    return __course_drop_enabled;
}

std::string SemesterManager::get_status() const
{
    std::ostringstream _oss;
    _oss << "\n"
         << Colors::BOLD << Colors::YELLOW << " SEMESTER SETTINGS" << Colors::RESET << "\n";
    _oss << std::string(80, '-') << "\n";
    _oss << Colors::CYAN << "  Course Registration: " << Colors::RESET
         << (__course_add_enabled ? Colors::GREEN + "[OK] OPEN" : Colors::RED + "[X] CLOSED") << Colors::RESET << "\n";
    _oss << Colors::CYAN << "  Course Drop:         " << Colors::RESET
         << (__course_drop_enabled ? Colors::GREEN + "[OK] ENABLED" : Colors::RED + "[X] DISABLED") << Colors::RESET << "\n";
    _oss << Colors::CYAN << "  Max Credits/Sem:     " << Colors::RESET << Colors::BOLD << "16" << Colors::RESET << " credits\n";
    _oss << Colors::CYAN << "  Capstone Credits:    " << Colors::RESET << Colors::BOLD << "8" << Colors::RESET << " credits\n";
    _oss << Colors::CYAN << "  Research Credits:    " << Colors::RESET << Colors::BOLD << "16" << Colors::RESET << " credits\n";
    return _oss.str();
}
