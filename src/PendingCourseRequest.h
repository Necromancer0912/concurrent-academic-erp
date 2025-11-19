#ifndef PENDING_COURSE_REQUEST_H
#define PENDING_COURSE_REQUEST_H

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Enum for request status
enum class RequestStatus
{
    PENDING,
    APPROVED,
    REJECTED
};

// Convert RequestStatus to string
inline std::string request_status_to_string(RequestStatus status)
{
    switch (status)
    {
    case RequestStatus::PENDING:
        return "PENDING";
    case RequestStatus::APPROVED:
        return "APPROVED";
    case RequestStatus::REJECTED:
        return "REJECTED";
    default:
        return "UNKNOWN";
    }
}

// Convert string to RequestStatus
inline RequestStatus string_to_request_status(const std::string &str)
{
    if (str == "PENDING")
        return RequestStatus::PENDING;
    else if (str == "APPROVED")
        return RequestStatus::APPROVED;
    else if (str == "REJECTED")
        return RequestStatus::REJECTED;
    else
        throw std::invalid_argument("Invalid request status string: " + str);
}

// Pending Course Request class
template <typename RollType, typename CourseCodeType>
class PendingCourseRequest
{
private:
    int __request_id;
    RollType __roll_number;
    std::string __student_name;
    CourseCodeType __course_code;
    std::string __course_name;
    RequestStatus __status;
    std::chrono::system_clock::time_point __request_time;
    std::chrono::system_clock::time_point __processed_time;
    std::string __processed_by; // Admin username who approved/rejected
    std::string __remarks;      // Optional remarks from admin

public:
    // Constructor
    PendingCourseRequest(int id, const RollType &roll, const std::string &name,
                         const CourseCodeType &code, const std::string &courseName)
        : __request_id(id), __roll_number(roll), __student_name(name),
          __course_code(code), __course_name(courseName),
          __status(RequestStatus::PENDING),
          __request_time(std::chrono::system_clock::now()),
          __processed_time{}, __processed_by(""), __remarks("")
    {
    }

    // Default constructor
    PendingCourseRequest()
        : __request_id(0), __roll_number(RollType{}), __student_name(""),
          __course_code(CourseCodeType{}), __course_name(""),
          __status(RequestStatus::PENDING),
          __request_time(std::chrono::system_clock::now()),
          __processed_time{}, __processed_by(""), __remarks("")
    {
    }

    // Getters
    int get_request_id() const { return __request_id; }
    RollType get_roll_number() const { return __roll_number; }
    std::string get_student_name() const { return __student_name; }
    CourseCodeType get_course_code() const { return __course_code; }
    std::string get_course_name() const { return __course_name; }
    RequestStatus get_status() const { return __status; }
    std::chrono::system_clock::time_point get_request_time() const { return __request_time; }
    std::chrono::system_clock::time_point get_processed_time() const { return __processed_time; }
    std::string get_processed_by() const { return __processed_by; }
    std::string get_remarks() const { return __remarks; }

    // Setters
    void set_request_id(int id) { __request_id = id; }
    void set_status(RequestStatus s) { __status = s; }
    void set_processed_time(std::chrono::system_clock::time_point t) { __processed_time = t; }
    void set_processed_by(const std::string &admin) { __processed_by = admin; }
    void set_remarks(const std::string &r) { __remarks = r; }

    // Approve the request
    void approve(const std::string &adminUsername, const std::string &adminRemarks = "")
    {
        __status = RequestStatus::APPROVED;
        __processed_time = std::chrono::system_clock::now();
        __processed_by = adminUsername;
        __remarks = adminRemarks;
    }

    // Reject the request
    void reject(const std::string &adminUsername, const std::string &adminRemarks = "")
    {
        __status = RequestStatus::REJECTED;
        __processed_time = std::chrono::system_clock::now();
        __processed_by = adminUsername;
        __remarks = adminRemarks;
    }

    // Display request information
    void display_info() const
    {
        std::cout << "Request ID: " << __request_id << "\n";
        std::cout << "Roll Number: " << __roll_number << "\n";
        std::cout << "Student Name: " << __student_name << "\n";
        std::cout << "Course Code: " << __course_code << "\n";
        std::cout << "Course Name: " << __course_name << "\n";
        std::cout << "Status: " << request_status_to_string(__status) << "\n";

        // Format request time
        auto requestTimeT = std::chrono::system_clock::to_time_t(__request_time);
        std::cout << "Request Time: " << std::put_time(std::localtime(&requestTimeT), "%Y-%m-%d %H:%M:%S") << "\n";

        if (__status != RequestStatus::PENDING)
        {
            auto processedTimeT = std::chrono::system_clock::to_time_t(__processed_time);
            std::cout << "Processed Time: " << std::put_time(std::localtime(&processedTimeT), "%Y-%m-%d %H:%M:%S") << "\n";
            std::cout << "Processed By: " << __processed_by << "\n";

            if (!__remarks.empty())
            {
                std::cout << "Remarks: " << __remarks << "\n";
            }
        }
    }

    // Get formatted request time as string
    std::string get_request_time_string() const
    {
        auto requestTimeT = std::chrono::system_clock::to_time_t(__request_time);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&requestTimeT), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // Get formatted processed time as string
    std::string get_processed_time_string() const
    {
        if (__status == RequestStatus::PENDING)
            return "N/A";

        auto processedTimeT = std::chrono::system_clock::to_time_t(__processed_time);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&processedTimeT), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // Comparison operator for sorting by request ID
    bool operator<(const PendingCourseRequest &other) const
    {
        return __request_id < other.__request_id;
    }
};

#endif // PENDING_COURSE_REQUEST_H
