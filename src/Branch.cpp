#include "Branch.h"

// Constructor
Branch::Branch(const std::string &_code, const std::string &_name, const std::string &_dept)
    : __branch_code(_code), __branch_name(_name), __department(_dept) {}

// Default constructor
Branch::Branch() : __branch_code(""), __branch_name(""), __department("") {}

// Copy constructor
Branch::Branch(const Branch &_other)
    : __branch_code(_other.__branch_code), __branch_name(_other.__branch_name), __department(_other.__department) {}

// Assignment operator
Branch &Branch::operator=(const Branch &_other)
{
    if (this != &_other)
    {
        __branch_code = _other.__branch_code;
        __branch_name = _other.__branch_name;
        __department = _other.__department;
    }
    return *this;
}

// Getters
std::string Branch::get_branch_code() const { return __branch_code; }
std::string Branch::get_branch_name() const { return __branch_name; }
std::string Branch::get_department() const { return __department; }

// Display
void Branch::display_info() const
{
    std::cout << "Branch: " << __branch_name << " (" << __branch_code << ")"
              << " | Department: " << __department << std::endl;
}

// Comparison operators
bool Branch::operator==(const Branch &_other) const
{
    return __branch_code == _other.__branch_code;
}

bool Branch::operator!=(const Branch &_other) const
{
    return !(*this == _other);
}

// Static factory methods for common IIIT-D branches
// BTech Branches
Branch Branch::CSE()
{
    return Branch("CSE", "Computer Science and Engineering", "Engineering");
}

Branch Branch::ECE()
{
    return Branch("ECE", "Electronics and Communications Engineering", "Engineering");
}

Branch Branch::CSAM()
{
    return Branch("CSAM", "Computer Science and Applied Mathematics", "Engineering");
}

Branch Branch::CSB()
{
    return Branch("CSB", "Computer Science and Biosciences", "Engineering");
}

Branch Branch::CSSS()
{
    return Branch("CSSS", "Computer Science and Social Sciences", "Engineering");
}

Branch Branch::CSAI()
{
    return Branch("CSAI", "Computer Science & Artificial Intelligence", "Engineering");
}

Branch Branch::CSD()
{
    return Branch("CSD", "Computer Science and Design", "Engineering");
}

Branch Branch::EVE()
{
    return Branch("EVE", "Electronics & VLSI Engineering", "Engineering");
}

Branch Branch::CSEcon()
{
    return Branch("CSEcon", "Computer Science and Economics", "Engineering");
}

// MTech Branches
Branch Branch::MTCSE()
{
    return Branch("MTCSE", "M.Tech Computer Science & Engineering", "Engineering");
}

Branch Branch::MTECE()
{
    return Branch("MTECE", "M.Tech Electronics & Communications Engineering", "Engineering");
}

Branch Branch::MTCB()
{
    return Branch("MTCB", "M.Tech Computational Biology", "Biology");
}

// PhD Branches
Branch Branch::PHDCSE()
{
    return Branch("PHDCSE", "PhD Computer Science & Engineering", "Engineering");
}

Branch Branch::PHDECE()
{
    return Branch("PHDECE", "PhD Electronics & Communications Engineering", "Engineering");
}

Branch Branch::PHDCB()
{
    return Branch("PHDCB", "PhD Computational Biology", "Biology");
}

Branch Branch::PHDMATH()
{
    return Branch("PHDMATH", "PhD Mathematics", "Mathematics");
}

Branch Branch::PHDHCD()
{
    return Branch("PHDHCD", "PhD Human-Centred & Design", "Design");
}

Branch Branch::PHDSSH()
{
    return Branch("PHDSSH", "PhD Social Sciences & Humanities", "Humanities");
}
