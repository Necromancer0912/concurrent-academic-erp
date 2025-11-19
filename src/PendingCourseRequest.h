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
inline std::string requestStatusToString(RequestStatus status)
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
inline RequestStatus stringToRequestStatus(const std::string &str)
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
    int requestId;
    RollType rollNumber;
    std::string studentName;
    CourseCodeType courseCode;
    std::string courseName;
    RequestStatus status;
    std::chrono::system_clock::time_point requestTime;
    std::chrono::system_clock::time_point processedTime;
    std::string processedBy; // Admin username who approved/rejected
    std::string remarks;     // Optional remarks from admin

public:
    // Constructor
    PendingCourseRequest(int id, const RollType &roll, const std::string &name,
                         const CourseCodeType &code, const std::string &courseName)
        : requestId(id), rollNumber(roll), studentName(name),
          courseCode(code), courseName(courseName),
          status(RequestStatus::PENDING),
          requestTime(std::chrono::system_clock::now()),
          processedTime{}, processedBy(""), remarks("")
    {
    }

    // Default constructor
    PendingCourseRequest()
        : requestId(0), rollNumber(RollType{}), studentName(""),
          courseCode(CourseCodeType{}), courseName(""),
          status(RequestStatus::PENDING),
          requestTime(std::chrono::system_clock::now()),
          processedTime{}, processedBy(""), remarks("")
    {
    }

    // Getters
    int getRequestId() const { return requestId; }
    RollType getRollNumber() const { return rollNumber; }
    std::string getStudentName() const { return studentName; }
    CourseCodeType getCourseCode() const { return courseCode; }
    std::string getCourseName() const { return courseName; }
    RequestStatus getStatus() const { return status; }
    std::chrono::system_clock::time_point getRequestTime() const { return requestTime; }
    std::chrono::system_clock::time_point getProcessedTime() const { return processedTime; }
    std::string getProcessedBy() const { return processedBy; }
    std::string getRemarks() const { return remarks; }

    // Setters
    void setRequestId(int id) { requestId = id; }
    void setStatus(RequestStatus s) { status = s; }
    void setProcessedTime(std::chrono::system_clock::time_point t) { processedTime = t; }
    void setProcessedBy(const std::string &admin) { processedBy = admin; }
    void setRemarks(const std::string &r) { remarks = r; }

    // Approve the request
    void approve(const std::string &adminUsername, const std::string &adminRemarks = "")
    {
        status = RequestStatus::APPROVED;
        processedTime = std::chrono::system_clock::now();
        processedBy = adminUsername;
        remarks = adminRemarks;
    }

    // Reject the request
    void reject(const std::string &adminUsername, const std::string &adminRemarks = "")
    {
        status = RequestStatus::REJECTED;
        processedTime = std::chrono::system_clock::now();
        processedBy = adminUsername;
        remarks = adminRemarks;
    }

    // Display request information
    void displayInfo() const
    {
        std::cout << "Request ID: " << requestId << "\n";
        std::cout << "Roll Number: " << rollNumber << "\n";
        std::cout << "Student Name: " << studentName << "\n";
        std::cout << "Course Code: " << courseCode << "\n";
        std::cout << "Course Name: " << courseName << "\n";
        std::cout << "Status: " << requestStatusToString(status) << "\n";

        // Format request time
        auto requestTimeT = std::chrono::system_clock::to_time_t(requestTime);
        std::cout << "Request Time: " << std::put_time(std::localtime(&requestTimeT), "%Y-%m-%d %H:%M:%S") << "\n";

        if (status != RequestStatus::PENDING)
        {
            auto processedTimeT = std::chrono::system_clock::to_time_t(processedTime);
            std::cout << "Processed Time: " << std::put_time(std::localtime(&processedTimeT), "%Y-%m-%d %H:%M:%S") << "\n";
            std::cout << "Processed By: " << processedBy << "\n";

            if (!remarks.empty())
            {
                std::cout << "Remarks: " << remarks << "\n";
            }
        }
    }

    // Get formatted request time as string
    std::string getRequestTimeString() const
    {
        auto requestTimeT = std::chrono::system_clock::to_time_t(requestTime);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&requestTimeT), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // Get formatted processed time as string
    std::string getProcessedTimeString() const
    {
        if (status == RequestStatus::PENDING)
            return "N/A";

        auto processedTimeT = std::chrono::system_clock::to_time_t(processedTime);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&processedTimeT), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // Comparison operator for sorting by request ID
    bool operator<(const PendingCourseRequest &other) const
    {
        return requestId < other.requestId;
    }
};

#endif // PENDING_COURSE_REQUEST_H
