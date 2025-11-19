#include "EnrollmentRecord.h"
#include <iostream>
#include <iomanip>

// Constructor
template <typename RollType, typename CourseCodeType>
EnrollmentRecord<RollType, CourseCodeType>::EnrollmentRecord(int id, const RollType &roll, const std::string &name)
    : __record_id(id), __roll_number(roll), __student_name(name),
      __enrollment_time(std::chrono::system_clock::now()) {}

// Getters
template <typename RollType, typename CourseCodeType>
int EnrollmentRecord<RollType, CourseCodeType>::get_record_id() const
{
    return __record_id;
}

template <typename RollType, typename CourseCodeType>
RollType EnrollmentRecord<RollType, CourseCodeType>::get_roll_number() const
{
    return __roll_number;
}

template <typename RollType, typename CourseCodeType>
std::string EnrollmentRecord<RollType, CourseCodeType>::get_student_name() const
{
    return __student_name;
}

template <typename RollType, typename CourseCodeType>
std::chrono::system_clock::time_point EnrollmentRecord<RollType, CourseCodeType>::get_enrollment_time() const
{
    return __enrollment_time;
}

// Display
template <typename RollType, typename CourseCodeType>
void EnrollmentRecord<RollType, CourseCodeType>::display_info() const
{
    auto time_t_now = std::chrono::system_clock::to_time_t(__enrollment_time);
    std::cout << "Record ID: " << __record_id
              << " | Name: " << __student_name
              << " | Roll: ";
    if constexpr (std::is_same<RollType, std::string>::value)
    {
        std::cout << __roll_number;
    }
    else
    {
        std::cout << __roll_number;
    }
    std::cout << " | Enrolled: " << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S")
              << std::endl;
}

// Comparison by record ID
template <typename RollType, typename CourseCodeType>
bool EnrollmentRecord<RollType, CourseCodeType>::operator<(const EnrollmentRecord &other) const
{
    return __record_id < other.__record_id;
}

// Explicit template instantiations
template class EnrollmentRecord<std::string, std::string>;
template class EnrollmentRecord<std::string, int>;
template class EnrollmentRecord<unsigned int, std::string>;
template class EnrollmentRecord<unsigned int, int>;
