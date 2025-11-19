#include "Grade.h"
#include <iomanip>
#include <cmath>

// Constructor with marks (30% mid, 40% end, 30% assignment)
Grade::Grade(double mid, double end, double assignment)
    : midSemMarks(mid), endSemMarks(end), assignmentMarks(assignment)
{
    calculateGrade();
}

// Constructor with direct grade
Grade::Grade(double gp) : midSemMarks(0), endSemMarks(0), assignmentMarks(0),
                          totalMarks(0), gradePoint(gp)
{
    letterGrade = calculateLetterGrade(gp);
}

// Default constructor
Grade::Grade() : midSemMarks(0), endSemMarks(0), assignmentMarks(0),
                 totalMarks(0), gradePoint(0), letterGrade("F") {}

// Copy constructor
Grade::Grade(const Grade &other)
    : midSemMarks(other.midSemMarks), endSemMarks(other.endSemMarks),
      assignmentMarks(other.assignmentMarks), totalMarks(other.totalMarks),
      gradePoint(other.gradePoint), letterGrade(other.letterGrade) {}

// Assignment operator
Grade &Grade::operator=(const Grade &other)
{
    if (this != &other)
    {
        midSemMarks = other.midSemMarks;
        endSemMarks = other.endSemMarks;
        assignmentMarks = other.assignmentMarks;
        totalMarks = other.totalMarks;
        gradePoint = other.gradePoint;
        letterGrade = other.letterGrade;
    }
    return *this;
}

// Calculate grade from marks
void Grade::calculateGrade()
{
    // Weighted calculation: 30% mid, 40% end, 30% assignment
    totalMarks = (midSemMarks * 0.30) + (endSemMarks * 0.40) + (assignmentMarks * 0.30);

    // Convert to 10-point scale
    // Assuming marks are out of 100
    if (totalMarks >= 90)
        gradePoint = 10.0;
    else if (totalMarks >= 80)
        gradePoint = 9.0;
    else if (totalMarks >= 70)
        gradePoint = 8.0;
    else if (totalMarks >= 60)
        gradePoint = 7.0;
    else if (totalMarks >= 50)
        gradePoint = 6.0;
    else if (totalMarks >= 40)
        gradePoint = 5.0;
    else if (totalMarks >= 30)
        gradePoint = 4.0;
    else
        gradePoint = 0.0;

    letterGrade = calculateLetterGrade(gradePoint);
}

// Getters
double Grade::getMidSemMarks() const { return midSemMarks; }
double Grade::getEndSemMarks() const { return endSemMarks; }
double Grade::getAssignmentMarks() const { return assignmentMarks; }
double Grade::getTotalMarks() const { return totalMarks; }
double Grade::getGradePoint() const { return gradePoint; }
std::string Grade::getLetterGrade() const { return letterGrade; }

// Setters
void Grade::setMarks(double mid, double end, double assignment)
{
    midSemMarks = mid;
    endSemMarks = end;
    assignmentMarks = assignment;
    calculateGrade();
}

void Grade::setGradePoint(double gp)
{
    gradePoint = gp;
    letterGrade = calculateLetterGrade(gp);
}

// Display
void Grade::displayInfo() const
{
    std::cout << "Grade: " << letterGrade
              << " (GP: " << std::fixed << std::setprecision(2) << gradePoint << ")"
              << " | Total: " << totalMarks
              << " (Mid: " << midSemMarks << ", End: " << endSemMarks
              << ", Asgn: " << assignmentMarks << ")" << std::endl;
}

// Comparison operators
bool Grade::operator>=(double minGrade) const
{
    return gradePoint >= minGrade;
}

bool Grade::operator<(const Grade &other) const
{
    return gradePoint < other.gradePoint;
}

bool Grade::operator>(const Grade &other) const
{
    return gradePoint > other.gradePoint;
}

bool Grade::operator==(const Grade &other) const
{
    return std::abs(gradePoint - other.gradePoint) < 0.01;
}

// Static helper for grade calculation
std::string Grade::calculateLetterGrade(double gradePoint)
{
    if (gradePoint >= 10.0)
        return "A+";
    else if (gradePoint >= 9.0)
        return "A";
    else if (gradePoint >= 8.0)
        return "A-";
    else if (gradePoint >= 7.0)
        return "B+";
    else if (gradePoint >= 6.0)
        return "B";
    else if (gradePoint >= 5.0)
        return "B-";
    else if (gradePoint >= 4.0)
        return "C";
    else if (gradePoint >= 3.0)
        return "D";
    else
        return "F";
}
