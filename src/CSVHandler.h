#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <stdexcept>
#include "Student.h"
#include "Course.h"
#include "Branch.h"

// CSV Exception
class CSVException : public std::runtime_error
{
public:
    explicit CSVException(const std::string &msg) : std::runtime_error(msg) {}
};

// CSV Handler class
template <typename RollType, typename CourseCodeType>
class CSVHandler
{
private:
    std::string __filename;

    // Parse a CSV line
    std::vector<std::string> parse_line(const std::string &line);

    // Convert string to appropriate type
    RollType parseRollNumber(const std::string &rollStr);
    CourseCodeType parseCourseCode(const std::string &codeStr);

    // Parse student level
    StudentLevel parseLevel(const std::string &levelStr);

    // Parse branch
    Branch parseBranch(const std::string &branchCode);

public:
    // Constructor
    CSVHandler(const std::string &filename);

    // Read students from CSV
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> read_students();

    // Read a range of student lines (start = 0-based line index after header)
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> read_students_range(int start, int count);

    // Write students to CSV
    void write_students(const std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> &students);

    // Generate sample CSV file
    static void generate_sample_csv(const std::string &filename, int numRecords);
};

#endif // CSV_HANDLER_H
