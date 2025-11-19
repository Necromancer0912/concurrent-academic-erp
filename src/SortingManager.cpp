#include "SortingManager.h"
#include <iostream>
#include <iomanip>
#include <cstring>

// Constructor
template <typename RollType, typename CourseCodeType>
SortingManager<RollType, CourseCodeType>::SortingManager(int threads, SortCriteria crit)
    : numThreads(threads), criteria(crit)
{
    if (numThreads < 1)
        numThreads = 1;
    if (numThreads > 8)
        numThreads = 8; // Reasonable upper limit
}

// Comparison function based on criteria
template <typename RollType, typename CourseCodeType>
bool SortingManager<RollType, CourseCodeType>::compare(
    const std::shared_ptr<Student<RollType, CourseCodeType>> &a,
    const std::shared_ptr<Student<RollType, CourseCodeType>> &b) const
{

    switch (criteria)
    {
    case SortCriteria::BY_NAME:
        return a->getName() < b->getName();

    case SortCriteria::BY_ROLL_NUMBER:
    {
        if constexpr (std::is_same<RollType, std::string>::value)
        {
            return a->getRollNumber() < b->getRollNumber();
        }
        else
        {
            return a->getRollNumber() < b->getRollNumber();
        }
    }

    case SortCriteria::BY_CGPA:
        return a->calculateCGPA() > b->calculateCGPA(); // Descending order for CGPA

    case SortCriteria::BY_STARTING_YEAR:
        return a->getStartingYear() < b->getStartingYear();

    case SortCriteria::BY_BRANCH:
        return a->getBranch().getBranchCode() < b->getBranch().getBranchCode();

    default:
        return a->getName() < b->getName();
    }
}

// Merge two sorted arrays
template <typename RollType, typename CourseCodeType>
void SortingManager<RollType, CourseCodeType>::merge(
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> &arr,
    int left, int mid, int right)
{

    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> leftArr(n1);
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> rightArr(n2);

    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (compare(leftArr[i], rightArr[j]))
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// Merge sort helper
template <typename RollType, typename CourseCodeType>
void SortingManager<RollType, CourseCodeType>::mergeSort(
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> &arr,
    int left, int right, int threadId)
{

    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid, threadId);
        mergeSort(arr, mid + 1, right, threadId);

        merge(arr, left, mid, right);
    }
}

// Thread function for sorting a segment
template <typename RollType, typename CourseCodeType>
void SortingManager<RollType, CourseCodeType>::sortSegment(
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> &arr,
    int left, int right, int threadId)
{

    auto startTime = std::chrono::steady_clock::now();

    // Use std::sort instead of mergeSort for better performance on segments
    // std::sort is typically introsort (quicksort + heapsort + insertion sort)
    // which is faster than mergeSort for most cases
    std::sort(arr.begin() + left, arr.begin() + right + 1,
              [this](const auto &a, const auto &b)
              { return compare(a, b); });

    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    // Log thread information (thread-safe)
    ThreadLog log;
    log.threadId = threadId;
    log.duration = duration;
    log.elementsProcessed = right - left + 1;
    log.startTime = startTime;
    log.endTime = endTime;

    {
        std::lock_guard<std::mutex> lg(logsMutex);
        threadLogs.push_back(log);
    }
}

// Parallel sort function
template <typename RollType, typename CourseCodeType>
std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
SortingManager<RollType, CourseCodeType>::parallelSort(
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> students)
{

    threadLogs.clear();

    if (students.empty())
    {
        return students;
    }

    int n = students.size();

    // Calculate optimal thread count based on student count
    int optimalThreads = calculateOptimalThreads(n);

    // Override numThreads with optimal value
    int effectiveThreads = optimalThreads;

    std::cout << "\n[Parallel Sort Configuration]\n";
    std::cout << "  Student count: " << n << "\n";
    std::cout << "  Hardware threads: " << std::thread::hardware_concurrency() << "\n";
    std::cout << "  Optimal threads: " << optimalThreads << "\n";
    std::cout << "  Using threads: " << effectiveThreads << "\n\n";

    // If data is small or only 1 thread, use single-threaded sort
    if (n < 100 || effectiveThreads == 1)
    {
        sortSegment(students, 0, n - 1, 0);
        return students;
    }

    // Calculate segment size for each thread (divide equally)
    int segmentSize = n / effectiveThreads;

    std::vector<std::thread> threads;
    std::vector<int> boundaries;

    // Create boundaries for each thread
    std::cout << "[Thread Workload Distribution]\n";
    for (int i = 0; i < effectiveThreads; ++i)
    {
        int left = i * segmentSize;
        int right = (i == effectiveThreads - 1) ? n - 1 : (i + 1) * segmentSize - 1;
        boundaries.push_back(left);
        boundaries.push_back(right);
        std::cout << "  Thread " << i << ": Elements [" << left << " - " << right << "] (" << (right - left + 1) << " elements)\n";
    }
    std::cout << "\n";

    // Launch threads to sort each segment
    auto sortingStart = std::chrono::steady_clock::now();
    std::cout << "[Phase 1: Parallel Sorting]\n";
    for (int i = 0; i < effectiveThreads; ++i)
    {
        int left = boundaries[i * 2];
        int right = boundaries[i * 2 + 1];
        threads.emplace_back(&SortingManager::sortSegment, this, std::ref(students), left, right, i);
    }

    // Wait for all threads to complete
    for (auto &t : threads)
    {
        t.join();
    }
    auto sortingEnd = std::chrono::steady_clock::now();
    auto sortingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(sortingEnd - sortingStart);
    std::cout << "  All sorting threads completed in " << sortingDuration.count() << " ms\n\n";

    // Merge all sorted segments using a dedicated merge thread
    std::cout << "[Phase 2: Parallel Merging]\n";
    auto mergeStartTime = std::chrono::steady_clock::now();

    // Create a lambda for the merge operation to run in a separate thread
    auto mergeLambda = [this, &students, &boundaries, effectiveThreads]() -> std::chrono::milliseconds
    {
        auto localMergeStart = std::chrono::steady_clock::now();

        // Iteratively merge adjacent sorted segments (pairwise) to reduce total work
        std::vector<std::pair<int, int>> runs;
        for (int i = 0; i < effectiveThreads; ++i)
        {
            int left = boundaries[i * 2];
            int right = boundaries[i * 2 + 1];
            runs.emplace_back(left, right);
        }

        // Pairwise merge until a single run remains
        while (runs.size() > 1)
        {
            std::vector<std::pair<int, int>> nextRuns;
            for (size_t i = 0; i < runs.size(); i += 2)
            {
                if (i + 1 < runs.size())
                {
                    int left = runs[i].first;
                    int mid = runs[i].second;
                    int right = runs[i + 1].second;
                    merge(students, left, mid, right);
                    nextRuns.emplace_back(left, right);
                }
                else
                {
                    // Odd run, carry forward
                    nextRuns.push_back(runs[i]);
                }
            }
            runs.swap(nextRuns);
        }

        auto localMergeEnd = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(localMergeEnd - localMergeStart);
    }; // Launch merge operation in a separate thread
    std::thread mergeThread([&mergeLambda, &mergeStartTime, this, n]()
                            {
        auto mergeDuration = mergeLambda();
        auto mergeEndTime = std::chrono::steady_clock::now();
        
        std::cout << "  Merge thread completed in " << mergeDuration.count() << " ms\n\n";
        
        // Log merge operation
        ThreadLog mergeLog;
        mergeLog.threadId = -1; // -1 indicates merge operation
        mergeLog.duration = mergeDuration;
        mergeLog.elementsProcessed = n;
        mergeLog.startTime = mergeStartTime;
        mergeLog.endTime = mergeEndTime;
        {
            std::lock_guard<std::mutex> lg(logsMutex);
            threadLogs.push_back(mergeLog);
        } });

    // Wait for merge thread to complete
    mergeThread.join();

    auto overallEnd = std::chrono::steady_clock::now();
    auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(overallEnd - sortingStart);
    std::cout << "[Overall Performance]\n";
    std::cout << "  Total sorting time: " << totalDuration.count() << " ms\n";
    std::cout << "  Sorted " << n << " students using " << effectiveThreads << " threads\n\n";

    return students;
}

// Set sorting criteria
template <typename RollType, typename CourseCodeType>
void SortingManager<RollType, CourseCodeType>::setSortCriteria(SortCriteria crit)
{
    criteria = crit;
}

// Get thread logs
template <typename RollType, typename CourseCodeType>
std::vector<ThreadLog> SortingManager<RollType, CourseCodeType>::getThreadLogs() const
{
    return threadLogs;
}

// Display thread logs
template <typename RollType, typename CourseCodeType>
void SortingManager<RollType, CourseCodeType>::displayThreadLogs() const
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "Thread Sorting Logs" << std::endl;
    std::cout << "========================================" << std::endl;

    for (const auto &log : threadLogs)
    {
        if (log.threadId == -1)
        {
            std::cout << "Merge Operation: ";
        }
        else
        {
            std::cout << "Thread " << log.threadId << ": ";
        }

        std::cout << "Processed " << log.elementsProcessed << " elements in "
                  << log.duration.count() << " ms" << std::endl;
    }

    // Calculate parallel execution time (maximum thread duration)\n    if (!threadLogs.empty())\n    {\n        // Find the longest thread execution time (since threads run in parallel)\n        std::chrono::milliseconds maxThreadDuration(0);\n        std::chrono::milliseconds mergeDuration(0);\n        \n        for (const auto &lg : threadLogs)\n        {\n            if (lg.threadId != -1) // Sorting threads\n            {\n                if (lg.duration > maxThreadDuration)\n                {\n                    maxThreadDuration = lg.duration;\n                }\n            }\n            else // Merge operation\n            {\n                mergeDuration = lg.duration;\n            }\n        }\n\n        std::cout << \"\\nParallel sorting phase: \" << maxThreadDuration.count() << \" ms (max thread time)\" << std::endl;\n        std::cout << \"Merge phase: \" << mergeDuration.count() << \" ms\" << std::endl;\n        std::cout << \"Total parallel time: \" << (maxThreadDuration.count() + mergeDuration.count()) << \" ms\" << std::endl;\n    }

    std::cout << "========================================\n"
              << std::endl;
}

// Set number of threads
template <typename RollType, typename CourseCodeType>
void SortingManager<RollType, CourseCodeType>::setNumThreads(int threads)
{
    numThreads = threads;
    if (numThreads < 1)
        numThreads = 1;
    if (numThreads > 8)
        numThreads = 8;
}

// Calculate optimal thread count based on student count
template <typename RollType, typename CourseCodeType>
int SortingManager<RollType, CourseCodeType>::calculateOptimalThreads(size_t studentCount)
{
    // Get hardware concurrency (max threads available)
    unsigned int maxThreads = std::thread::hardware_concurrency();
    if (maxThreads == 0)
        maxThreads = 2; // Fallback if unable to detect

    int optimalThreads;

    // Thread allocation based on student count
    if (studentCount <= 3000)
    {
        optimalThreads = 2;
    }
    else if (studentCount <= 6000)
    {
        optimalThreads = 3;
    }
    else if (studentCount <= 10000)
    {
        optimalThreads = 4;
    }
    else if (studentCount <= 15000)
    {
        optimalThreads = 5;
    }
    else
    {
        optimalThreads = 6;
    }

    // Respect hardware limits
    if (static_cast<unsigned int>(optimalThreads) > maxThreads)
    {
        optimalThreads = maxThreads;
    }

    return optimalThreads;
}

// Explicit template instantiations
template class SortingManager<std::string, std::string>;
template class SortingManager<std::string, int>;
template class SortingManager<unsigned int, std::string>;
template class SortingManager<unsigned int, int>;
