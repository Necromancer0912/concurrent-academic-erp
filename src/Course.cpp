#include "Course.h"

// Constructor
template <typename CourseCodeType>
Course<CourseCodeType>::Course(const CourseCodeType &code, const std::string &name,
                               const std::string &instr, int cred, const std::string &sem)
    : courseCode(code), courseName(name), instructor(instr), credits(cred), semester(sem) {}

// Default constructor
template <typename CourseCodeType>
Course<CourseCodeType>::Course() : courseCode(CourseCodeType()), courseName(""), instructor(""), credits(0), semester("") {}

// Copy constructor
template <typename CourseCodeType>
Course<CourseCodeType>::Course(const Course &other)
    : courseCode(other.courseCode), courseName(other.courseName),
      instructor(other.instructor), credits(other.credits), semester(other.semester) {}

// Assignment operator
template <typename CourseCodeType>
Course<CourseCodeType> &Course<CourseCodeType>::operator=(const Course &other)
{
    if (this != &other)
    {
        courseCode = other.courseCode;
        courseName = other.courseName;
        instructor = other.instructor;
        credits = other.credits;
        semester = other.semester;
    }
    return *this;
}

// Getters
template <typename CourseCodeType>
CourseCodeType Course<CourseCodeType>::getCourseCode() const { return courseCode; }

template <typename CourseCodeType>
std::string Course<CourseCodeType>::getCourseName() const { return courseName; }

template <typename CourseCodeType>
std::string Course<CourseCodeType>::getInstructor() const { return instructor; }

template <typename CourseCodeType>
int Course<CourseCodeType>::getCredits() const { return credits; }

template <typename CourseCodeType>
std::string Course<CourseCodeType>::getSemester() const { return semester; }

// Setters
template <typename CourseCodeType>
void Course<CourseCodeType>::setInstructor(const std::string &instr)
{
    instructor = instr;
}

template <typename CourseCodeType>
void Course<CourseCodeType>::setSemester(const std::string &sem)
{
    semester = sem;
}

// Display
template <typename CourseCodeType>
void Course<CourseCodeType>::displayInfo() const
{
    std::cout << "Course Code: " << courseCodeToString()
              << " | Name: " << courseName
              << " | Instructor: " << instructor
              << " | Credits: " << credits
              << " | Semester: " << semester << std::endl;
}

// Comparison operators
template <typename CourseCodeType>
bool Course<CourseCodeType>::operator==(const Course &other) const
{
    return courseCode == other.courseCode;
}

template <typename CourseCodeType>
bool Course<CourseCodeType>::operator!=(const Course &other) const
{
    return !(*this == other);
}

// Convert course code to string
template <typename CourseCodeType>
std::string Course<CourseCodeType>::courseCodeToString() const
{
    if constexpr (std::is_same<CourseCodeType, std::string>::value)
    {
        return courseCode;
    }
    else
    {
        return std::to_string(courseCode);
    }
}

// Explicit template instantiations
template class Course<std::string>;
template class Course<int>;
template class Course<unsigned int>;
