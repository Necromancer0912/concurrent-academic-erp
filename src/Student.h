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
    std::string __name;
    RollType __roll_number;
    Branch __branch;
    StudentLevel __level;
    int __starting_year;

    // Current courses with grades (marks)
    std::vector<Course<CourseCodeType>> __current_courses;
    std::vector<Grade> __current_grades;

    // Previous courses with grades
    std::vector<Course<CourseCodeType>> __previous_courses;
    std::vector<Grade> __previous_grades;

    // Loaded CGPA from import (0 if not provided)
    double __loaded_cgpa;

    // Validate name format
    void __validate_name(const std::string &n);

    // Validate roll number format
    void __validate_roll_number(const RollType &roll);

public:
    // Constructor
    Student(const std::string &n, const RollType &roll, const Branch &br,
            StudentLevel lvl, int year);

    // Copy constructor
    Student(const Student &other);

    // Assignment operator
    Student &operator=(const Student &other);

    // Getters
    std::string get_name() const;
    RollType get_roll_number() const;
    Branch get_branch() const;
    StudentLevel get_level() const;
    int get_starting_year() const;
    std::vector<Course<CourseCodeType>> get_current_courses() const;
    std::vector<Grade> get_current_grades() const;
    std::vector<Course<CourseCodeType>> get_previous_courses() const;
    std::vector<Grade> get_previous_grades() const;

    // Setters
    void set_name(const std::string &n);
    void set_branch(const Branch &br);

    // Course management
    void add_current_course(const Course<CourseCodeType> &course);
    void add_current_course(const Course<CourseCodeType> &course, const Grade &grade);
    void remove_current_course(const CourseCodeType &course_code);

    // Grade management
    void update_grade(const CourseCodeType &course_code, const Grade &grade);
    void update_grade_from_marks(const CourseCodeType &course_code,
                                 double mid_sem, double end_sem, double assignment);

    // Move course from current to previous
    void move_to_previous_courses(const CourseCodeType &course_code);

    // Get specific grade
    Grade get_grade_for_course(const CourseCodeType &course_code) const;

    // Loaded CGPA accessor
    double get_loaded_cgpa() const;
    void set_loaded_cgpa(double cgpa);

    // CGPA calculation
    double calculate_cgpa() const;
    double calculate_sgpa() const; // For current semester

    // Search functionality
    bool has_grade_in_course(const CourseCodeType &course_code, double min_grade) const;

    // Display
    void display_info() const;
    void display_detailed() const;

    // Comparison operators for sorting
    bool operator<(const Student &other) const;
    bool operator>(const Student &other) const;
    bool operator==(const Student &other) const;

    // Static helper
    static std::string level_to_string(StudentLevel level);
};

// Type aliases for convenience
using ApexStudent = Student<std::string, std::string>; // Apex uses string roll numbers and course codes
using VertexStudent = Student<std::string, int>;       // Vertex uses string roll numbers and integer course codes

#endif // STUDENT_H
