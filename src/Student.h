#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include "Course.h"
#include "Grade.h"
#include "Branch.h"

// Custom Exception Classes
class InvalidNameException : public std::runtime_error
{
public:
    explicit InvalidNameException(const std::string &msg) : std::runtime_error(msg) {}
};

class InvalidRollNumberException : public std::runtime_error
{
public:
    explicit InvalidRollNumberException(const std::string &msg) : std::runtime_error(msg) {}
};

// Enum for student level
enum class StudentLevel
{
    BTECH,
    MTECH,
    PHD,
    DUAL_DEGREE
};

// Template class for Student to handle different roll number and course code types
template <typename RollType, typename CourseCodeType>
class Student
{
private:
    std::string name;
    RollType rollNumber;
    Branch branch;
    StudentLevel level;
    int startingYear;

    // Current courses with grades (marks)
    std::vector<Course<CourseCodeType>> currentCourses;
    std::vector<Grade> currentGrades;

    // Previous courses with grades
    std::vector<Course<CourseCodeType>> previousCourses;
    std::vector<Grade> previousGrades;

    // Loaded CGPA from import (0 if not provided)
    double loadedCGPA;

    // Validate name format
    void validateName(const std::string &n);

    // Validate roll number format
    void validateRollNumber(const RollType &roll);

public:
    // Constructor
    Student(const std::string &n, const RollType &roll, const Branch &br,
            StudentLevel lvl, int year);

    // Copy constructor
    Student(const Student &other);

    // Assignment operator
    Student &operator=(const Student &other);

    // Getters
    std::string getName() const;
    RollType getRollNumber() const;
    Branch getBranch() const;
    StudentLevel getLevel() const;
    int getStartingYear() const;
    std::vector<Course<CourseCodeType>> getCurrentCourses() const;
    std::vector<Grade> getCurrentGrades() const;
    std::vector<Course<CourseCodeType>> getPreviousCourses() const;
    std::vector<Grade> getPreviousGrades() const;

    // Setters
    void setName(const std::string &n);
    void setBranch(const Branch &br);

    // Course management
    void addCurrentCourse(const Course<CourseCodeType> &course);
    void addCurrentCourse(const Course<CourseCodeType> &course, const Grade &grade);
    void removeCurrentCourse(const CourseCodeType &courseCode);

    // Grade management
    void updateGrade(const CourseCodeType &courseCode, const Grade &grade);
    void updateGradeFromMarks(const CourseCodeType &courseCode,
                              double midSem, double endSem, double assignment);

    // Move course from current to previous
    void moveToPreviousCourses(const CourseCodeType &courseCode);

    // Get specific grade
    Grade getGradeForCourse(const CourseCodeType &courseCode) const;

    // Loaded CGPA accessor
    double getLoadedCGPA() const;
    void setLoadedCGPA(double cgpa);

    // CGPA calculation
    double calculateCGPA() const;
    double calculateSGPA() const; // For current semester

    // Search functionality
    bool hasGradeInCourse(const CourseCodeType &courseCode, double minGrade) const;

    // Display
    void displayInfo() const;
    void displayDetailed() const;

    // Comparison operators for sorting
    bool operator<(const Student &other) const;
    bool operator>(const Student &other) const;
    bool operator==(const Student &other) const;

    // Static helper
    static std::string levelToString(StudentLevel level);
};

// Type aliases for convenience
using IIITDStudent = Student<std::string, std::string>; // IIIT-D uses string roll numbers and course codes
using IITDStudent = Student<std::string, int>;          // IIT-D uses string roll numbers and integer course codes

#endif // STUDENT_H
