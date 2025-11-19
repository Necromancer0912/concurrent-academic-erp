#include "CSVHandler.h"
#include <random>
#include <ctime>
#include <algorithm>

// Constructor
template <typename RollType, typename CourseCodeType>
CSVHandler<RollType, CourseCodeType>::CSVHandler(const std::string &fname)
    : __filename(fname) {}

// Parse a CSV line
template <typename RollType, typename CourseCodeType>
std::vector<std::string> CSVHandler<RollType, CourseCodeType>::parse_line(const std::string &line)
{
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string cell;

    while (std::getline(ss, cell, ','))
    {
        // Trim whitespace
        cell.erase(0, cell.find_first_not_of(" \t\r\n"));
        cell.erase(cell.find_last_not_of(" \t\r\n") + 1);
        result.push_back(cell);
    }

    return result;
}

// Convert string to appropriate roll number type
template <typename RollType, typename CourseCodeType>
RollType CSVHandler<RollType, CourseCodeType>::parseRollNumber(const std::string &rollStr)
{
    if constexpr (std::is_same<RollType, std::string>::value)
    {
        return rollStr;
    }
    else
    {
        return std::stoul(rollStr);
    }
}

// Convert string to appropriate course code type
template <typename RollType, typename CourseCodeType>
CourseCodeType CSVHandler<RollType, CourseCodeType>::parseCourseCode(const std::string &codeStr)
{
    if constexpr (std::is_same<CourseCodeType, std::string>::value)
    {
        return codeStr;
    }
    else
    {
        return std::stoi(codeStr);
    }
}

// Parse student level
template <typename RollType, typename CourseCodeType>
StudentLevel CSVHandler<RollType, CourseCodeType>::parseLevel(const std::string &levelStr)
{
    if (levelStr == "BTECH" || levelStr == "BTech" || levelStr == "B.Tech")
    {
        return StudentLevel::BTECH;
    }
    else if (levelStr == "MTECH" || levelStr == "MTech" || levelStr == "M.Tech")
    {
        return StudentLevel::MTECH;
    }
    else if (levelStr == "PHD" || levelStr == "PhD" || levelStr == "Ph.D.")
    {
        return StudentLevel::PHD;
    }
    else if (levelStr == "DUAL" || levelStr == "Dual")
    {
        return StudentLevel::DUAL_DEGREE;
    }
    return StudentLevel::BTECH; // Default
}

// Parse branch
template <typename RollType, typename CourseCodeType>
Branch CSVHandler<RollType, CourseCodeType>::parseBranch(const std::string &branchCode)
{
    if (branchCode == "CSE")
        return Branch::CSE();
    else if (branchCode == "ECE")
        return Branch::ECE();
    else if (branchCode == "CSAM")
        return Branch::CSAM();
    else if (branchCode == "CSB")
        return Branch::CSB();
    else if (branchCode == "CSSS")
        return Branch::CSSS();
    else if (branchCode == "CSAI")
        return Branch::CSAI();
    else if (branchCode == "CSD")
        return Branch::CSD();
    else if (branchCode == "EVE")
        return Branch::EVE();
    else if (branchCode == "CSEcon")
        return Branch::CSEcon();
    // MTech branches
    else if (branchCode == "MTCSE")
        return Branch::MTCSE();
    else if (branchCode == "MTECE")
        return Branch::MTECE();
    else if (branchCode == "MTCB")
        return Branch::MTCB();
    // PhD branches
    else if (branchCode == "PHDCSE")
        return Branch::PHDCSE();
    else if (branchCode == "PHDECE")
        return Branch::PHDECE();
    else if (branchCode == "PHDCB")
        return Branch::PHDCB();
    else if (branchCode == "PHDMATH")
        return Branch::PHDMATH();
    else if (branchCode == "PHDHCD")
        return Branch::PHDHCD();
    else if (branchCode == "PHDSSH")
        return Branch::PHDSSH();
    else
        return Branch::CSE(); // Default
}

// Read students from CSV
template <typename RollType, typename CourseCodeType>
std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
CSVHandler<RollType, CourseCodeType>::read_students()
{
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> students;

    std::ifstream file(__filename);
    if (!file.is_open())
    {
        throw CSVException("Unable to open file: " + __filename);
    }

    std::string line;
    int lineNumber = 0;

    // Read header line and identify optional columns
    std::vector<std::string> headerTokens;
    if (std::getline(file, line))
    {
        lineNumber++;
        headerTokens = parse_line(line);
    }

    // Find cgpa column index if present (case-insensitive)
    int cgpaIndex = -1;
    for (size_t i = 0; i < headerTokens.size(); ++i)
    {
        std::string t = headerTokens[i];
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        if (t == "cgpa")
        {
            cgpaIndex = (int)i;
            break;
        }
    }

    while (std::getline(file, line))
    {
        lineNumber++;

        if (line.empty() || line[0] == '#')
        {
            continue; // Skip empty lines and comments
        }

        try
        {
            auto fields = parse_line(line);

            // Expected format: Name,RollNumber,BranchCode,Level,StartYear[,cgpa],CourseCode1:Grade1,...
            if (fields.size() < 5)
            {
                throw CSVException("Invalid number of fields at line " + std::to_string(lineNumber));
            }

            std::string name = fields[0];
            RollType rollNumber = parseRollNumber(fields[1]);
            Branch branch = parseBranch(fields[2]);
            StudentLevel level = parseLevel(fields[3]);
            int startYear = std::stoi(fields[4]);

            // Parse optional CGPA
            double loadedCgpa = 0.0;
            if (cgpaIndex != -1 && cgpaIndex < (int)fields.size())
            {
                if (!fields[cgpaIndex].empty())
                {
                    try
                    {
                        loadedCgpa = std::stod(fields[cgpaIndex]);
                    }
                    catch (...)
                    {
                        loadedCgpa = 0.0;
                    }
                }
            }

            // Create student
            auto student = std::make_shared<Student<RollType, CourseCodeType>>(
                name, rollNumber, branch, level, startYear);

            // Set loaded CGPA
            student->set_loaded_cgpa(loadedCgpa);

            // Parse courses and grades (if present)
            int coursesStart = 5;
            if (cgpaIndex != -1 && cgpaIndex >= 5)
            {
                coursesStart = cgpaIndex + 1;
            }
            for (size_t i = coursesStart; i < fields.size(); ++i)
            {
                if (fields[i].empty())
                    continue;

                size_t colonPos = fields[i].find(':');
                if (colonPos != std::string::npos)
                {
                    std::string courseCodeStr = fields[i].substr(0, colonPos);
                    std::string gradeStr = fields[i].substr(colonPos + 1);

                    CourseCodeType courseCode = parseCourseCode(courseCodeStr);
                    double gradePoint = std::stod(gradeStr);

                    // Create course and grade
                    Course<CourseCodeType> course(courseCode, "Course " + courseCodeStr,
                                                  "Instructor", 4, "Previous");
                    Grade grade(gradePoint);

                    // Add to previous courses
                    student->add_current_course(course, grade);
                    student->move_to_previous_courses(courseCode);
                }
            }

            students.push_back(student);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing line " << lineNumber << ": " << e.what() << std::endl;
            // Continue with next line
        }
    }

    file.close();
    return students;
}

// Write students to CSV
template <typename RollType, typename CourseCodeType>
void CSVHandler<RollType, CourseCodeType>::write_students(
    const std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> &students)
{

    std::ofstream file(__filename);
    if (!file.is_open())
    {
        throw CSVException("Unable to create file: " + __filename);
    }

    // Write header
    file << "Name,RollNumber,BranchCode,Level,StartYear,Courses:Grades" << std::endl;

    // Write student data
    for (const auto &student : students)
    {
        file << student->get_name() << ",";

        if constexpr (std::is_same<RollType, std::string>::value)
        {
            file << student->get_roll_number();
        }
        else
        {
            file << student->get_roll_number();
        }

        file << "," << student->get_branch().get_branch_code()
             << "," << Student<RollType, CourseCodeType>::level_to_string(student->get_level())
             << "," << student->get_starting_year();

        // Write previous courses with grades
        auto prevCourses = student->get_previous_courses();
        auto prevGrades = student->get_previous_grades();

        for (size_t i = 0; i < prevCourses.size(); ++i)
        {
            file << "," << prevCourses[i].course_code_to_string()
                 << ":" << prevGrades[i].get_grade_point();
        }

        file << std::endl;
    }

    file.close();
}

// Read a range of students from CSV (start = 0-based index after header)
template <typename RollType, typename CourseCodeType>
std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>>
CSVHandler<RollType, CourseCodeType>::read_students_range(int start, int count)
{
    std::vector<std::shared_ptr<Student<RollType, CourseCodeType>>> students;

    std::ifstream file(__filename);
    if (!file.is_open())
    {
        throw CSVException("Unable to open file: " + __filename);
    }

    std::string line;

    // Read header
    std::vector<std::string> headerTokens;
    if (std::getline(file, line))
    {
        headerTokens = parse_line(line);
    }

    int cgpaIndex = -1;
    for (size_t i = 0; i < headerTokens.size(); ++i)
    {
        std::string t = headerTokens[i];
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        if (t == "cgpa")
        {
            cgpaIndex = (int)i;
            break;
        }
    }

    int targetStart = start; // 0-based after header
    int readCount = 0;
    int currentIndex = 0;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        if (currentIndex < targetStart)
        {
            currentIndex++;
            continue;
        }

        if (readCount >= count)
            break;

        try
        {
            auto fields = parse_line(line);

            if (fields.size() < 5)
            {
                currentIndex++;
                continue;
            }

            std::string name = fields[0];
            RollType rollNumber = parseRollNumber(fields[1]);
            Branch branch = parseBranch(fields[2]);
            StudentLevel level = parseLevel(fields[3]);
            int startYear = std::stoi(fields[4]);

            double loadedCgpa = 0.0;
            if (cgpaIndex != -1 && cgpaIndex < (int)fields.size())
            {
                if (!fields[cgpaIndex].empty())
                {
                    try
                    {
                        loadedCgpa = std::stod(fields[cgpaIndex]);
                    }
                    catch (...)
                    {
                        loadedCgpa = 0.0;
                    }
                }
            }

            auto student = std::make_shared<Student<RollType, CourseCodeType>>(name, rollNumber, branch, level, startYear);
            student->set_loaded_cgpa(loadedCgpa);

            // Parse courses from remaining fields (starting after known indices)
            // Determine courses start index
            int coursesStart = 5;
            if (cgpaIndex != -1 && cgpaIndex >= 5)
            {
                coursesStart = cgpaIndex + 1;
            }

            for (size_t i = coursesStart; i < fields.size(); ++i)
            {
                if (fields[i].empty())
                    continue;

                size_t colonPos = fields[i].find(':');
                if (colonPos != std::string::npos)
                {
                    std::string courseCodeStr = fields[i].substr(0, colonPos);
                    std::string gradeStr = fields[i].substr(colonPos + 1);

                    CourseCodeType courseCode = parseCourseCode(courseCodeStr);
                    double gradePoint = std::stod(gradeStr);

                    Course<CourseCodeType> course(courseCode, "Course " + courseCodeStr,
                                                  "Instructor", 4, "Previous");
                    Grade grade(gradePoint);
                    student->add_current_course(course, grade);
                    student->move_to_previous_courses(courseCode);
                }
            }

            students.push_back(student);
            readCount++;
            currentIndex++;
        }
        catch (const std::exception &)
        {
            currentIndex++;
            continue;
        }
    }

    file.close();
    return students;
}

// Generate sample CSV file
template <typename RollType, typename CourseCodeType>
void CSVHandler<RollType, CourseCodeType>::generate_sample_csv(const std::string &__filename, int numRecords)
{
    std::ofstream file(__filename);
    if (!file.is_open())
    {
        throw CSVException("Unable to create file: " + __filename);
    }

    // Write header
    file << "Name,RollNumber,BranchCode,Level,StartYear,Courses:Grades" << std::endl;

    // Random generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> yearDist(2020, 2025);
    std::uniform_int_distribution<> branchDist(0, 8);
    std::uniform_int_distribution<> levelDist(0, 3);
    std::uniform_real_distribution<> gradeDist(4.0, 10.0);
    std::uniform_int_distribution<> numCoursesDist(3, 8);

    std::vector<std::string> firstNames = {
        "Aarav", "Vivaan", "Aditya", "Arjun", "Sai", "Arnav", "Ayaan", "Krishna", "Ishaan", "Shaurya",
        "Atharva", "Advik", "Pranav", "Reyansh", "Aarush", "Vihaan", "Rohan", "Advait", "Kabir", "Dhruv",
        "Ananya", "Aadhya", "Anika", "Diya", "Navya", "Ira", "Saanvi", "Sara", "Pari", "Kiara",
        "Aarya", "Myra", "Anvi", "Aanya", "Shanaya", "Avni", "Riya", "Mira", "Siya", "Prisha"};

    std::vector<std::string> lastNames = {
        "Sharma", "Verma", "Kumar", "Singh", "Patel", "Gupta", "Reddy", "Jain", "Rao", "Nair",
        "Agarwal", "Mehta", "Malhotra", "Khanna", "Kapoor", "Chopra", "Bose", "Das", "Roy", "Sen",
        "Iyer", "Krishnan", "Menon", "Pillai", "Desai", "Shah", "Gandhi", "Trivedi", "Kulkarni", "Joshi"};

    std::vector<std::string> branches = {"CSE", "ECE", "CSAM", "CSB", "CSSS", "CSAI", "CSD", "EVE", "CSEcon"};
    std::vector<std::string> levels = {"BTECH", "MTECH", "PHD", "DUAL"};
    std::vector<std::string> courses = {"CSE101", "CSE201", "MAT101", "ECE101", "PHY101", "HSS101",
                                        "CSE301", "CSE401", "MAT201", "ECE201"};

    for (int i = 0; i < numRecords; ++i)
    {
        // Generate name
        std::string firstName = firstNames[gen() % firstNames.size()];
        std::string lastName = lastNames[gen() % lastNames.size()];
        std::string name = firstName + " " + lastName;

        // Generate roll number
        int year = yearDist(gen);
        std::string rollNumber;
        if constexpr (std::is_same<RollType, std::string>::value)
        {
            rollNumber = std::to_string(year) + std::to_string(1000 + i);
        }
        else
        {
            rollNumber = std::to_string(year) + std::to_string(1000 + i);
        }

        // Select branch and level
        std::string branch = branches[branchDist(gen)];
        std::string level = levels[levelDist(gen)];

        // Write basic info
        file << name << "," << rollNumber << "," << branch << "," << level << "," << year;

        // Add some courses with grades
        int numCourses = numCoursesDist(gen);
        for (int j = 0; j < numCourses && j < (int)courses.size(); ++j)
        {
            double grade = gradeDist(gen);
            file << "," << courses[j] << ":" << std::fixed << std::setprecision(1) << grade;
        }

        file << std::endl;
    }

    file.close();
}

// Explicit template instantiations
template class CSVHandler<std::string, std::string>;
template class CSVHandler<std::string, int>;
template class CSVHandler<unsigned int, std::string>;
template class CSVHandler<unsigned int, int>;
