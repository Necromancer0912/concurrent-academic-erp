#include "Branch.h"

// Constructor
Branch::Branch(const std::string &code, const std::string &name, const std::string &dept)
    : branchCode(code), branchName(name), department(dept) {}

// Default constructor
Branch::Branch() : branchCode(""), branchName(""), department("") {}

// Copy constructor
Branch::Branch(const Branch &other)
    : branchCode(other.branchCode), branchName(other.branchName), department(other.department) {}

// Assignment operator
Branch &Branch::operator=(const Branch &other)
{
    if (this != &other)
    {
        branchCode = other.branchCode;
        branchName = other.branchName;
        department = other.department;
    }
    return *this;
}

// Getters
std::string Branch::getBranchCode() const { return branchCode; }
std::string Branch::getBranchName() const { return branchName; }
std::string Branch::getDepartment() const { return department; }

// Display
void Branch::displayInfo() const
{
    std::cout << "Branch: " << branchName << " (" << branchCode << ")"
              << " | Department: " << department << std::endl;
}

// Comparison operators
bool Branch::operator==(const Branch &other) const
{
    return branchCode == other.branchCode;
}

bool Branch::operator!=(const Branch &other) const
{
    return !(*this == other);
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
