#ifndef GRADE_H
#define GRADE_H

#include <string>
#include <iostream>

class Grade
{
private:
    double midSemMarks;
    double endSemMarks;
    double assignmentMarks;
    double totalMarks;
    double gradePoint;       // 0-10 scale
    std::string letterGrade; // A, A-, B+, etc.

    // Calculate grade from marks
    void calculateGrade();

public:
    // Constructor with marks
    Grade(double mid, double end, double assignment);

    // Constructor with direct grade
    Grade(double gradePoint);

    // Default constructor
    Grade();

    // Copy constructor
    Grade(const Grade &other);

    // Assignment operator
    Grade &operator=(const Grade &other);

    // Getters
    double getMidSemMarks() const;
    double getEndSemMarks() const;
    double getAssignmentMarks() const;
    double getTotalMarks() const;
    double getGradePoint() const;
    std::string getLetterGrade() const;

    // Setters
    void setMarks(double mid, double end, double assignment);
    void setGradePoint(double gp);

    // Display
    void displayInfo() const;

    // Comparison operators
    bool operator>=(double minGrade) const;
    bool operator<(const Grade &other) const;
    bool operator>(const Grade &other) const;
    bool operator==(const Grade &other) const;

    // Static helper for grade calculation
    static std::string calculateLetterGrade(double gradePoint);
};

#endif // GRADE_H
