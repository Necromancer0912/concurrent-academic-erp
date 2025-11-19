#include "ERPSystem.h"
#include "Database.h"
#include <iostream>
#include <iomanip>

// ============================================================================
// InsertionOrderIterator Implementation
// ============================================================================

template <typename RollType, typename CourseCodeType>
InsertionOrderIterator<RollType, CourseCodeType>::InsertionOrderIterator(
    typename std::vector<EnrollmentRecord<RollType, CourseCodeType>>::const_iterator iter,
    const std::unordered_map<std::string, std::shared_ptr<Student<RollType, CourseCodeType>>> *map)
    : it(iter), studentsMap(map) {}

template <typename RollType, typename CourseCodeType>
std::shared_ptr<Student<RollType, CourseCodeType>>
InsertionOrderIterator<RollType, CourseCodeType>::operator*() const
{
    std::string rollStr;
    if constexpr (std::is_same<RollType, std::string>::value)
    {
        rollStr = it->getRollNumber();
    }
    else
    {
        rollStr = std::to_string(it->getRollNumber());
    }
    return studentsMap->at(rollStr);
}

template <typename RollType, typename CourseCodeType>
InsertionOrderIterator<RollType, CourseCodeType> &
InsertionOrderIterator<RollType, CourseCodeType>::operator++()
{
    ++it;
    return *this;
}

template <typename RollType, typename CourseCodeType>
bool InsertionOrderIterator<RollType, CourseCodeType>::operator!=(const InsertionOrderIterator &other) const
{
    return it != other.it;
}

template <typename RollType, typename CourseCodeType>
bool InsertionOrderIterator<RollType, CourseCodeType>::operator==(const InsertionOrderIterator &other) const
{
    return it == other.it;
}

// ============================================================================
// SortedOrderIterator Implementation
// ============================================================================

template <typename RollType, typename CourseCodeType>
SortedOrderIterator<RollType, CourseCodeType>::SortedOrderIterator(
    typename std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>::const_iterator iter)
    : it(iter) {}

template <typename RollType, typename CourseCodeType>
std::shared_ptr<Student<RollType, CourseCodeType>>
SortedOrderIterator<RollType, CourseCodeType>::operator*() const
{
    return *it;
}

template <typename RollType, typename CourseCodeType>
SortedOrderIterator<RollType, CourseCodeType> &
SortedOrderIterator<RollType, CourseCodeType>::operator++()
{
    ++it;
    return *this;
}

template <typename RollType, typename CourseCodeType>
bool SortedOrderIterator<RollType, CourseCodeType>::operator!=(const SortedOrderIterator &other) const
{
    return it != other.it;
}

template <typename RollType, typename CourseCodeType>
bool SortedOrderIterator<RollType, CourseCodeType>::operator==(const SortedOrderIterator &other) const
{
    return it == other.it;
}

// ============================================================================
// ERPSystem Implementation
// ============================================================================

template <typename RollType, typename CourseCodeType>
ERPSystem<RollType, CourseCodeType>::ERPSystem()
    : isSorted(false), nextRecordId(1), nextRequestId(1) {}

// Helper to convert roll number to string key
template <typename RollType, typename CourseCodeType>
std::string ERPSystem<RollType, CourseCodeType>::rollToString(const RollType &roll) const
{
    if constexpr (std::is_same<RollType, std::string>::value)
    {
        return roll;
    }
    else
    {
        return std::to_string(roll);
    }
}

// Add student
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::addStudent(std::shared_ptr<Student<RollType, CourseCodeType>> student)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::string rollKey = rollToString(student->getRollNumber());

    if (students.find(rollKey) != students.end())
    {
        throw std::runtime_error("Student with roll number " + rollKey + " already exists.");
    }

    students[rollKey] = student;
    enrollmentRecords.emplace_back(nextRecordId++, student->getRollNumber(), student->getName());

    // Update grade index
    updateGradeIndex(student);

    // Mark as unsorted
    isSorted = false;
}

// Remove student
template <typename RollType, typename CourseCodeType>
bool ERPSystem<RollType, CourseCodeType>::removeStudent(const RollType &rollNumber)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::string rollKey = rollToString(rollNumber);
    auto it = students.find(rollKey);

    if (it == students.end())
    {
        return false;
    }

    // Remove from enrollment records
    enrollmentRecords.erase(
        std::remove_if(enrollmentRecords.begin(), enrollmentRecords.end(),
                       [&rollNumber](const EnrollmentRecord<RollType, CourseCodeType> &record)
                       {
                           return record.getRollNumber() == rollNumber;
                       }),
        enrollmentRecords.end());

    // Remove from students map
    students.erase(it);

    // Mark as unsorted (sorted list may need updating)
    isSorted = false;

    return true;
}

// Find student
template <typename RollType, typename CourseCodeType>
std::shared_ptr<Student<RollType, CourseCodeType>>
ERPSystem<RollType, CourseCodeType>::findStudent(const RollType &rollNumber) const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::string rollKey = rollToString(rollNumber);
    auto it = students.find(rollKey);

    if (it != students.end())
    {
        return it->second;
    }

    return nullptr;
}

// Update student grade and refresh index
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::updateStudentGrade(
    const RollType &rollNumber, const CourseCodeType &courseCode, const Grade &grade)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    auto student = findStudent(rollNumber);
    if (student)
    {
        student->updateGrade(courseCode, grade);
        updateGradeIndex(student);
    }
}

// Update grade index
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::updateGradeIndex(
    std::shared_ptr<Student<RollType, CourseCodeType>> student)
{

    // Get all previous courses (completed courses with grades)
    auto prevCourses = student->getPreviousCourses();
    auto prevGrades = student->getPreviousGrades();

    for (size_t i = 0; i < prevCourses.size(); ++i)
    {
        CourseCodeType courseCode = prevCourses[i].getCourseCode();
        double gradePoint = prevGrades[i].getGradePoint();

        // Add to grade index
        gradeIndex[courseCode].insert({gradePoint, student});
    }
}

// Get all students
template <typename RollType, typename CourseCodeType>
std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
ERPSystem<RollType, CourseCodeType>::getAllStudents() const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    // If we have a sorted list, return it
    if (isSorted && !sortedStudents.empty())
    {
        return sortedStudents;
    }

    // Otherwise return from the map
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> result;
    for (const auto &pair : students)
    {
        result.push_back(pair.second);
    }
    return result;
}

// Set sorted students
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::setSortedStudents(
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> sorted)
{
    std::lock_guard<std::mutex> lock(systemMutex);
    sortedStudents = sorted;
    isSorted = true;
}

// Check if sorted
template <typename RollType, typename CourseCodeType>
bool ERPSystem<RollType, CourseCodeType>::getIsSorted() const
{
    return isSorted;
}

// Iterators for insertion order
template <typename RollType, typename CourseCodeType>
InsertionOrderIterator<RollType, CourseCodeType>
ERPSystem<RollType, CourseCodeType>::beginInsertionOrder() const
{
    return InsertionOrderIterator<RollType, CourseCodeType>(enrollmentRecords.begin(), &students);
}

template <typename RollType, typename CourseCodeType>
InsertionOrderIterator<RollType, CourseCodeType>
ERPSystem<RollType, CourseCodeType>::endInsertionOrder() const
{
    return InsertionOrderIterator<RollType, CourseCodeType>(enrollmentRecords.end(), &students);
}

// Iterators for sorted order
template <typename RollType, typename CourseCodeType>
SortedOrderIterator<RollType, CourseCodeType>
ERPSystem<RollType, CourseCodeType>::beginSorted() const
{
    return SortedOrderIterator<RollType, CourseCodeType>(sortedStudents.begin());
}

template <typename RollType, typename CourseCodeType>
SortedOrderIterator<RollType, CourseCodeType>
ERPSystem<RollType, CourseCodeType>::endSorted() const
{
    return SortedOrderIterator<RollType, CourseCodeType>(sortedStudents.end());
}

// Search students with grade >= threshold in a course
template <typename RollType, typename CourseCodeType>
std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
ERPSystem<RollType, CourseCodeType>::findStudentsWithGrade(
    const CourseCodeType &courseCode, double minGrade) const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> result;

    auto courseIt = gradeIndex.find(courseCode);
    if (courseIt != gradeIndex.end())
    {
        // Use lower_bound to find students with grade >= minGrade
        auto start = courseIt->second.lower_bound(minGrade);
        for (auto it = start; it != courseIt->second.end(); ++it)
        {
            result.push_back(it->second);
        }
    }

    return result;
}

// Rebuild grade index
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::rebuildGradeIndex()
{
    std::lock_guard<std::mutex> lock(systemMutex);

    gradeIndex.clear();

    for (const auto &pair : students)
    {
        updateGradeIndex(pair.second);
    }
}

// Get statistics
template <typename RollType, typename CourseCodeType>
size_t ERPSystem<RollType, CourseCodeType>::getStudentCount() const
{
    std::lock_guard<std::mutex> lock(systemMutex);
    return students.size();
}

// Display all students in insertion order
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::displayAllInsertionOrder() const
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "Students in Insertion Order" << std::endl;
    std::cout << "========================================" << std::endl;

    int count = 1;
    for (auto it = beginInsertionOrder(); it != endInsertionOrder(); ++it)
    {
        std::cout << count++ << ". ";
        (*it)->displayInfo();
    }

    std::cout << "========================================\n"
              << std::endl;
}

// Display all students in sorted order
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::displayAllSorted() const
{
    if (!isSorted)
    {
        std::cout << "Students are not sorted yet. Please sort first." << std::endl;
        return;
    }

    std::cout << "\n========================================" << std::endl;
    std::cout << "Students in Sorted Order" << std::endl;
    std::cout << "========================================" << std::endl;

    int count = 1;
    for (auto it = beginSorted(); it != endSorted(); ++it)
    {
        std::cout << count++ << ". ";
        (*it)->displayInfo();
    }

    std::cout << "========================================\n"
              << std::endl;
}

// Search by name
template <typename RollType, typename CourseCodeType>
std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
ERPSystem<RollType, CourseCodeType>::searchByName(const std::string &name) const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> result;

    for (const auto &pair : students)
    {
        if (pair.second->getName().find(name) != std::string::npos)
        {
            result.push_back(pair.second);
        }
    }

    return result;
}

// Search by branch
template <typename RollType, typename CourseCodeType>
std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
ERPSystem<RollType, CourseCodeType>::searchByBranch(const Branch &branch) const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> result;

    for (const auto &pair : students)
    {
        if (pair.second->getBranch() == branch)
        {
            result.push_back(pair.second);
        }
    }

    return result;
}

// ============================================================================
// Pending Course Request Management
// ============================================================================

// Add a pending course registration request
template <typename RollType, typename CourseCodeType>
int ERPSystem<RollType, CourseCodeType>::addPendingRequest(
    const RollType &rollNumber, const CourseCodeType &courseCode)
{
    // Find the student first (this locks and unlocks internally)
    auto student = findStudent(rollNumber);
    if (!student)
    {
        throw std::runtime_error("Student not found");
    }

    // Get course name (you'll need to pass this from CourseCatalog)
    std::string courseName;
    if constexpr (std::is_same<CourseCodeType, std::string>::value)
    {
        courseName = courseCode; // Simplified - you can enhance this
    }
    else
    {
        courseName = std::to_string(courseCode);
    }

    // Now lock for modifying the pending requests
    std::lock_guard<std::mutex> lock(systemMutex);

    // Create pending request
    PendingCourseRequest<RollType, CourseCodeType> request(
        nextRequestId++,
        rollNumber,
        student->getName(),
        courseCode,
        courseName);

    pendingRequests.push_back(request);

    return request.getRequestId();
}

// Get all pending requests
template <typename RollType, typename CourseCodeType>
std::vector<PendingCourseRequest<RollType, CourseCodeType>>
ERPSystem<RollType, CourseCodeType>::getPendingRequests() const
{
    std::lock_guard<std::mutex> lock(systemMutex);
    return pendingRequests;
}

// Get pending requests for a specific student
template <typename RollType, typename CourseCodeType>
std::vector<PendingCourseRequest<RollType, CourseCodeType>>
ERPSystem<RollType, CourseCodeType>::getPendingRequestsByStudent(const RollType &rollNumber) const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::vector<PendingCourseRequest<RollType, CourseCodeType>> result;

    for (const auto &request : pendingRequests)
    {
        if (request.getRollNumber() == rollNumber)
        {
            result.push_back(request);
        }
    }

    return result;
}

// Find a specific pending request
template <typename RollType, typename CourseCodeType>
PendingCourseRequest<RollType, CourseCodeType> *
ERPSystem<RollType, CourseCodeType>::findPendingRequest(int requestId)
{
    for (auto &request : pendingRequests)
    {
        if (request.getRequestId() == requestId)
        {
            return &request;
        }
    }
    return nullptr;
}

// Approve a pending request
template <typename RollType, typename CourseCodeType>
bool ERPSystem<RollType, CourseCodeType>::approvePendingRequest(
    int requestId, const std::string &adminUsername, const std::string &remarks)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    auto *request = findPendingRequest(requestId);
    if (!request || request->getStatus() != RequestStatus::PENDING)
    {
        return false;
    }

    // Approve the request
    request->approve(adminUsername, remarks);

    return true;
}

// Reject a pending request
template <typename RollType, typename CourseCodeType>
bool ERPSystem<RollType, CourseCodeType>::rejectPendingRequest(
    int requestId, const std::string &adminUsername, const std::string &remarks)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    auto *request = findPendingRequest(requestId);
    if (!request || request->getStatus() != RequestStatus::PENDING)
    {
        return false;
    }

    // Reject the request
    request->reject(adminUsername, remarks);

    return true;
}

// Remove a pending request (after processing)
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::removePendingRequest(int requestId)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    pendingRequests.erase(
        std::remove_if(pendingRequests.begin(), pendingRequests.end(),
                       [requestId](const PendingCourseRequest<RollType, CourseCodeType> &req)
                       {
                           return req.getRequestId() == requestId;
                       }),
        pendingRequests.end());
}

// Load insertion order from file and rebuild enrollmentRecords
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::loadInsertionOrderFromFile()
{
    std::lock_guard<std::mutex> lock(systemMutex);

    Database db;
    std::vector<RollType> rollNumbers = db.loadInsertionOrder<RollType>();

    if (rollNumbers.empty())
    {
        return; // No insertion order file exists or is empty
    }

    // Rebuild enrollmentRecords in the correct order
    enrollmentRecords.clear();
    nextRecordId = 1;

    for (const auto &roll : rollNumbers)
    {
        std::string rollKey = rollToString(roll);
        auto it = students.find(rollKey);
        if (it != students.end())
        {
            enrollmentRecords.emplace_back(nextRecordId++, roll, it->second->getName());
        }
    }
}

// Save current insertion order to file
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::saveInsertionOrderToFile()
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::vector<RollType> rollNumbers;
    for (const auto &record : enrollmentRecords)
    {
        rollNumbers.push_back(record.getRollNumber());
    }

    Database db;
    db.saveInsertionOrder(rollNumbers);
}

// Explicit template instantiations
template class ERPSystem<std::string, std::string>;
template class ERPSystem<std::string, int>;
template class ERPSystem<unsigned int, std::string>;
template class ERPSystem<unsigned int, int>;

template class InsertionOrderIterator<std::string, std::string>;
template class InsertionOrderIterator<std::string, int>;
template class SortedOrderIterator<std::string, std::string>;
template class SortedOrderIterator<std::string, int>;

template class PendingCourseRequest<std::string, std::string>;
template class PendingCourseRequest<std::string, int>;
template class PendingCourseRequest<unsigned int, std::string>;
template class PendingCourseRequest<unsigned int, int>;
