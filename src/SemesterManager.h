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

    bool __course_add_enabled;
    bool __course_drop_enabled;

public:
    SemesterManager(CourseCatalog *cat);

    // Enable/disable course addition
    void set_course_add_enabled(bool enabled);

    // Enable/disable course drops
    void set_course_drop_enabled(bool enabled);

    // Check if course addition is enabled
    bool is_course_add_enabled() const;

    // Check if course drop is enabled
    bool is_course_drop_enabled() const;

    // Calculate CGPA
    template <typename RollType, typename CourseType>
    double calculate_cgpa(const Student<RollType, CourseType> &student) const;

    // Calculate total credits
    template <typename RollType, typename CourseType>
    int get_total_credits_completed(const Student<RollType, CourseType> &student) const;

    // Generate transcript
    template <typename RollType, typename CourseType>
    std::string generate_transcript(const Student<RollType, CourseType> &student) const;

    // Get status
    std::string get_status() const;
};

// Template implementations
template <typename RollType, typename CourseType>
double SemesterManager::calculate_cgpa(const Student<RollType, CourseType> &student) const
{
    return student.calculate_cgpa();
}

template <typename RollType, typename CourseType>
int SemesterManager::get_total_credits_completed(const Student<RollType, CourseType> &student) const
{
    int totalCredits = 0;

    auto currentCourses = student.get_current_courses();
    for (const auto &course : currentCourses)
    {
        totalCredits += course.get_credits();
    }

    auto previousCourses = student.get_previous_courses();
    for (const auto &course : previousCourses)
    {
        totalCredits += course.get_credits();
    }

    return totalCredits;
}

template <typename RollType, typename CourseType>
std::string SemesterManager::generate_transcript(const Student<RollType, CourseType> &student) const
{
    std::ostringstream transcript;

    transcript << "\n"
               << std::string(80, '=') << "\n";
    transcript << "                        ACADEMIC TRANSCRIPT\n";
    transcript << "                IIIT-Delhi ERP System\n";
    transcript << std::string(80, '=') << "\n\n";

    transcript << "Student Name: " << student.get_name() << "\n";
    transcript << "Roll Number: " << student.get_roll_number() << "\n";
    transcript << "Branch: " << student.get_branch().get_branch_name() << "\n";
    transcript << "Level: " << Student<RollType, CourseType>::level_to_string(student.get_level()) << "\n";
    transcript << "Start Year: " << student.get_starting_year() << "\n\n";

    // Current courses
    auto currentCourses = student.get_current_courses();
    auto currentGrades = student.get_current_grades();

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
            codeStream << currentCourses[i].get_course_code();

            transcript << std::left << std::setw(12) << codeStream.str()
                       << std::setw(30) << currentCourses[i].get_course_name()
                       << std::setw(10) << currentCourses[i].get_credits();

            if (i < currentGrades.size())
            {
                auto grade = currentGrades[i];
                transcript << std::setw(10) << grade.get_letter_grade()
                           << std::setw(10) << std::fixed << std::setprecision(2)
                           << grade.get_grade_point();
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
    auto previousCourses = student.get_previous_courses();
    auto previousGrades = student.get_previous_grades();

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
            codeStream << previousCourses[i].get_course_code();

            transcript << std::left << std::setw(12) << codeStream.str()
                       << std::setw(30) << previousCourses[i].get_course_name()
                       << std::setw(10) << previousCourses[i].get_credits();

            if (i < previousGrades.size())
            {
                auto grade = previousGrades[i];
                transcript << std::setw(10) << grade.get_letter_grade()
                           << std::setw(10) << std::fixed << std::setprecision(2)
                           << grade.get_grade_point();
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
    transcript << "Total Credits Completed: " << get_total_credits_completed(student) << "\n";
    transcript << "CGPA: " << std::fixed << std::setprecision(2) << calculate_cgpa(student) << "\n";
    transcript << std::string(80, '=') << "\n";

    return transcript.str();
}

#endif // SEMESTERMANAGER_H
