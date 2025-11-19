#ifndef SEMESTERMANAGER_H
#define SEMESTERMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include "Student.h"
#include "CourseCatalog.h"

class SemesterManager
{
private:
    static const int MAX_CREDITS_PER_SEMESTER = 16;
    static const int CAPSTONE_CREDITS = 8;
    static const int RESEARCH_CREDITS = 16;

    bool courseAddEnabled;
    bool courseDropEnabled;

public:
    SemesterManager(CourseCatalog *cat);

    // Enable/disable course addition
    void setCourseAddEnabled(bool enabled);

    // Enable/disable course drops
    void setCourseDropEnabled(bool enabled);

    // Check if course addition is enabled
    bool isCourseAddEnabled() const;

    // Check if course drop is enabled
    bool isCourseDropEnabled() const;

    // Calculate CGPA
    template <typename RollType, typename CourseType>
    double calculateCGPA(const Student<RollType, CourseType> &student) const;

    // Calculate total credits
    template <typename RollType, typename CourseType>
    int getTotalCreditsCompleted(const Student<RollType, CourseType> &student) const;

    // Generate transcript
    template <typename RollType, typename CourseType>
    std::string generateTranscript(const Student<RollType, CourseType> &student) const;

    // Get status
    std::string getStatus() const;
};

// Template implementations
template <typename RollType, typename CourseType>
double SemesterManager::calculateCGPA(const Student<RollType, CourseType> &student) const
{
    return student.calculateCGPA();
}

template <typename RollType, typename CourseType>
int SemesterManager::getTotalCreditsCompleted(const Student<RollType, CourseType> &student) const
{
    int totalCredits = 0;

    auto currentCourses = student.getCurrentCourses();
    for (const auto &course : currentCourses)
    {
        totalCredits += course.getCredits();
    }

    auto previousCourses = student.getPreviousCourses();
    for (const auto &course : previousCourses)
    {
        totalCredits += course.getCredits();
    }

    return totalCredits;
}

template <typename RollType, typename CourseType>
std::string SemesterManager::generateTranscript(const Student<RollType, CourseType> &student) const
{
    std::ostringstream transcript;

    transcript << "\n"
               << std::string(80, '=') << "\n";
    transcript << "                        ACADEMIC TRANSCRIPT\n";
    transcript << "                IIIT-Delhi ERP System\n";
    transcript << std::string(80, '=') << "\n\n";

    transcript << "Student Name: " << student.getName() << "\n";
    transcript << "Roll Number: " << student.getRollNumber() << "\n";
    transcript << "Branch: " << student.getBranch().getBranchName() << "\n";
    transcript << "Level: " << Student<RollType, CourseType>::levelToString(student.getLevel()) << "\n";
    transcript << "Start Year: " << student.getStartingYear() << "\n\n";

    // Current courses
    auto currentCourses = student.getCurrentCourses();
    auto currentGrades = student.getCurrentGrades();

    if (!currentCourses.empty())
    {
        transcript << "CURRENT COURSES\n";
        transcript << std::string(80, '-') << "\n";
        transcript << std::left << std::setw(12) << "Course Code"
                   << std::setw(30) << "Course Name"
                   << std::setw(10) << "Credits"
                   << std::setw(10) << "Grade"
                   << std::setw(10) << "Points" << "\n";
        transcript << std::string(80, '-') << "\n";

        for (size_t i = 0; i < currentCourses.size(); ++i)
        {
            std::ostringstream codeStream;
            codeStream << currentCourses[i].getCourseCode();

            transcript << std::left << std::setw(12) << codeStream.str()
                       << std::setw(30) << currentCourses[i].getCourseName()
                       << std::setw(10) << currentCourses[i].getCredits();

            if (i < currentGrades.size())
            {
                auto grade = currentGrades[i];
                transcript << std::setw(10) << grade.getLetterGrade()
                           << std::setw(10) << std::fixed << std::setprecision(2)
                           << grade.getGradePoint();
            }
            else
            {
                transcript << std::setw(10) << "N/A"
                           << std::setw(10) << "N/A";
            }
            transcript << "\n";
        }
        transcript << "\n";
    }

    // Previous courses
    auto previousCourses = student.getPreviousCourses();
    auto previousGrades = student.getPreviousGrades();

    if (!previousCourses.empty())
    {
        transcript << "PREVIOUS COURSES\n";
        transcript << std::string(80, '-') << "\n";
        transcript << std::left << std::setw(12) << "Course Code"
                   << std::setw(30) << "Course Name"
                   << std::setw(10) << "Credits"
                   << std::setw(10) << "Grade"
                   << std::setw(10) << "Points" << "\n";
        transcript << std::string(80, '-') << "\n";

        for (size_t i = 0; i < previousCourses.size(); ++i)
        {
            std::ostringstream codeStream;
            codeStream << previousCourses[i].getCourseCode();

            transcript << std::left << std::setw(12) << codeStream.str()
                       << std::setw(30) << previousCourses[i].getCourseName()
                       << std::setw(10) << previousCourses[i].getCredits();

            if (i < previousGrades.size())
            {
                auto grade = previousGrades[i];
                transcript << std::setw(10) << grade.getLetterGrade()
                           << std::setw(10) << std::fixed << std::setprecision(2)
                           << grade.getGradePoint();
            }
            else
            {
                transcript << std::setw(10) << "N/A"
                           << std::setw(10) << "N/A";
            }
            transcript << "\n";
        }
        transcript << "\n";
    }

    transcript << std::string(80, '=') << "\n";
    transcript << "Total Credits Completed: " << getTotalCreditsCompleted(student) << "\n";
    transcript << "CGPA: " << std::fixed << std::setprecision(2) << calculateCGPA(student) << "\n";
    transcript << std::string(80, '=') << "\n";

    return transcript.str();
}

#endif // SEMESTERMANAGER_H
