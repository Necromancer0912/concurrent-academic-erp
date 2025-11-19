#ifndef ENROLLMENT_RECORD_H
#define ENROLLMENT_RECORD_H

#include <string>
#include <ctime>
#include <chrono>

// Enrollment record to track insertion order
template <typename RollType, typename CourseCodeType>
class EnrollmentRecord
{
private:
    int __record_id;
    RollType __roll_number;
    std::string __student_name;
    std::chrono::system_clock::time_point __enrollment_time;

public:
    // Constructor
    EnrollmentRecord(int id, const RollType &roll, const std::string &name);

    // Getters
    int get_record_id() const;
    RollType get_roll_number() const;
    std::string get_student_name() const;
    std::chrono::system_clock::time_point get_enrollment_time() const;

    // Display
    void display_info() const;

    // Comparison by record ID (insertion order)
    bool operator<(const EnrollmentRecord &other) const;
};

#endif // ENROLLMENT_RECORD_H
