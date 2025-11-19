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
        rollStr = it->get_roll_number();
    }
    else
    {
        rollStr = std::to_string(it->get_roll_number());
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
    : __is_sorted(false), nextRecordId(1), nextRequestId(1) {}

// Helper to convert roll number to string key
template <typename RollType, typename CourseCodeType>
std::string ERPSystem<RollType, CourseCodeType>::roll_to_string(const RollType &roll) const
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
void ERPSystem<RollType, CourseCodeType>::add_student(std::shared_ptr<Student<RollType, CourseCodeType>> student)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::string rollKey = roll_to_string(student->get_roll_number());

    if (__students.find(rollKey) != __students.end())
    {
        throw std::runtime_error("Student with roll number " + rollKey + " already exists.");
    }

    __students[rollKey] = student;
    __enrollment_records.emplace_back(nextRecordId++, student->get_roll_number(), student->get_name());

    // Update grade index
    update_grade_index(student);

    // Mark as unsorted
    __is_sorted = false;
}

// Remove student
template <typename RollType, typename CourseCodeType>
bool ERPSystem<RollType, CourseCodeType>::remove_student(const RollType &rollNumber)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::string rollKey = roll_to_string(rollNumber);
    auto it = __students.find(rollKey);

    if (it == __students.end())
    {
        return false;
    }

    // Remove from enrollment records
    __enrollment_records.erase(
        std::remove_if(__enrollment_records.begin(), __enrollment_records.end(),
                       [&rollNumber](const EnrollmentRecord<RollType, CourseCodeType> &record)
                       {
                           return record.get_roll_number() == rollNumber;
                       }),
        __enrollment_records.end());

    // Remove from __students map
    __students.erase(it);

    // Mark as unsorted (sorted list may need updating)
    __is_sorted = false;

    return true;
}

// Find student
template <typename RollType, typename CourseCodeType>
std::shared_ptr<Student<RollType, CourseCodeType>>
ERPSystem<RollType, CourseCodeType>::find_student(const RollType &rollNumber) const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::string rollKey = roll_to_string(rollNumber);
    auto it = __students.find(rollKey);

    if (it != __students.end())
    {
        return it->second;
    }

    return nullptr;
}

// Update student grade and refresh index
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::update_student_grade(
    const RollType &rollNumber, const CourseCodeType &courseCode, const Grade &grade)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    auto student = find_student(rollNumber);
    if (student)
    {
        student->update_grade(courseCode, grade);
        update_grade_index(student);
    }
}

// Update grade index
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::update_grade_index(
    std::shared_ptr<Student<RollType, CourseCodeType>> student)
{

    // Get all previous courses (completed courses with grades)
    auto prevCourses = student->get_previous_courses();
    auto prevGrades = student->get_previous_grades();

    for (size_t i = 0; i < prevCourses.size(); ++i)
    {
        CourseCodeType courseCode = prevCourses[i].get_course_code();
        double gradePoint = prevGrades[i].get_grade_point();

        // Add to grade index
        __grade_index[courseCode].insert({gradePoint, student});
    }
}

// Get all __students
template <typename RollType, typename CourseCodeType>
std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
ERPSystem<RollType, CourseCodeType>::get_all_students() const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    // If we have a sorted list, return it
    if (__is_sorted && !__sorted_students.empty())
    {
        return __sorted_students;
    }

    // Otherwise return from the map
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> result;
    for (const auto &pair : __students)
    {
        result.push_back(pair.second);
    }
    return result;
}

// Set sorted __students
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::set_sorted_students(
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> sorted)
{
    std::lock_guard<std::mutex> lock(systemMutex);
    __sorted_students = sorted;
    __is_sorted = true;
}

// Check if sorted
template <typename RollType, typename CourseCodeType>
bool ERPSystem<RollType, CourseCodeType>::get_is_sorted() const
{
    return __is_sorted;
}

// Iterators for insertion order
template <typename RollType, typename CourseCodeType>
InsertionOrderIterator<RollType, CourseCodeType>
ERPSystem<RollType, CourseCodeType>::beginInsertionOrder() const
{
    return InsertionOrderIterator<RollType, CourseCodeType>(__enrollment_records.begin(), &__students);
}

template <typename RollType, typename CourseCodeType>
InsertionOrderIterator<RollType, CourseCodeType>
ERPSystem<RollType, CourseCodeType>::endInsertionOrder() const
{
    return InsertionOrderIterator<RollType, CourseCodeType>(__enrollment_records.end(), &__students);
}

// Iterators for sorted order
template <typename RollType, typename CourseCodeType>
SortedOrderIterator<RollType, CourseCodeType>
ERPSystem<RollType, CourseCodeType>::beginSorted() const
{
    return SortedOrderIterator<RollType, CourseCodeType>(__sorted_students.begin());
}

template <typename RollType, typename CourseCodeType>
SortedOrderIterator<RollType, CourseCodeType>
ERPSystem<RollType, CourseCodeType>::endSorted() const
{
    return SortedOrderIterator<RollType, CourseCodeType>(__sorted_students.end());
}

// Search __students with grade >= threshold in a course
template <typename RollType, typename CourseCodeType>
std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
ERPSystem<RollType, CourseCodeType>::findStudentsWithGrade(
    const CourseCodeType &courseCode, double minGrade) const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> result;

    auto courseIt = __grade_index.find(courseCode);
    if (courseIt != __grade_index.end())
    {
        // Use lower_bound to find __students with grade >= minGrade
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
void ERPSystem<RollType, CourseCodeType>::rebuild_grade_index()
{
    std::lock_guard<std::mutex> lock(systemMutex);

    __grade_index.clear();

    for (const auto &pair : __students)
    {
        update_grade_index(pair.second);
    }
}

// Get statistics
template <typename RollType, typename CourseCodeType>
size_t ERPSystem<RollType, CourseCodeType>::getStudentCount() const
{
    std::lock_guard<std::mutex> lock(systemMutex);
    return __students.size();
}

// Display all __students in insertion order
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::display_all_insertion_order() const
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "Students in Insertion Order" << std::endl;
    std::cout << "========================================" << std::endl;

    int count = 1;
    for (auto it = beginInsertionOrder(); it != endInsertionOrder(); ++it)
    {
        std::cout << count++ << ". ";
        (*it)->display_info();
    }

    std::cout << "========================================\n"
              << std::endl;
}

// Display all __students in sorted order
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::display_all_sorted() const
{
    if (!__is_sorted)
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
        (*it)->display_info();
    }

    std::cout << "========================================\n"
              << std::endl;
}

// Search by name
template <typename RollType, typename CourseCodeType>
std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
ERPSystem<RollType, CourseCodeType>::search_by_name(const std::string &name) const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> result;

    for (const auto &pair : __students)
    {
        if (pair.second->get_name().find(name) != std::string::npos)
        {
            result.push_back(pair.second);
        }
    }

    return result;
}

// Search by branch
template <typename RollType, typename CourseCodeType>
std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
ERPSystem<RollType, CourseCodeType>::search_by_branch(const Branch &branch) const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> result;

    for (const auto &pair : __students)
    {
        if (pair.second->get_branch() == branch)
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
    auto student = find_student(rollNumber);
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
        student->get_name(),
        courseCode,
        courseName);

    __pending_requests.push_back(request);

    return request.get_request_id();
}

// Get all pending requests
template <typename RollType, typename CourseCodeType>
std::vector<PendingCourseRequest<RollType, CourseCodeType>>
ERPSystem<RollType, CourseCodeType>::get_pending_requests() const
{
    std::lock_guard<std::mutex> lock(systemMutex);
    return __pending_requests;
}

// Get pending requests for a specific student
template <typename RollType, typename CourseCodeType>
std::vector<PendingCourseRequest<RollType, CourseCodeType>>
ERPSystem<RollType, CourseCodeType>::get_pending_requests_by_student(const RollType &rollNumber) const
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::vector<PendingCourseRequest<RollType, CourseCodeType>> result;

    for (const auto &request : __pending_requests)
    {
        if (request.get_roll_number() == rollNumber)
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
    for (auto &request : __pending_requests)
    {
        if (request.get_request_id() == requestId)
        {
            return &request;
        }
    }
    return nullptr;
}

// Approve a pending request
template <typename RollType, typename CourseCodeType>
bool ERPSystem<RollType, CourseCodeType>::approve_pending_request(
    int requestId, const std::string &adminUsername, const std::string &remarks)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    auto *request = findPendingRequest(requestId);
    if (!request || request->get_status() != RequestStatus::PENDING)
    {
        return false;
    }

    // Approve the request
    request->approve(adminUsername, remarks);

    return true;
}

// Reject a pending request
template <typename RollType, typename CourseCodeType>
bool ERPSystem<RollType, CourseCodeType>::reject_pending_request(
    int requestId, const std::string &adminUsername, const std::string &remarks)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    auto *request = findPendingRequest(requestId);
    if (!request || request->get_status() != RequestStatus::PENDING)
    {
        return false;
    }

    // Reject the request
    request->reject(adminUsername, remarks);

    return true;
}

// Remove a pending request (after processing)
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::remove_pending_request(int requestId)
{
    std::lock_guard<std::mutex> lock(systemMutex);

    __pending_requests.erase(
        std::remove_if(__pending_requests.begin(), __pending_requests.end(),
                       [requestId](const PendingCourseRequest<RollType, CourseCodeType> &req)
                       {
                           return req.get_request_id() == requestId;
                       }),
        __pending_requests.end());
}

// Load insertion order from file and rebuild __enrollment_records
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::load_insertion_order_from_file()
{
    std::lock_guard<std::mutex> lock(systemMutex);

    Database db;
    std::vector<RollType> rollNumbers = db.loadInsertionOrder<RollType>();

    if (rollNumbers.empty())
    {
        return; // No insertion order file exists or is empty
    }

    // Rebuild __enrollment_records in the correct order
    __enrollment_records.clear();
    nextRecordId = 1;

    for (const auto &roll : rollNumbers)
    {
        std::string rollKey = roll_to_string(roll);
        auto it = __students.find(rollKey);
        if (it != __students.end())
        {
            __enrollment_records.emplace_back(nextRecordId++, roll, it->second->get_name());
        }
    }
}

// Save current insertion order to file
template <typename RollType, typename CourseCodeType>
void ERPSystem<RollType, CourseCodeType>::save_insertion_order_to_file()
{
    std::lock_guard<std::mutex> lock(systemMutex);

    std::vector<RollType> rollNumbers;
    for (const auto &record : __enrollment_records)
    {
        rollNumbers.push_back(record.get_roll_number());
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
