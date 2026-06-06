#ifndef BRANCH_H
#define BRANCH_H

#include <string>
#include <iostream>

// Branch class to represent different branches/departments
class Branch
{
private:
    std::string __branch_code;
    std::string __branch_name;
    std::string __department;

public:
    // Constructor
    Branch(const std::string &code, const std::string &name, const std::string &dept);

    // Default constructor
    Branch();

    // Copy constructor
    Branch(const Branch &other);

    // Assignment operator
    Branch &operator=(const Branch &other);

    // Getters
    std::string get_branch_code() const;
    std::string get_branch_name() const;
    std::string get_department() const;

    // Display
    void display_info() const;

    // Comparison operators
    bool operator==(const Branch &other) const;
    bool operator!=(const Branch &other) const;

    // Static factory methods for common engineering branches
    // BTech Branches
    static Branch CSE();    // Computer Science and Engineering
    static Branch ECE();    // Electronics and Communications Engineering
    static Branch CSAI();   // Computer Science & Artificial Intelligence
    static Branch CSAM();   // Computer Science and Applied Mathematics
    static Branch CSD();    // Computer Science and Design
    static Branch CSB();    // Computer Science and Biosciences
    static Branch CSSS();   // Computer Science and Social Sciences
    static Branch EVE();    // Electronics & VLSI Engineering
    static Branch CSEcon(); // Computer Science and Economics

    // MTech Branches
    static Branch MTCSE(); // M.Tech CSE
    static Branch MTECE(); // M.Tech ECE
    static Branch MTCB();  // M.Tech Computational Biology

    // PhD Branches
    static Branch PHDCSE();  // PhD CSE
    static Branch PHDECE();  // PhD ECE
    static Branch PHDCB();   // PhD Computational Biology
    static Branch PHDMATH(); // PhD Mathematics
    static Branch PHDHCD();  // PhD Human-Centred Design
    static Branch PHDSSH();  // PhD Social Sciences & Humanities
};

#endif // BRANCH_H
