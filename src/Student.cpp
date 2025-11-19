#include "Student.h"
#include <iostream>
#include <iomanip>
#include <regex>

// Validate __name format - must be "firstName secondName"
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::__validate_name(const std::string &n)
{
    std::regex namePattern("^[A-Za-z]+\\s[A-Za-z]+(\\s[A-Za-z]+)*$");
    if (!std::regex_match(n, namePattern))
    {
        throw InvalidNameException("Invalid __name format. Must be in 'FirstName SecondName' format with only alphabetic characters.");
    }
}

// Validate roll number format - no spaces, max 15 characters
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::__validate_roll_number(const RollType &roll)
{
    std::string rollStr;
    if constexpr (std::is_same<RollType, std::string>::value)
    {
        rollStr = roll;
    }
    else
    {
        rollStr = std::to_string(roll);
    }

    if (rollStr.length() > 15)
    {
        throw InvalidRollNumberException("Roll number must not exceed 15 characters.");
    }

    if (rollStr.find(' ') != std::string::npos)
    {
        throw InvalidRollNumberException("Roll number must not contain spaces.");
    }

    if (rollStr.empty())
    {
        throw InvalidRollNumberException("Roll number cannot be empty.");
    }
}

// Constructor
template <typename RollType, typename CourseCodeType>
Student<RollType, CourseCodeType>::Student(const std::string &n, const RollType &roll,
                                           const Branch &br, StudentLevel lvl, int year)
    : __branch(br), __level(lvl), __starting_year(year)
{
    __validate_name(n);
    __validate_roll_number(roll);
    __name = n;
    __roll_number = roll;
    __loaded_cgpa = 0.0;
}

// Loaded CGPA accessors
template <typename RollType, typename CourseCodeType>
double Student<RollType, CourseCodeType>::get_loaded_cgpa() const
{
    return __loaded_cgpa;
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::set_loaded_cgpa(double cgpa)
{
    __loaded_cgpa = cgpa;
}

// Copy constructor
template <typename RollType, typename CourseCodeType>
Student<RollType, CourseCodeType>::Student(const Student &other)
    : __name(other.__name), __roll_number(other.__roll_number), __branch(other.__branch),
      __level(other.__level), __starting_year(other.__starting_year),
      __current_courses(other.__current_courses), __current_grades(other.__current_grades),
      __previous_courses(other.__previous_courses), __previous_grades(other.__previous_grades),
      __loaded_cgpa(other.__loaded_cgpa) {}

// Assignment operator
template <typename RollType, typename CourseCodeType>
Student<RollType, CourseCodeType> &Student<RollType, CourseCodeType>::operator=(const Student &other)
{
    if (this != &other)
    {
        __name = other.__name;
        __roll_number = other.__roll_number;
        __branch = other.__branch;
        __level = other.__level;
        __starting_year = other.__starting_year;
        __current_courses = other.__current_courses;
        __current_grades = other.__current_grades;
        __previous_courses = other.__previous_courses;
        __previous_grades = other.__previous_grades;
        __loaded_cgpa = other.__loaded_cgpa;
    }
    return *this;
}

// Getters
template <typename RollType, typename CourseCodeType>
std::string Student<RollType, CourseCodeType>::get_name() const { return __name; }

template <typename RollType, typename CourseCodeType>
RollType Student<RollType, CourseCodeType>::get_roll_number() const { return __roll_number; }

template <typename RollType, typename CourseCodeType>
Branch Student<RollType, CourseCodeType>::get_branch() const { return __branch; }

template <typename RollType, typename CourseCodeType>
StudentLevel Student<RollType, CourseCodeType>::get_level() const { return __level; }

template <typename RollType, typename CourseCodeType>
int Student<RollType, CourseCodeType>::get_starting_year() const { return __starting_year; }

template <typename RollType, typename CourseCodeType>
std::vector<Course<CourseCodeType>> Student<RollType, CourseCodeType>::get_current_courses() const
{
    return __current_courses;
}

template <typename RollType, typename CourseCodeType>
std::vector<Grade> Student<RollType, CourseCodeType>::get_current_grades() const
{
    return __current_grades;
}

template <typename RollType, typename CourseCodeType>
std::vector<Course<CourseCodeType>> Student<RollType, CourseCodeType>::get_previous_courses() const
{
    return __previous_courses;
}

template <typename RollType, typename CourseCodeType>
std::vector<Grade> Student<RollType, CourseCodeType>::get_previous_grades() const
{
    return __previous_grades;
}

// Setters
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::set_name(const std::string &n)
{
    __validate_name(n);
    __name = n;
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::set_branch(const Branch &br)
{
    __branch = br;
}

// Course management
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::add_current_course(const Course<CourseCodeType> &course)
{
    __current_courses.push_back(course);
    __current_grades.push_back(Grade()); // Add empty grade
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::add_current_course(const Course<CourseCodeType> &course, const Grade &grade)
{
    __current_courses.push_back(course);
    __current_grades.push_back(grade);
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::remove_current_course(const CourseCodeType &courseCode)
{
    for (size_t i = 0; i < __current_courses.size(); ++i)
    {
        if (__current_courses[i].get_course_code() == courseCode)
        {
            __current_courses.erase(__current_courses.begin() + i);
            __current_grades.erase(__current_grades.begin() + i);
            break;
        }
    }
}

// Grade management
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::update_grade(const CourseCodeType &courseCode, const Grade &grade)
{
    for (size_t i = 0; i < __current_courses.size(); ++i)
    {
        if (__current_courses[i].get_course_code() == courseCode)
        {
            __current_grades[i] = grade;
            return;
        }
    }

    for (size_t i = 0; i < __previous_courses.size(); ++i)
    {
        if (__previous_courses[i].get_course_code() == courseCode)
        {
            __previous_grades[i] = grade;
            return;
        }
    }
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::update_grade_from_marks(const CourseCodeType &course_code,
                                                                double mid_sem, double end_sem, double assignment)
{
    Grade newGrade(mid_sem, end_sem, assignment);
    update_grade(course_code, newGrade);
}

// Move course from current to previous
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::move_to_previous_courses(const CourseCodeType &courseCode)
{
    for (size_t i = 0; i < __current_courses.size(); ++i)
    {
        if (__current_courses[i].get_course_code() == courseCode)
        {
            __previous_courses.push_back(__current_courses[i]);
            __previous_grades.push_back(__current_grades[i]);
            __current_courses.erase(__current_courses.begin() + i);
            __current_grades.erase(__current_grades.begin() + i);
            break;
        }
    }
}

// Get specific grade
template <typename RollType, typename CourseCodeType>
Grade Student<RollType, CourseCodeType>::get_grade_for_course(const CourseCodeType &courseCode) const
{
    for (size_t i = 0; i < __current_courses.size(); ++i)
    {
        if (__current_courses[i].get_course_code() == courseCode)
        {
            return __current_grades[i];
        }
    }

    for (size_t i = 0; i < __previous_courses.size(); ++i)
    {
        if (__previous_courses[i].get_course_code() == courseCode)
        {
            return __previous_grades[i];
        }
    }

    return Grade(); // Return empty grade if not found
}

// CGPA calculation
template <typename RollType, typename CourseCodeType>
double Student<RollType, CourseCodeType>::calculate_cgpa() const
{
    if (__previous_courses.empty())
        return 0.0;

    double totalGradePoints = 0.0;
    int totalCredits = 0;

    for (size_t i = 0; i < __previous_courses.size(); ++i)
    {
        totalGradePoints += __previous_grades[i].get_grade_point() * __previous_courses[i].get_credits();
        totalCredits += __previous_courses[i].get_credits();
    }

    return totalCredits > 0 ? totalGradePoints / totalCredits : 0.0;
}

template <typename RollType, typename CourseCodeType>
double Student<RollType, CourseCodeType>::calculate_sgpa() const
{
    if (__current_courses.empty())
        return 0.0;

    double totalGradePoints = 0.0;
    int totalCredits = 0;

    for (size_t i = 0; i < __current_courses.size(); ++i)
    {
        totalGradePoints += __current_grades[i].get_grade_point() * __current_courses[i].get_credits();
        totalCredits += __current_courses[i].get_credits();
    }

    return totalCredits > 0 ? totalGradePoints / totalCredits : 0.0;
}

// Search functionality
template <typename RollType, typename CourseCodeType>
bool Student<RollType, CourseCodeType>::has_grade_in_course(const CourseCodeType &courseCode, double minGrade) const
{
    for (size_t i = 0; i < __previous_courses.size(); ++i)
    {
        if (__previous_courses[i].get_course_code() == courseCode)
        {
            return __previous_grades[i].get_grade_point() >= minGrade;
        }
    }
    return false;
}

// Display
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::display_info() const
{
    std::cout << "Name: " << __name << " | Roll: ";
    if constexpr (std::is_same<RollType, std::string>::value)
    {
        std::cout << __roll_number;
    }
    else
    {
        std::cout << __roll_number;
    }
    double cgpaToShow = __loaded_cgpa > 0.0 ? __loaded_cgpa : calculate_cgpa();
    std::cout << " | Branch: " << __branch.get_branch_code()
              << " | Level: " << level_to_string(__level)
              << " | Year: " << __starting_year
              << " | CGPA: " << std::fixed << std::setprecision(2) << cgpaToShow << std::endl;
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::display_detailed() const
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "Student Details" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Name: " << __name << std::endl;
    std::cout << "Roll Number: ";
    if constexpr (std::is_same<RollType, std::string>::value)
    {
        std::cout << __roll_number;
    }
    else
    {
        std::cout << __roll_number;
    }
    std::cout << std::endl;
    std::cout << "Branch: " << __branch.get_branch_name() << " (" << __branch.get_branch_code() << ")" << std::endl;
    std::cout << "Level: " << level_to_string(__level) << std::endl;
    std::cout << "Starting Year: " << __starting_year << std::endl;
    double cgpaToShow = __loaded_cgpa > 0.0 ? __loaded_cgpa : calculate_cgpa();
    std::cout << "CGPA: " << std::fixed << std::setprecision(2) << cgpaToShow << std::endl;
    std::cout << "SGPA: " << std::fixed << std::setprecision(2) << calculate_sgpa() << std::endl;

    if (!__current_courses.empty())
    {
        std::cout << "\nCurrent Courses:" << std::endl;
        for (size_t i = 0; i < __current_courses.size(); ++i)
        {
            std::cout << "  - " << __current_courses[i].course_code_to_string()
                      << ": " << __current_courses[i].get_course_name()
                      << " (Grade: " << __current_grades[i].get_letter_grade() << ")" << std::endl;
        }
    }

    if (!__previous_courses.empty())
    {
        std::cout << "\nPrevious Courses:" << std::endl;
        for (size_t i = 0; i < __previous_courses.size(); ++i)
        {
            std::cout << "  - " << __previous_courses[i].course_code_to_string()
                      << ": " << __previous_courses[i].get_course_name()
                      << " (Grade: " << __previous_grades[i].get_letter_grade() << ")" << std::endl;
        }
    }
    std::cout << "========================================\n"
              << std::endl;
}

// Comparison operators
template <typename RollType, typename CourseCodeType>
bool Student<RollType, CourseCodeType>::operator<(const Student &other) const
{
    return __name < other.__name;
}

template <typename RollType, typename CourseCodeType>
bool Student<RollType, CourseCodeType>::operator>(const Student &other) const
{
    return __name > other.__name;
}

template <typename RollType, typename CourseCodeType>
bool Student<RollType, CourseCodeType>::operator==(const Student &other) const
{
    return __roll_number == other.__roll_number;
}

// Static helper
template <typename RollType, typename CourseCodeType>
std::string Student<RollType, CourseCodeType>::level_to_string(StudentLevel __level)
{
    switch (__level)
    {
    case StudentLevel::BTECH:
        return "B.Tech";
    case StudentLevel::MTECH:
        return "M.Tech";
    case StudentLevel::PHD:
        return "Ph.D.";
    case StudentLevel::DUAL_DEGREE:
        return "Dual Degree";
    default:
        return "Unknown";
    }
}

// Explicit template instantiations
template class Student<std::string, std::string>;
template class Student<std::string, int>;
template class Student<unsigned int, std::string>;
template class Student<unsigned int, int>;
