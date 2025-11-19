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
    CourseCodeType courseCode;
    std::string courseName;
    std::string instructor;
    int credits;
    std::string semester; // e.g., "Monsoon 2025", "Winter 2025"

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
    CourseCodeType getCourseCode() const;
    std::string getCourseName() const;
    std::string getInstructor() const;
    int getCredits() const;
    std::string getSemester() const;

    // Setters
    void setInstructor(const std::string &instructor);
    void setSemester(const std::string &semester);

    // Display
    void displayInfo() const;

    // Comparison operators
    bool operator==(const Course &other) const;
    bool operator!=(const Course &other) const;

    // Convert course code to string for display
    std::string courseCodeToString() const;
};

#endif // COURSE_H
