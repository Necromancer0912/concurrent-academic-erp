#ifndef COURSECATALOG_H
#define COURSECATALOG_H

#include <string>
#include <vector>
#include <map>
#include <set>

struct CourseInfo
{
    std::string courseCode;
    std::string courseName;
    int credits;
    std::string syllabus;
    std::string instructor;
    std::vector<std::string> prerequisites;
    std::set<std::string> eligibleLevels; // BTech, MTech, PhD
    int duration;                         // in semesters
    std::string courseType;               // Regular, Capstone, Research

    CourseInfo() : credits(0), duration(1) {}

    CourseInfo(const std::string &code, const std::string &name, int cred)
        : courseCode(code), courseName(name), credits(cred), duration(1), courseType("Regular") {}
};

class CourseCatalog
{
private:
    static const std::string CATALOG_FILE;
    static const std::string SYLLABUS_DIR;

    std::map<std::string, CourseInfo> courses;

    // Load catalog from file
    void loadCatalog();

    // Save catalog to file
    void saveCatalog();

public:
    CourseCatalog();

    // Add a new course to catalog
    void addCourse(const CourseInfo &course);

    // Get course information
    CourseInfo getCourseInfo(const std::string &courseCode) const;

    // Check if course exists
    bool courseExists(const std::string &courseCode) const;

    // Get all courses for a level
    std::vector<CourseInfo> getCoursesByLevel(const std::string &level) const;

    // Get all courses
    std::vector<CourseInfo> getAllCourses() const;

    // Get syllabus for a course
    std::string getSyllabus(const std::string &courseCode) const;

    // Update course information
    bool updateCourse(const CourseInfo &course);

    // Delete a course
    bool deleteCourse(const std::string &courseCode);

    // Search courses by name
    std::vector<CourseInfo> searchCourses(const std::string &query) const;

    // Check if student can take course (based on level)
    bool canTakeCourse(const std::string &courseCode, const std::string &studentLevel) const;

    // Get course credits
    int getCourseCredits(const std::string &courseCode) const;

    // Get course type (Regular, Capstone, Research)
    std::string getCourseType(const std::string &courseCode) const;

    // Initialize with default IIIT-Delhi courses
    void initializeDefaultCourses();

    // Display course details
    void displayCourse(const std::string &courseCode) const;
};

#endif // COURSECATALOG_H
