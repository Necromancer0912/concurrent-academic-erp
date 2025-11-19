#include "Student.h"
#include <iostream>
#include <iomanip>
#include <regex>

// Validate name format - must be "firstName secondName"
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::validateName(const std::string &n)
{
    std::regex namePattern("^[A-Za-z]+\\s[A-Za-z]+(\\s[A-Za-z]+)*$");
    if (!std::regex_match(n, namePattern))
    {
        throw InvalidNameException("Invalid name format. Must be in 'FirstName SecondName' format with only alphabetic characters.");
    }
}

// Validate roll number format - no spaces, max 15 characters
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::validateRollNumber(const RollType &roll)
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
    : branch(br), level(lvl), startingYear(year)
{
    validateName(n);
    validateRollNumber(roll);
    name = n;
    rollNumber = roll;
    loadedCGPA = 0.0;
}

// Loaded CGPA accessors
template <typename RollType, typename CourseCodeType>
double Student<RollType, CourseCodeType>::getLoadedCGPA() const
{
    return loadedCGPA;
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::setLoadedCGPA(double cgpa)
{
    loadedCGPA = cgpa;
}

// Copy constructor
template <typename RollType, typename CourseCodeType>
Student<RollType, CourseCodeType>::Student(const Student &other)
    : name(other.name), rollNumber(other.rollNumber), branch(other.branch),
      level(other.level), startingYear(other.startingYear),
      currentCourses(other.currentCourses), currentGrades(other.currentGrades),
      previousCourses(other.previousCourses), previousGrades(other.previousGrades),
      loadedCGPA(other.loadedCGPA) {}

// Assignment operator
template <typename RollType, typename CourseCodeType>
Student<RollType, CourseCodeType> &Student<RollType, CourseCodeType>::operator=(const Student &other)
{
    if (this != &other)
    {
        name = other.name;
        rollNumber = other.rollNumber;
        branch = other.branch;
        level = other.level;
        startingYear = other.startingYear;
        currentCourses = other.currentCourses;
        currentGrades = other.currentGrades;
        previousCourses = other.previousCourses;
        previousGrades = other.previousGrades;
        loadedCGPA = other.loadedCGPA;
    }
    return *this;
}

// Getters
template <typename RollType, typename CourseCodeType>
std::string Student<RollType, CourseCodeType>::getName() const { return name; }

template <typename RollType, typename CourseCodeType>
RollType Student<RollType, CourseCodeType>::getRollNumber() const { return rollNumber; }

template <typename RollType, typename CourseCodeType>
Branch Student<RollType, CourseCodeType>::getBranch() const { return branch; }

template <typename RollType, typename CourseCodeType>
StudentLevel Student<RollType, CourseCodeType>::getLevel() const { return level; }

template <typename RollType, typename CourseCodeType>
int Student<RollType, CourseCodeType>::getStartingYear() const { return startingYear; }

template <typename RollType, typename CourseCodeType>
std::vector<Course<CourseCodeType>> Student<RollType, CourseCodeType>::getCurrentCourses() const
{
    return currentCourses;
}

template <typename RollType, typename CourseCodeType>
std::vector<Grade> Student<RollType, CourseCodeType>::getCurrentGrades() const
{
    return currentGrades;
}

template <typename RollType, typename CourseCodeType>
std::vector<Course<CourseCodeType>> Student<RollType, CourseCodeType>::getPreviousCourses() const
{
    return previousCourses;
}

template <typename RollType, typename CourseCodeType>
std::vector<Grade> Student<RollType, CourseCodeType>::getPreviousGrades() const
{
    return previousGrades;
}

// Setters
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::setName(const std::string &n)
{
    validateName(n);
    name = n;
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::setBranch(const Branch &br)
{
    branch = br;
}

// Course management
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::addCurrentCourse(const Course<CourseCodeType> &course)
{
    currentCourses.push_back(course);
    currentGrades.push_back(Grade()); // Add empty grade
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::addCurrentCourse(const Course<CourseCodeType> &course, const Grade &grade)
{
    currentCourses.push_back(course);
    currentGrades.push_back(grade);
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::removeCurrentCourse(const CourseCodeType &courseCode)
{
    for (size_t i = 0; i < currentCourses.size(); ++i)
    {
        if (currentCourses[i].getCourseCode() == courseCode)
        {
            currentCourses.erase(currentCourses.begin() + i);
            currentGrades.erase(currentGrades.begin() + i);
            break;
        }
    }
}

// Grade management
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::updateGrade(const CourseCodeType &courseCode, const Grade &grade)
{
    for (size_t i = 0; i < currentCourses.size(); ++i)
    {
        if (currentCourses[i].getCourseCode() == courseCode)
        {
            currentGrades[i] = grade;
            return;
        }
    }

    for (size_t i = 0; i < previousCourses.size(); ++i)
    {
        if (previousCourses[i].getCourseCode() == courseCode)
        {
            previousGrades[i] = grade;
            return;
        }
    }
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::updateGradeFromMarks(const CourseCodeType &courseCode,
                                                             double midSem, double endSem, double assignment)
{
    Grade newGrade(midSem, endSem, assignment);
    updateGrade(courseCode, newGrade);
}

// Move course from current to previous
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::moveToPreviousCourses(const CourseCodeType &courseCode)
{
    for (size_t i = 0; i < currentCourses.size(); ++i)
    {
        if (currentCourses[i].getCourseCode() == courseCode)
        {
            previousCourses.push_back(currentCourses[i]);
            previousGrades.push_back(currentGrades[i]);
            currentCourses.erase(currentCourses.begin() + i);
            currentGrades.erase(currentGrades.begin() + i);
            break;
        }
    }
}

// Get specific grade
template <typename RollType, typename CourseCodeType>
Grade Student<RollType, CourseCodeType>::getGradeForCourse(const CourseCodeType &courseCode) const
{
    for (size_t i = 0; i < currentCourses.size(); ++i)
    {
        if (currentCourses[i].getCourseCode() == courseCode)
        {
            return currentGrades[i];
        }
    }

    for (size_t i = 0; i < previousCourses.size(); ++i)
    {
        if (previousCourses[i].getCourseCode() == courseCode)
        {
            return previousGrades[i];
        }
    }

    return Grade(); // Return empty grade if not found
}

// CGPA calculation
template <typename RollType, typename CourseCodeType>
double Student<RollType, CourseCodeType>::calculateCGPA() const
{
    if (previousCourses.empty())
        return 0.0;

    double totalGradePoints = 0.0;
    int totalCredits = 0;

    for (size_t i = 0; i < previousCourses.size(); ++i)
    {
        totalGradePoints += previousGrades[i].getGradePoint() * previousCourses[i].getCredits();
        totalCredits += previousCourses[i].getCredits();
    }

    return totalCredits > 0 ? totalGradePoints / totalCredits : 0.0;
}

template <typename RollType, typename CourseCodeType>
double Student<RollType, CourseCodeType>::calculateSGPA() const
{
    if (currentCourses.empty())
        return 0.0;

    double totalGradePoints = 0.0;
    int totalCredits = 0;

    for (size_t i = 0; i < currentCourses.size(); ++i)
    {
        totalGradePoints += currentGrades[i].getGradePoint() * currentCourses[i].getCredits();
        totalCredits += currentCourses[i].getCredits();
    }

    return totalCredits > 0 ? totalGradePoints / totalCredits : 0.0;
}

// Search functionality
template <typename RollType, typename CourseCodeType>
bool Student<RollType, CourseCodeType>::hasGradeInCourse(const CourseCodeType &courseCode, double minGrade) const
{
    for (size_t i = 0; i < previousCourses.size(); ++i)
    {
        if (previousCourses[i].getCourseCode() == courseCode)
        {
            return previousGrades[i].getGradePoint() >= minGrade;
        }
    }
    return false;
}

// Display
template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::displayInfo() const
{
    std::cout << "Name: " << name << " | Roll: ";
    if constexpr (std::is_same<RollType, std::string>::value)
    {
        std::cout << rollNumber;
    }
    else
    {
        std::cout << rollNumber;
    }
    double cgpaToShow = loadedCGPA > 0.0 ? loadedCGPA : calculateCGPA();
    std::cout << " | Branch: " << branch.getBranchCode()
              << " | Level: " << levelToString(level)
              << " | Year: " << startingYear
              << " | CGPA: " << std::fixed << std::setprecision(2) << cgpaToShow << std::endl;
}

template <typename RollType, typename CourseCodeType>
void Student<RollType, CourseCodeType>::displayDetailed() const
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "Student Details" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Roll Number: ";
    if constexpr (std::is_same<RollType, std::string>::value)
    {
        std::cout << rollNumber;
    }
    else
    {
        std::cout << rollNumber;
    }
    std::cout << std::endl;
    std::cout << "Branch: " << branch.getBranchName() << " (" << branch.getBranchCode() << ")" << std::endl;
    std::cout << "Level: " << levelToString(level) << std::endl;
    std::cout << "Starting Year: " << startingYear << std::endl;
    double cgpaToShow = loadedCGPA > 0.0 ? loadedCGPA : calculateCGPA();
    std::cout << "CGPA: " << std::fixed << std::setprecision(2) << cgpaToShow << std::endl;
    std::cout << "SGPA: " << std::fixed << std::setprecision(2) << calculateSGPA() << std::endl;

    if (!currentCourses.empty())
    {
        std::cout << "\nCurrent Courses:" << std::endl;
        for (size_t i = 0; i < currentCourses.size(); ++i)
        {
            std::cout << "  - " << currentCourses[i].courseCodeToString()
                      << ": " << currentCourses[i].getCourseName()
                      << " (Grade: " << currentGrades[i].getLetterGrade() << ")" << std::endl;
        }
    }

    if (!previousCourses.empty())
    {
        std::cout << "\nPrevious Courses:" << std::endl;
        for (size_t i = 0; i < previousCourses.size(); ++i)
        {
            std::cout << "  - " << previousCourses[i].courseCodeToString()
                      << ": " << previousCourses[i].getCourseName()
                      << " (Grade: " << previousGrades[i].getLetterGrade() << ")" << std::endl;
        }
    }
    std::cout << "========================================\n"
              << std::endl;
}

// Comparison operators
template <typename RollType, typename CourseCodeType>
bool Student<RollType, CourseCodeType>::operator<(const Student &other) const
{
    return name < other.name;
}

template <typename RollType, typename CourseCodeType>
bool Student<RollType, CourseCodeType>::operator>(const Student &other) const
{
    return name > other.name;
}

template <typename RollType, typename CourseCodeType>
bool Student<RollType, CourseCodeType>::operator==(const Student &other) const
{
    return rollNumber == other.rollNumber;
}

// Static helper
template <typename RollType, typename CourseCodeType>
std::string Student<RollType, CourseCodeType>::levelToString(StudentLevel level)
{
    switch (level)
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
