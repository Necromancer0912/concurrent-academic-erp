#ifndef SORTING_MANAGER_H
#define SORTING_MANAGER_H

#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <algorithm>
#include <iostream>
#include <memory>
#include <mutex>
#include "Student.h"

// Sorting criteria enum
enum class SortCriteria
{
    BY_NAME,
    BY_ROLL_NUMBER,
    BY_CGPA,
    BY_STARTING_YEAR,
    BY_BRANCH
};

// Thread logging structure
struct ThreadLog
{
    int threadId;
    std::chrono::milliseconds duration;
    size_t elementsProcessed;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
};

// Sorting Manager for parallel sorting
template <typename RollType, typename CourseCodeType>
class SortingManager
{
private:
    int numThreads;
    std::vector<ThreadLog> threadLogs;
    SortCriteria criteria;
    mutable std::mutex logsMutex; // protect threadLogs

    // Comparison function based on criteria
    bool compare(const std::shared_ptr<Student<RollType, CourseCodeType>> &a,
                 const std::shared_ptr<Student<RollType, CourseCodeType>> &b) const;

    // Merge sort helper
    void mergeSort(std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> &arr,
                   int left, int right, int threadId);

    // Merge two sorted arrays
    void merge(std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> &arr,
               int left, int mid, int right);

    // Thread function for parallel sorting
    void sortSegment(std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> &arr,
                     int left, int right, int threadId);

public:
    // Constructor
    SortingManager(int threads = 2, SortCriteria crit = SortCriteria::BY_NAME);

    // Parallel sort function
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
    parallelSort(std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> students);

    // Set sorting criteria
    void setSortCriteria(SortCriteria crit);

    // Get thread logs
    std::vector<ThreadLog> getThreadLogs() const;

    // Display thread logs
    void displayThreadLogs() const;

    // Set number of threads
    void setNumThreads(int threads);

    // Calculate optimal thread count based on student count
    static int calculateOptimalThreads(size_t studentCount);
};

#endif // SORTING_MANAGER_H
