#include "Grade.h"
#include <iomanip>
#include <cmath>

// Constructor with marks (30% mid, 40% end, 30% assignment)
Grade::Grade(double _mid, double _end, double _assignment)
    : __mid_sem_marks(_mid), __end_sem_marks(_end), __assignment_marks(_assignment)
{
    __calculate_grade();
}

// Constructor with direct grade (back-calculate marks from grade point)
Grade::Grade(double _gp) : __grade_point(_gp)
{
    __letter_grade = calculate_letter_grade(_gp);

    // Back-calculate total marks from grade point (assuming 0-10 scale)
    // Grade point 10 = 90-100%, 9 = 80-90%, etc.
    if (_gp >= 10.0)
        __total_marks = 95.0;
    else if (_gp >= 9.0)
        __total_marks = 85.0;
    else if (_gp >= 8.0)
        __total_marks = 75.0;
    else if (_gp >= 7.0)
        __total_marks = 65.0;
    else if (_gp >= 6.0)
        __total_marks = 55.0;
    else if (_gp >= 5.0)
        __total_marks = 45.0;
    else if (_gp >= 4.0)
        __total_marks = 35.0;
    else
        __total_marks = 20.0;

    // Distribute marks proportionally (30% mid, 50% end, 20% assignment)
    __mid_sem_marks = __total_marks * 0.30 / 0.30;    // Full marks for mid portion
    __end_sem_marks = __total_marks * 0.50 / 0.50;    // Full marks for end portion
    __assignment_marks = __total_marks * 0.20 / 0.20; // Full marks for assignment portion
}

// Default constructor
Grade::Grade() : __mid_sem_marks(0), __end_sem_marks(0), __assignment_marks(0),
                 __total_marks(0), __grade_point(0), __letter_grade("F") {}

// Copy constructor
Grade::Grade(const Grade &_other)
    : __mid_sem_marks(_other.__mid_sem_marks), __end_sem_marks(_other.__end_sem_marks),
      __assignment_marks(_other.__assignment_marks), __total_marks(_other.__total_marks),
      __grade_point(_other.__grade_point), __letter_grade(_other.__letter_grade) {}

// Assignment operator
Grade &Grade::operator=(const Grade &_other)
{
    if (this != &_other)
    {
        __mid_sem_marks = _other.__mid_sem_marks;
        __end_sem_marks = _other.__end_sem_marks;
        __assignment_marks = _other.__assignment_marks;
        __total_marks = _other.__total_marks;
        __grade_point = _other.__grade_point;
        __letter_grade = _other.__letter_grade;
    }
    return *this;
}

// Calculate grade from marks
void Grade::__calculate_grade()
{
    // Weighted calculation: 30% mid, 40% end, 30% assignment
    __total_marks = (__mid_sem_marks * 0.30) + (__end_sem_marks * 0.40) + (__assignment_marks * 0.30);

    // Convert to 10-point scale
    // Assuming marks are out of 100
    if (__total_marks >= 90)
        __grade_point = 10.0;
    else if (__total_marks >= 80)
        __grade_point = 9.0;
    else if (__total_marks >= 70)
        __grade_point = 8.0;
    else if (__total_marks >= 60)
        __grade_point = 7.0;
    else if (__total_marks >= 50)
        __grade_point = 6.0;
    else if (__total_marks >= 40)
        __grade_point = 5.0;
    else if (__total_marks >= 30)
        __grade_point = 4.0;
    else
        __grade_point = 0.0;

    __letter_grade = calculate_letter_grade(__grade_point);
}

// Getters
double Grade::get_mid_sem_marks() const { return __mid_sem_marks; }
double Grade::get_end_sem_marks() const { return __end_sem_marks; }
double Grade::get_assignment_marks() const { return __assignment_marks; }
double Grade::get_total_marks() const { return __total_marks; }
double Grade::get_grade_point() const { return __grade_point; }
std::string Grade::get_letter_grade() const { return __letter_grade; }

// Setters
void Grade::set_marks(double _mid, double _end, double _assignment)
{
    __mid_sem_marks = _mid;
    __end_sem_marks = _end;
    __assignment_marks = _assignment;
    __calculate_grade();
}

void Grade::set_grade_point(double _gp)
{
    __grade_point = _gp;
    __letter_grade = calculate_letter_grade(_gp);
}

// Display
void Grade::display_info() const
{
    std::cout << "Grade: " << __letter_grade
              << " (GP: " << std::fixed << std::setprecision(2) << __grade_point << ")"
              << " | Total: " << __total_marks
              << " (Mid: " << __mid_sem_marks << ", End: " << __end_sem_marks
              << ", Asgn: " << __assignment_marks << ")" << std::endl;
}

// Comparison operators
bool Grade::operator>=(double _min_grade) const
{
    return __grade_point >= _min_grade;
}

bool Grade::operator<(const Grade &_other) const
{
    return __grade_point < _other.__grade_point;
}

bool Grade::operator>(const Grade &_other) const
{
    return __grade_point > _other.__grade_point;
}

bool Grade::operator==(const Grade &_other) const
{
    return std::abs(__grade_point - _other.__grade_point) < 0.01;
}

// Static helper for grade calculation
std::string Grade::calculate_letter_grade(double _grade_point)
{
    if (_grade_point >= 10.0)
        return "A+";
    else if (_grade_point >= 9.0)
        return "A";
    else if (_grade_point >= 8.0)
        return "A-";
    else if (_grade_point >= 7.0)
        return "B+";
    else if (_grade_point >= 6.0)
        return "B";
    else if (_grade_point >= 5.0)
        return "B-";
    else if (_grade_point >= 4.0)
        return "C";
    else if (_grade_point >= 3.0)
        return "D";
    else
        return "F";
}
