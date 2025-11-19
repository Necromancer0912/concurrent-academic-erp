#include "Database.h"
#include <iostream>
#include <ctime>
#include <cstdio>
#include <sys/stat.h>

const std::string Database::STUDENT_DB_FILE = "data/__students.db";
const std::string Database::INSERTION_ORDER_FILE = "data/insertion_order.db";
const std::string Database::STUDENT_DB_BACKUP_1 = "data/__students.db.backup1";
const std::string Database::STUDENT_DB_BACKUP_2 = "data/__students.db.backup2";

void Database::createBackup()
{
    std::ifstream src(STUDENT_DB_FILE, std::ios::binary);
    if (!src.good())
    {
        return; // No existing file to backup
    }

    rotateBackups();

    std::ofstream dst(STUDENT_DB_BACKUP_1, std::ios::binary);
    dst << src.rdbuf();

    src.close();
    dst.close();
}

void Database::rotateBackups()
{
    // Move backup_1 to backup_2
    std::ifstream backup1(STUDENT_DB_BACKUP_1, std::ios::binary);
    if (backup1.good())
    {
        std::ofstream backup2(STUDENT_DB_BACKUP_2, std::ios::binary);
        backup2 << backup1.rdbuf();
        backup1.close();
        backup2.close();
    }
}

std::string Database::levelToString(StudentLevel level)
{
    switch (level)
    {
    case StudentLevel::BTECH:
        return "BTECH";
    case StudentLevel::MTECH:
        return "MTECH";
    case StudentLevel::PHD:
        return "PHD";
    case StudentLevel::DUAL_DEGREE:
        return "DUAL_DEGREE";
    default:
        return "UNKNOWN";
    }
}

StudentLevel Database::stringToLevel(const std::string &str)
{
    if (str == "BTECH")
        return StudentLevel::BTECH;
    if (str == "MTECH")
        return StudentLevel::MTECH;
    if (str == "PHD")
        return StudentLevel::PHD;
    if (str == "DUAL_DEGREE")
        return StudentLevel::DUAL_DEGREE;
    return StudentLevel::BTECH; // Default
}

bool Database::restoreFromBackup(int backupNumber)
{
    std::string backupFile;

    if (backupNumber == 1)
    {
        backupFile = STUDENT_DB_BACKUP_1;
    }
    else if (backupNumber == 2)
    {
        backupFile = STUDENT_DB_BACKUP_2;
    }
    else
    {
        return false;
    }

    std::ifstream src(backupFile, std::ios::binary);
    if (!src.good())
    {
        return false;
    }

    std::ofstream dst(STUDENT_DB_FILE, std::ios::binary);
    dst << src.rdbuf();

    src.close();
    dst.close();

    return true;
}

std::vector<std::string> Database::getBackupInfo()
{
    std::vector<std::string> info;

    struct stat fileInfo;

    // Check backup 1
    if (stat(STUDENT_DB_BACKUP_1.c_str(), &fileInfo) == 0)
    {
        char timeStr[100];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S",
                 localtime(&fileInfo.st_mtime));
        info.push_back("Backup 1: " + std::string(timeStr));
    }
    else
    {
        info.push_back("Backup 1: Not available");
    }

    // Check backup 2
    if (stat(STUDENT_DB_BACKUP_2.c_str(), &fileInfo) == 0)
    {
        char timeStr[100];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S",
                 localtime(&fileInfo.st_mtime));
        info.push_back("Backup 2: " + std::string(timeStr));
    }
    else
    {
        info.push_back("Backup 2: Not available");
    }

    return info;
}

void Database::initializeDatabaseFiles()
{
    // Create data directory if it doesn't exist
    system("mkdir -p data");

    // Create empty database file if it doesn't exist
    std::ifstream testFile(STUDENT_DB_FILE);
    if (!testFile.good())
    {
        std::ofstream file(STUDENT_DB_FILE);
        file << "# Student Database File\n";
        file << "# Version: 2.0\n";
        file << "# Total Students: 0\n";
        file << "---\n";
        file.close();
        std::cout << "Initialized new student database.\n";
    }
    testFile.close();
}

void Database::exportToCSV(const std::string &filename)
{
    std::cout << "CSV export feature - to be implemented\n";
    (void)filename; // Suppress unused parameter warning
}

int Database::countStudents()
{
    std::ifstream file(STUDENT_DB_FILE);
    if (!file.is_open())
    {
        return 0;
    }

    int count = 0;
    std::string line;

    while (std::getline(file, line))
    {
        if (line == "STUDENT_START")
        {
            count++;
        }
    }

    file.close();
    return count;
}

void Database::clearInsertionOrder()
{
    std::remove(INSERTION_ORDER_FILE.c_str());
}
