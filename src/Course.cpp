#include "Course.h"

// Constructor
template <typename CourseCodeType>
Course<CourseCodeType>::Course(const CourseCodeType &_code, const std::string &_name,
                               const std::string &_instr, int _cred, const std::string &_sem)
    : __course_code(_code), __course_name(_name), __instructor(_instr), __credits(_cred), __semester(_sem) {}

// Default constructor
template <typename CourseCodeType>
Course<CourseCodeType>::Course() : __course_code(CourseCodeType()), __course_name(""), __instructor(""), __credits(0), __semester("") {}

// Copy constructor
template <typename CourseCodeType>
Course<CourseCodeType>::Course(const Course &_other)
    : __course_code(_other.__course_code), __course_name(_other.__course_name),
      __instructor(_other.__instructor), __credits(_other.__credits), __semester(_other.__semester) {}

// Assignment operator
template <typename CourseCodeType>
Course<CourseCodeType> &Course<CourseCodeType>::operator=(const Course &_other)
{
    if (this != &_other)
    {
        __course_code = _other.__course_code;
        __course_name = _other.__course_name;
        __instructor = _other.__instructor;
        __credits = _other.__credits;
        __semester = _other.__semester;
    }
    return *this;
}

// Getters
template <typename CourseCodeType>
CourseCodeType Course<CourseCodeType>::get_course_code() const { return __course_code; }

template <typename CourseCodeType>
std::string Course<CourseCodeType>::get_course_name() const { return __course_name; }

template <typename CourseCodeType>
std::string Course<CourseCodeType>::get_instructor() const { return __instructor; }

template <typename CourseCodeType>
int Course<CourseCodeType>::get_credits() const { return __credits; }

template <typename CourseCodeType>
std::string Course<CourseCodeType>::get_semester() const { return __semester; }

// Setters
template <typename CourseCodeType>
void Course<CourseCodeType>::set_instructor(const std::string &_instr)
{
    __instructor = _instr;
}

template <typename CourseCodeType>
void Course<CourseCodeType>::set_semester(const std::string &_sem)
{
    __semester = _sem;
}

// Display
template <typename CourseCodeType>
void Course<CourseCodeType>::display_info() const
{
    std::cout << "Course Code: " << course_code_to_string()
              << " | Name: " << __course_name
              << " | Instructor: " << __instructor
              << " | Credits: " << __credits
              << " | Semester: " << __semester << std::endl;
}

// Comparison operators
template <typename CourseCodeType>
bool Course<CourseCodeType>::operator==(const Course &_other) const
{
    return __course_code == _other.__course_code;
}

template <typename CourseCodeType>
bool Course<CourseCodeType>::operator!=(const Course &other) const
{
    return !(*this == other);
}

// Convert course code to string
template <typename CourseCodeType>
std::string Course<CourseCodeType>::course_code_to_string() const
{
    if constexpr (std::is_same<CourseCodeType, std::string>::value)
    {
        return __course_code;
    }
    else
    {
        return std::to_string(__course_code);
    }
}

// Explicit template instantiations
template class Course<std::string>;
template class Course<int>;
template class Course<unsigned int>;
