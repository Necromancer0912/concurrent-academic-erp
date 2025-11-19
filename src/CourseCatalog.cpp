#include "CourseCatalog.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

const std::string CourseCatalog::CATALOG_FILE = "data/course_catalog.dat";
const std::string CourseCatalog::SYLLABUS_DIR = "data/syllabus/";

CourseCatalog::CourseCatalog()
{
    system("mkdir -p data/syllabus");
    loadCatalog();
}

void CourseCatalog::loadCatalog()
{
    std::ifstream file(CATALOG_FILE);

    if (!file.is_open())
    {
        initializeDefaultCourses();
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line == "COURSE_START")
        {
            CourseInfo course;

            while (std::getline(file, line) && line != "COURSE_END")
            {
                if (line.find("Code: ") == 0)
                {
                    course.courseCode = line.substr(6);
                }
                else if (line.find("Name: ") == 0)
                {
                    course.courseName = line.substr(6);
                }
                else if (line.find("Credits: ") == 0)
                {
                    course.credits = std::stoi(line.substr(9));
                }
                else if (line.find("Instructor: ") == 0)
                {
                    course.instructor = line.substr(12);
                }
                else if (line.find("Duration: ") == 0)
                {
                    course.duration = std::stoi(line.substr(10));
                }
                else if (line.find("Type: ") == 0)
                {
                    course.courseType = line.substr(6);
                }
                else if (line.find("Levels: ") == 0)
                {
                    std::string levels = line.substr(8);
                    std::istringstream iss(levels);
                    std::string level;
                    while (iss >> level)
                    {
                        course.eligibleLevels.insert(level);
                    }
                }
                else if (line.find("Prerequisites: ") == 0)
                {
                    std::string prereqs = line.substr(15);
                    std::istringstream iss(prereqs);
                    std::string prereq;
                    while (iss >> prereq)
                    {
                        course.prerequisites.push_back(prereq);
                    }
                }
                else if (line.find("Syllabus: ") == 0)
                {
                    course.syllabus = line.substr(10);
                }
            }

            courses[course.courseCode] = course;
        }
    }

    file.close();
}

void CourseCatalog::saveCatalog()
{
    std::ofstream file(CATALOG_FILE);

    if (!file.is_open())
    {
        throw std::runtime_error("Unable to save course catalog");
    }

    file << "# Course Catalog Database\n";
    file << "# Total Courses: " << courses.size() << "\n";
    file << "---\n";

    for (const auto &pair : courses)
    {
        const CourseInfo &course = pair.second;

        file << "COURSE_START\n";
        file << "Code: " << course.courseCode << "\n";
        file << "Name: " << course.courseName << "\n";
        file << "Credits: " << course.credits << "\n";
        file << "Instructor: " << course.instructor << "\n";
        file << "Duration: " << course.duration << "\n";
        file << "Type: " << course.courseType << "\n";

        file << "Levels: ";
        for (const auto &level : course.eligibleLevels)
        {
            file << level << " ";
        }
        file << "\n";

        file << "Prerequisites: ";
        for (const auto &prereq : course.prerequisites)
        {
            file << prereq << " ";
        }
        file << "\n";

        file << "Syllabus: " << course.syllabus << "\n";
        file << "COURSE_END\n";
    }

    file.close();
}

void CourseCatalog::addCourse(const CourseInfo &course)
{
    courses[course.courseCode] = course;
    saveCatalog();
}

CourseInfo CourseCatalog::getCourseInfo(const std::string &courseCode) const
{
    auto it = courses.find(courseCode);
    if (it != courses.end())
    {
        return it->second;
    }
    return CourseInfo();
}

bool CourseCatalog::courseExists(const std::string &courseCode) const
{
    return courses.find(courseCode) != courses.end();
}

std::vector<CourseInfo> CourseCatalog::getCoursesByLevel(const std::string &level) const
{
    std::vector<CourseInfo> result;

    for (const auto &pair : courses)
    {
        if (pair.second.eligibleLevels.count(level) > 0)
        {
            result.push_back(pair.second);
        }
    }

    return result;
}

std::vector<CourseInfo> CourseCatalog::getAllCourses() const
{
    std::vector<CourseInfo> result;

    for (const auto &pair : courses)
    {
        result.push_back(pair.second);
    }

    return result;
}

std::string CourseCatalog::getSyllabus(const std::string &courseCode) const
{
    auto it = courses.find(courseCode);
    if (it == courses.end())
    {
        return "Course not found";
    }

    std::string syllabusFile = SYLLABUS_DIR + courseCode + ".txt";
    std::ifstream file(syllabusFile);

    if (!file.is_open())
    {
        return it->second.syllabus.empty() ? "Syllabus not available" : it->second.syllabus;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

bool CourseCatalog::updateCourse(const CourseInfo &course)
{
    if (courses.find(course.courseCode) == courses.end())
    {
        return false;
    }

    courses[course.courseCode] = course;
    saveCatalog();
    return true;
}

bool CourseCatalog::deleteCourse(const std::string &courseCode)
{
    if (courses.erase(courseCode) > 0)
    {
        saveCatalog();
        return true;
    }
    return false;
}

std::vector<CourseInfo> CourseCatalog::searchCourses(const std::string &query) const
{
    std::vector<CourseInfo> result;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    for (const auto &pair : courses)
    {
        std::string lowerName = pair.second.courseName;
        std::string lowerCode = pair.second.courseCode;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        std::transform(lowerCode.begin(), lowerCode.end(), lowerCode.begin(), ::tolower);

        if (lowerName.find(lowerQuery) != std::string::npos ||
            lowerCode.find(lowerQuery) != std::string::npos)
        {
            result.push_back(pair.second);
        }
    }

    return result;
}

bool CourseCatalog::canTakeCourse(const std::string &courseCode, const std::string &studentLevel) const
{
    auto it = courses.find(courseCode);
    if (it == courses.end())
    {
        return false;
    }

    return it->second.eligibleLevels.count(studentLevel) > 0;
}

int CourseCatalog::getCourseCredits(const std::string &courseCode) const
{
    auto it = courses.find(courseCode);
    if (it != courses.end())
    {
        return it->second.credits;
    }
    return 0;
}

std::string CourseCatalog::getCourseType(const std::string &courseCode) const
{
    auto it = courses.find(courseCode);
    if (it != courses.end())
    {
        return it->second.courseType;
    }
    return "Unknown";
}

void CourseCatalog::initializeDefaultCourses()
{
    // BTech Core Courses
    CourseInfo dsa;
    dsa.courseCode = "CSE102";
    dsa.courseName = "Data Structures and Algorithms";
    dsa.credits = 4;
    dsa.instructor = "Dr. Rajiv Ratn Shah";
    dsa.duration = 1;
    dsa.courseType = "Regular";
    dsa.eligibleLevels = {"BTECH"};
    dsa.syllabus = "Arrays, Linked Lists, Stacks, Queues, Trees, Graphs, Sorting, Searching, Dynamic Programming";
    addCourse(dsa);

    // BTech/MTech Core Courses
    CourseInfo oopd;
    oopd.courseCode = "CSE201";
    oopd.courseName = "Object-Oriented Programming and Design";
    oopd.credits = 4;
    oopd.instructor = "Dr. Arani Bhattacharya";
    oopd.duration = 1;
    oopd.courseType = "Regular";
    oopd.eligibleLevels = {"BTECH", "MTECH"};
    oopd.syllabus = "Classes, Objects, Inheritance, Polymorphism, Templates, Exception Handling, Design Patterns";
    addCourse(oopd);

    CourseInfo dbms;
    dbms.courseCode = "CSE202";
    dbms.courseName = "Database Management Systems";
    dbms.credits = 4;
    dbms.instructor = "Dr. Tanmoy Chakraborty";
    dbms.duration = 1;
    dbms.courseType = "Regular";
    dbms.eligibleLevels = {"BTECH", "MTECH"};
    dbms.prerequisites = {"CSE102"};
    dbms.syllabus = "SQL, Relational Algebra, Normalization, Transactions, Concurrency Control, NoSQL";
    addCourse(dbms);

    CourseInfo os;
    os.courseCode = "CSE231";
    os.courseName = "Operating Systems";
    os.credits = 4;
    os.instructor = "Dr. Sambuddho Chakravarty";
    os.duration = 1;
    os.courseType = "Regular";
    os.eligibleLevels = {"BTECH", "MTECH"};
    os.prerequisites = {"CSE102"};
    os.syllabus = "Processes, Threads, Scheduling, Memory Management, File Systems, I/O Systems";
    addCourse(os);

    CourseInfo cn;
    cn.courseCode = "CSE232";
    cn.courseName = "Computer Networks";
    cn.credits = 4;
    cn.instructor = "Dr. Mukulika Maity";
    cn.duration = 1;
    cn.courseType = "Regular";
    cn.eligibleLevels = {"BTECH", "MTECH"};
    cn.syllabus = "TCP/IP Stack, Routing Protocols, Network Security, Wireless Networks, SDN";
    addCourse(cn);

    // Advanced Courses (All Levels)
    CourseInfo ml;
    ml.courseCode = "CSE343";
    ml.courseName = "Machine Learning";
    ml.credits = 4;
    ml.instructor = "Dr. Ganesh Bagler";
    ml.duration = 1;
    ml.courseType = "Regular";
    ml.eligibleLevels = {"BTECH", "MTECH", "PHD"};
    ml.prerequisites = {"CSE102"};
    ml.syllabus = "Supervised Learning, Unsupervised Learning, Neural Networks, Deep Learning, Reinforcement Learning";
    addCourse(ml);

    CourseInfo ai;
    ai.courseCode = "CSE643";
    ai.courseName = "Artificial Intelligence";
    ai.credits = 4;
    ai.instructor = "Dr. Koteswar Rao Jerripothula";
    ai.duration = 1;
    ai.courseType = "Regular";
    ai.eligibleLevels = {"BTECH", "MTECH", "PHD"};
    ai.syllabus = "Search Algorithms, Knowledge Representation, Planning, Natural Language Processing, Computer Vision";
    addCourse(ai);

    // MTech Specialized Courses
    CourseInfo advAlgo;
    advAlgo.courseCode = "CSE501";
    advAlgo.courseName = "Advanced Algorithms";
    advAlgo.credits = 4;
    advAlgo.instructor = "Dr. Sanjit Kaul";
    advAlgo.duration = 1;
    advAlgo.courseType = "Regular";
    advAlgo.eligibleLevels = {"MTECH", "PHD"};
    advAlgo.syllabus = "Advanced Data Structures, Graph Algorithms, Approximation Algorithms, Randomized Algorithms";
    addCourse(advAlgo);

    CourseInfo distSys;
    distSys.courseCode = "CSE502";
    distSys.courseName = "Distributed Systems";
    distSys.credits = 4;
    distSys.instructor = "Dr. Pravesh Biyani";
    distSys.duration = 1;
    distSys.courseType = "Regular";
    distSys.eligibleLevels = {"MTECH", "PHD"};
    distSys.syllabus = "Distributed Computing Models, Consensus, Fault Tolerance, Cloud Computing, Blockchain";
    addCourse(distSys);

    CourseInfo infosec;
    infosec.courseCode = "CSE503";
    infosec.courseName = "Information Security";
    infosec.credits = 4;
    infosec.instructor = "Dr. Sambuddho Chakravarty";
    infosec.duration = 1;
    infosec.courseType = "Regular";
    infosec.eligibleLevels = {"MTECH", "PHD"};
    infosec.syllabus = "Cryptography, Network Security, Web Security, Malware Analysis, Penetration Testing";
    addCourse(infosec);

    // Capstone Projects
    CourseInfo btechCapstone;
    btechCapstone.courseCode = "CSE499";
    btechCapstone.courseName = "BTech Capstone Project";
    btechCapstone.credits = 8;
    btechCapstone.instructor = "Various Faculty";
    btechCapstone.duration = 2;
    btechCapstone.courseType = "Capstone";
    btechCapstone.eligibleLevels = {"BTECH"};
    btechCapstone.syllabus = "Independent research project under faculty guidance on topics in CSE";
    addCourse(btechCapstone);

    CourseInfo mtechCapstone;
    mtechCapstone.courseCode = "CSE599";
    mtechCapstone.courseName = "MTech Thesis/Capstone";
    mtechCapstone.credits = 8;
    mtechCapstone.instructor = "Various Faculty";
    mtechCapstone.duration = 2;
    mtechCapstone.courseType = "Capstone";
    mtechCapstone.eligibleLevels = {"MTECH"};
    mtechCapstone.syllabus = "Advanced research project with industry relevance or academic contribution";
    addCourse(mtechCapstone);

    // Research/PhD Courses
    CourseInfo research;
    research.courseCode = "RES001";
    research.courseName = "Research Methodology & Publication";
    research.credits = 16;
    research.instructor = "PhD Advisor";
    research.duration = 2;
    research.courseType = "Research";
    research.eligibleLevels = {"PHD"};
    research.syllabus = "Original research leading to publication in peer-reviewed conferences/journals";
    addCourse(research);

    CourseInfo phdSeminar;
    phdSeminar.courseCode = "RES002";
    phdSeminar.courseName = "PhD Seminar & Thesis";
    phdSeminar.credits = 16;
    phdSeminar.instructor = "PhD Committee";
    phdSeminar.duration = 2;
    phdSeminar.courseType = "Research";
    phdSeminar.eligibleLevels = {"PHD"};
    phdSeminar.syllabus = "PhD dissertation work, literature survey, thesis writing, defense preparation";
    addCourse(phdSeminar);

    // ====================================================================
    // IIT-Delhi Courses (Integer-based Course Codes)
    // IIIT-Delhi students can take these courses as per cross-registration
    // ====================================================================

    CourseInfo iit101;
    iit101.courseCode = "101";
    iit101.courseName = "Introduction to Computing";
    iit101.credits = 4;
    iit101.instructor = "Prof. Subhashis Banerjee (IIT-Delhi)";
    iit101.duration = 1;
    iit101.courseType = "Regular";
    iit101.eligibleLevels = {"BTECH", "MTECH"};
    iit101.syllabus = "Programming Fundamentals, Data Types, Control Structures, Functions, Arrays";
    addCourse(iit101);

    CourseInfo iit201;
    iit201.courseCode = "201";
    iit201.courseName = "Advanced Algorithms";
    iit201.credits = 4;
    iit201.instructor = "Prof. Mausam (IIT-Delhi)";
    iit201.duration = 1;
    iit201.courseType = "Regular";
    iit201.eligibleLevels = {"BTECH", "MTECH", "PHD"};
    iit201.prerequisites = {"101"};
    iit201.syllabus = "Graph Theory, Dynamic Programming, Greedy Algorithms, NP-Completeness";
    addCourse(iit201);

    CourseInfo iit202;
    iit202.courseCode = "202";
    iit202.courseName = "Computer Architecture";
    iit202.credits = 4;
    iit202.instructor = "Prof. Anshul Kumar (IIT-Delhi)";
    iit202.duration = 1;
    iit202.courseType = "Regular";
    iit202.eligibleLevels = {"BTECH", "MTECH"};
    iit202.syllabus = "Instruction Set Architecture, Pipelining, Memory Hierarchy, I/O Systems";
    addCourse(iit202);

    CourseInfo iit305;
    iit305.courseCode = "305";
    iit305.courseName = "Database Systems";
    iit305.credits = 4;
    iit305.instructor = "Prof. S. Sudarshan (IIT-Delhi)";
    iit305.duration = 1;
    iit305.courseType = "Regular";
    iit305.eligibleLevels = {"BTECH", "MTECH", "PHD"};
    iit305.prerequisites = {"101"};
    iit305.syllabus = "Relational Model, SQL, Query Optimization, Transaction Management, Distributed Databases";
    addCourse(iit305);

    CourseInfo iit401;
    iit401.courseCode = "401";
    iit401.courseName = "Artificial Intelligence and Machine Learning";
    iit401.credits = 4;
    iit401.instructor = "Prof. Parag Singla (IIT-Delhi)";
    iit401.duration = 1;
    iit401.courseType = "Regular";
    iit401.eligibleLevels = {"BTECH", "MTECH", "PHD"};
    iit401.prerequisites = {"201"};
    iit401.syllabus = "Search, Knowledge Representation, Machine Learning, Neural Networks, Deep Learning";
    addCourse(iit401);

    CourseInfo iit501;
    iit501.courseCode = "501";
    iit501.courseName = "Advanced Computer Networks";
    iit501.credits = 4;
    iit501.instructor = "Prof. Huzur Saran (IIT-Delhi)";
    iit501.duration = 1;
    iit501.courseType = "Regular";
    iit501.eligibleLevels = {"MTECH", "PHD"};
    iit501.syllabus = "Network Protocols, Routing Algorithms, QoS, Network Security, Software Defined Networking";
    addCourse(iit501);

    CourseInfo iit502;
    iit502.courseCode = "502";
    iit502.courseName = "Parallel and Distributed Computing";
    iit502.credits = 4;
    iit502.instructor = "Prof. Kolin Paul (IIT-Delhi)";
    iit502.duration = 1;
    iit502.courseType = "Regular";
    iit502.eligibleLevels = {"MTECH", "PHD"};
    iit502.syllabus = "Parallel Architectures, MPI, OpenMP, Distributed Algorithms, MapReduce, Spark";
    addCourse(iit502);

    CourseInfo iit601;
    iit601.courseCode = "601";
    iit601.courseName = "Advanced Topics in Computer Vision";
    iit601.credits = 4;
    iit601.instructor = "Prof. Chetan Arora (IIT-Delhi)";
    iit601.duration = 1;
    iit601.courseType = "Regular";
    iit601.eligibleLevels = {"MTECH", "PHD"};
    iit601.prerequisites = {"401"};
    iit601.syllabus = "Image Processing, Object Detection, Semantic Segmentation, 3D Vision, GANs";
    addCourse(iit601);

    std::cout << "Initialized course catalog with " << courses.size() << " courses.\n";
    std::cout << "  - IIIT-Delhi courses: " << (courses.size() - 8) << "\n";
    std::cout << "  - IIT-Delhi courses: 8 (cross-registration available)\n";
}

void CourseCatalog::displayCourse(const std::string &courseCode) const
{
    auto it = courses.find(courseCode);
    if (it == courses.end())
    {
        std::cout << "Course not found.\n";
        return;
    }

    const CourseInfo &course = it->second;

    std::cout << "\n"
              << std::string(60, '=') << "\n";
    std::cout << "Course Code: " << course.courseCode << "\n";
    std::cout << "Course Name: " << course.courseName << "\n";
    std::cout << "Credits: " << course.credits << "\n";
    std::cout << "Instructor: " << course.instructor << "\n";
    std::cout << "Duration: " << course.duration << " semester(s)\n";
    std::cout << "Type: " << course.courseType << "\n";

    std::cout << "Eligible Levels: ";
    for (const auto &level : course.eligibleLevels)
    {
        std::cout << level << " ";
    }
    std::cout << "\n";

    if (!course.prerequisites.empty())
    {
        std::cout << "Prerequisites: ";
        for (const auto &prereq : course.prerequisites)
        {
            std::cout << prereq << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nSyllabus:\n"
              << getSyllabus(courseCode) << "\n";
    std::cout << std::string(60, '=') << "\n";
}
