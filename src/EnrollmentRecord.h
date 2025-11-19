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
    int recordId;
    RollType rollNumber;
    std::string studentName;
    std::chrono::system_clock::time_point enrollmentTime;

public:
    // Constructor
    EnrollmentRecord(int id, const RollType &roll, const std::string &name);

    // Getters
    int getRecordId() const;
    RollType getRollNumber() const;
    std::string getStudentName() const;
    std::chrono::system_clock::time_point getEnrollmentTime() const;

    // Display
    void displayInfo() const;

    // Comparison by record ID (insertion order)
    bool operator<(const EnrollmentRecord &other) const;
};

#endif // ENROLLMENT_RECORD_H
