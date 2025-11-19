#include "EnrollmentRecord.h"
#include <iostream>
#include <iomanip>

// Constructor
template <typename RollType, typename CourseCodeType>
EnrollmentRecord<RollType, CourseCodeType>::EnrollmentRecord(int id, const RollType &roll, const std::string &name)
    : recordId(id), rollNumber(roll), studentName(name),
      enrollmentTime(std::chrono::system_clock::now()) {}

// Getters
template <typename RollType, typename CourseCodeType>
int EnrollmentRecord<RollType, CourseCodeType>::getRecordId() const
{
    return recordId;
}

template <typename RollType, typename CourseCodeType>
RollType EnrollmentRecord<RollType, CourseCodeType>::getRollNumber() const
{
    return rollNumber;
}

template <typename RollType, typename CourseCodeType>
std::string EnrollmentRecord<RollType, CourseCodeType>::getStudentName() const
{
    return studentName;
}

template <typename RollType, typename CourseCodeType>
std::chrono::system_clock::time_point EnrollmentRecord<RollType, CourseCodeType>::getEnrollmentTime() const
{
    return enrollmentTime;
}

// Display
template <typename RollType, typename CourseCodeType>
void EnrollmentRecord<RollType, CourseCodeType>::displayInfo() const
{
    auto time_t_now = std::chrono::system_clock::to_time_t(enrollmentTime);
    std::cout << "Record ID: " << recordId
              << " | Name: " << studentName
              << " | Roll: ";
    if constexpr (std::is_same<RollType, std::string>::value)
    {
        std::cout << rollNumber;
    }
    else
    {
        std::cout << rollNumber;
    }
    std::cout << " | Enrolled: " << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S")
              << std::endl;
}

// Comparison by record ID
template <typename RollType, typename CourseCodeType>
bool EnrollmentRecord<RollType, CourseCodeType>::operator<(const EnrollmentRecord &other) const
{
    return recordId < other.recordId;
}

// Explicit template instantiations
template class EnrollmentRecord<std::string, std::string>;
template class EnrollmentRecord<std::string, int>;
template class EnrollmentRecord<unsigned int, std::string>;
template class EnrollmentRecord<unsigned int, int>;
