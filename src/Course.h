#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <iostream>
#include <sstream>

// Template class for Course to handle different course code types
template <typename CourseCodeType>
class Course
{
private:
    CourseCodeType __course_code;
    std::string __course_name;
    std::string __instructor;
    int __credits;
    std::string __semester; // e.g., "Monsoon 2025", "Winter 2025"

public:
    // Constructor
    Course(const CourseCodeType &code, const std::string &name,
           const std::string &instructor, int credits, const std::string &semester);

    // Default constructor
    Course();

    // Copy constructor
    Course(const Course &other);

    // Assignment operator
    Course &operator=(const Course &other);

    // Getters
    CourseCodeType get_course_code() const;
    std::string get_course_name() const;
    std::string get_instructor() const;
    int get_credits() const;
    std::string get_semester() const;

    // Setters
    void set_instructor(const std::string &instructor);
    void set_semester(const std::string &semester);

    // Display
    void display_info() const;

    // Comparison operators
    bool operator==(const Course &other) const;
    bool operator!=(const Course &other) const;

    // Convert course code to string for display
    std::string course_code_to_string() const;
};

#endif // COURSE_H
