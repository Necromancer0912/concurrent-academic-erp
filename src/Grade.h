#ifndef GRADE_H
#define GRADE_H

#include <string>
#include <iostream>

class Grade
{
private:
    double __mid_sem_marks;
    double __end_sem_marks;
    double __assignment_marks;
    double __total_marks;
    double __grade_point;       // 0-10 scale
    std::string __letter_grade; // A, A-, B+, etc.

    // Calculate grade from marks
    void __calculate_grade();

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
    double get_mid_sem_marks() const;
    double get_end_sem_marks() const;
    double get_assignment_marks() const;
    double get_total_marks() const;
    double get_grade_point() const;
    std::string get_letter_grade() const;

    // Setters
    void set_marks(double mid, double end, double assignment);
    void set_grade_point(double gp);

    // Display
    void display_info() const;

    // Comparison operators
    bool operator>=(double minGrade) const;
    bool operator<(const Grade &other) const;
    bool operator>(const Grade &other) const;
    bool operator==(const Grade &other) const;

    // Static helper for grade calculation
    static std::string calculate_letter_grade(double grade_point);
};

#endif // GRADE_H
