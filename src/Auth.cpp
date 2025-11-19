#include "Auth.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

const std::string Auth::ADMIN_CREDENTIALS_FILE = "data/admin_credentials.dat";
const std::string Auth::STUDENT_CREDENTIALS_FILE = "data/student_credentials.dat";
const std::string Auth::SALT = "IIITD_ERP_2025_SECURE";

std::string Auth::hashPassword(const std::string &password)
{
    // Simple hash function (in production, use proper cryptographic hashing)
    std::hash<std::string> hasher;
    std::string saltedPassword = password + SALT;
    size_t hash = hasher(saltedPassword);

    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(16) << hash;
    return ss.str();
}

std::map<std::string, std::string> Auth::loadCredentials(const std::string &filename)
{
    std::map<std::string, std::string> credentials;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return credentials;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        size_t pos = line.find(':');
        if (pos != std::string::npos)
        {
            std::string username = line.substr(0, pos);
            std::string hashedPassword = line.substr(pos + 1);
            credentials[username] = hashedPassword;
        }
    }

    file.close();
    return credentials;
}

void Auth::saveCredentials(const std::string &filename,
                           const std::map<std::string, std::string> &credentials)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open credentials file for writing: " + filename);
    }

    file << "# Credentials File - DO NOT EDIT MANUALLY\n";
    file << "# Format: username:hashed_password\n";

    for (const auto &pair : credentials)
    {
        file << pair.first << ":" << pair.second << "\n";
    }

    file.close();
}

bool Auth::authenticate(const std::string &username,
                        const std::string &password,
                        UserType userType)
{
    std::string filename = (userType == UserType::ADMIN) ? ADMIN_CREDENTIALS_FILE : STUDENT_CREDENTIALS_FILE;

    auto credentials = loadCredentials(filename);

    if (credentials.find(username) == credentials.end())
    {
        return false;
    }

    std::string hashedPassword = hashPassword(password);
    return credentials[username] == hashedPassword;
}

std::string Auth::createStudentAccount(const std::string &rollNumber)
{
    auto credentials = loadCredentials(STUDENT_CREDENTIALS_FILE);

    if (credentials.find(rollNumber) != credentials.end())
    {
        throw std::runtime_error("Student account already exists: " + rollNumber);
    }

    std::string password = generateRandomPassword();
    credentials[rollNumber] = hashPassword(password);

    saveCredentials(STUDENT_CREDENTIALS_FILE, credentials);

    return password;
}

bool Auth::createStudentAccountWithPassword(const std::string &rollNumber,
                                            const std::string &password)
{
    auto credentials = loadCredentials(STUDENT_CREDENTIALS_FILE);

    if (credentials.find(rollNumber) != credentials.end())
    {
        return false; // Account already exists
    }

    credentials[rollNumber] = hashPassword(password);
    saveCredentials(STUDENT_CREDENTIALS_FILE, credentials);

    return true;
}

bool Auth::createAdminAccount(const std::string &username,
                              const std::string &password)
{
    auto credentials = loadCredentials(ADMIN_CREDENTIALS_FILE);

    if (credentials.find(username) != credentials.end())
    {
        return false;
    }

    credentials[username] = hashPassword(password);
    saveCredentials(ADMIN_CREDENTIALS_FILE, credentials);

    return true;
}

bool Auth::changePassword(const std::string &username,
                          const std::string &oldPassword,
                          const std::string &newPassword,
                          UserType userType)
{
    std::string filename = (userType == UserType::ADMIN) ? ADMIN_CREDENTIALS_FILE : STUDENT_CREDENTIALS_FILE;

    auto credentials = loadCredentials(filename);

    if (credentials.find(username) == credentials.end())
    {
        return false;
    }

    if (credentials[username] != hashPassword(oldPassword))
    {
        return false;
    }

    credentials[username] = hashPassword(newPassword);
    saveCredentials(filename, credentials);

    return true;
}

std::string Auth::generateRandomPassword(int length)
{
    const std::string chars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.length() - 1);

    std::string password;
    for (int i = 0; i < length; ++i)
    {
        password += chars[dis(gen)];
    }

    return password;
}

bool Auth::usernameExists(const std::string &username, UserType userType)
{
    std::string filename = (userType == UserType::ADMIN) ? ADMIN_CREDENTIALS_FILE : STUDENT_CREDENTIALS_FILE;

    auto credentials = loadCredentials(filename);
    return credentials.find(username) != credentials.end();
}

void Auth::initializeCredentialFiles()
{
    // Create data directory if it doesn't exist
    system("mkdir -p data");

    // Initialize admin credentials with default admin account
    std::ifstream adminFile(ADMIN_CREDENTIALS_FILE);
    if (!adminFile.good())
    {
        std::map<std::string, std::string> adminCreds;
        adminCreds["admin"] = hashPassword("admin123");
        adminCreds["super"] = hashPassword("super123");
        adminCreds["sayan"] = hashPassword("sayan123"); // Creator/Super user account
        saveCredentials(ADMIN_CREDENTIALS_FILE, adminCreds);
        std::cout << "Default admin accounts created:\n";
        std::cout << "  Username: admin, Password: admin123\n";
        std::cout << "  Username: super, Password: super123\n";
        std::cout << "  Username: sayan, Password: sayan123 (Creator)\n";
    }
    adminFile.close();

    // Initialize student credentials file
    std::ifstream studentFile(STUDENT_CREDENTIALS_FILE);
    if (!studentFile.good())
    {
        std::map<std::string, std::string> studentCreds;
        saveCredentials(STUDENT_CREDENTIALS_FILE, studentCreds);
    }
    studentFile.close();
}
