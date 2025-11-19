#ifndef ERP_SYSTEM_H
#define ERP_SYSTEM_H

#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <string>
#include <algorithm>
#include <mutex>
#include "Student.h"
#include "EnrollmentRecord.h"
#include "PendingCourseRequest.h"

// Forward iterator for insertion order
template <typename RollType, typename CourseCodeType>
class InsertionOrderIterator
{
private:
    typename std::vector<EnrollmentRecord<RollType, CourseCodeType>>::const_iterator it;
    const std::unordered_map<std::string, std::shared_ptr<Student<RollType, CourseCodeType>>> *studentsMap;

public:
    InsertionOrderIterator(
        typename std::vector<EnrollmentRecord<RollType, CourseCodeType>>::const_iterator iter,
        const std::unordered_map<std::string, std::shared_ptr<Student<RollType, CourseCodeType>>> *map);

    std::shared_ptr<Student<RollType, CourseCodeType>> operator*() const;
    InsertionOrderIterator &operator++();
    bool operator!=(const InsertionOrderIterator &other) const;
    bool operator==(const InsertionOrderIterator &other) const;
};

// Forward iterator for sorted order
template <typename RollType, typename CourseCodeType>
class SortedOrderIterator
{
private:
    typename std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>::const_iterator it;

public:
    SortedOrderIterator(typename std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>::const_iterator iter);

    std::shared_ptr<Student<RollType, CourseCodeType>> operator*() const;
    SortedOrderIterator &operator++();
    bool operator!=(const SortedOrderIterator &other) const;
    bool operator==(const SortedOrderIterator &other) const;
};

// ERP System class
template <typename RollType, typename CourseCodeType>
class ERPSystem
{
private:
    // Main storage - hash map for O(1) lookup by roll number
    std::unordered_map<std::string, std::shared_ptr<Student<RollType, CourseCodeType>>> students;

    // Enrollment records to maintain insertion order
    std::vector<EnrollmentRecord<RollType, CourseCodeType>> enrollmentRecords;

    // Sorted list (updated after sorting)
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> sortedStudents;
    bool isSorted;

    // Index for quick grade-based searches - map<courseCode, map<gradeThreshold, vector<studentPointers>>>
    std::map<CourseCodeType, std::multimap<double, std::shared_ptr<Student<RollType, CourseCodeType>>>> gradeIndex;

    // Counter for record IDs
    int nextRecordId;

    // Pending course registration requests
    std::vector<PendingCourseRequest<RollType, CourseCodeType>> pendingRequests;
    int nextRequestId;

    // Mutex for thread safety
    mutable std::mutex systemMutex;

    // Helper to convert roll number to string key
    std::string rollToString(const RollType &roll) const;

    // Update grade index
    void updateGradeIndex(std::shared_ptr<Student<RollType, CourseCodeType>> student);

public:
    // Constructor
    ERPSystem();

    // Add student
    void addStudent(std::shared_ptr<Student<RollType, CourseCodeType>> student);

    // Remove student
    bool removeStudent(const RollType &rollNumber);

    // Find student
    std::shared_ptr<Student<RollType, CourseCodeType>> findStudent(const RollType &rollNumber) const;

    // Update student grade and refresh index
    void updateStudentGrade(const RollType &rollNumber, const CourseCodeType &courseCode, const Grade &grade);

    // Get all students
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> getAllStudents() const;

    // Sort students (will be used by SortingManager)
    void setSortedStudents(std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> sorted);

    // Check if sorted
    bool getIsSorted() const;

    // Iterators for insertion order
    InsertionOrderIterator<RollType, CourseCodeType> beginInsertionOrder() const;
    InsertionOrderIterator<RollType, CourseCodeType> endInsertionOrder() const;

    // Iterators for sorted order
    SortedOrderIterator<RollType, CourseCodeType> beginSorted() const;
    SortedOrderIterator<RollType, CourseCodeType> endSorted() const;

    // Search students with grade >= threshold in a course
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
    findStudentsWithGrade(const CourseCodeType &courseCode, double minGrade) const;

    // Rebuild grade index (call after bulk updates)
    void rebuildGradeIndex();

    // Get statistics
    size_t getStudentCount() const;

    // Display all students in insertion order
    void displayAllInsertionOrder() const;

    // Display all students in sorted order
    void displayAllSorted() const;

    // Search by name
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> searchByName(const std::string &name) const;

    // Search by branch
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> searchByBranch(const Branch &branch) const;

    // Pending Course Request Management
    int addPendingRequest(const RollType &rollNumber, const CourseCodeType &courseCode);
    std::vector<PendingCourseRequest<RollType, CourseCodeType>> getPendingRequests() const;
    std::vector<PendingCourseRequest<RollType, CourseCodeType>> getPendingRequestsByStudent(const RollType &rollNumber) const;
    PendingCourseRequest<RollType, CourseCodeType> *findPendingRequest(int requestId);
    bool approvePendingRequest(int requestId, const std::string &adminUsername, const std::string &remarks = "");
    bool rejectPendingRequest(int requestId, const std::string &adminUsername, const std::string &remarks = "");
    void removePendingRequest(int requestId);

    // Insertion order management
    void loadInsertionOrderFromFile();
    void saveInsertionOrderToFile();
};

#endif // ERP_SYSTEM_H
