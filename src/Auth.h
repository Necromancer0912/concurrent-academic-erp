#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <fstream>
#include <map>
#include <chrono>
#include <random>

enum class UserType
{
    STUDENT,
    ADMIN
};

class Auth
{
private:
    static const std::string ADMIN_CREDENTIALS_FILE;
    static const std::string STUDENT_CREDENTIALS_FILE;
    static const std::string SALT;

    // Hash password using simple hashing (in production, use bcrypt or similar)
    static std::string hashPassword(const std::string &password);

    // Load credentials from file
    static std::map<std::string, std::string> loadCredentials(const std::string &filename);

    // Save credentials to file
    static void saveCredentials(const std::string &filename,
                                const std::map<std::string, std::string> &credentials);

public:
    // Authenticate user
    static bool authenticate(const std::string &username,
                             const std::string &password,
                             UserType userType);

    // Create new student account (returns generated password)
    static std::string createStudentAccount(const std::string &rollNumber);

    // Create new student account with specific password (for bulk creation)
    static bool createStudentAccountWithPassword(const std::string &rollNumber,
                                                 const std::string &password);

    // Create new admin account
    static bool createAdminAccount(const std::string &username,
                                   const std::string &password);

    // Change password
    static bool changePassword(const std::string &username,
                               const std::string &oldPassword,
                               const std::string &newPassword,
                               UserType userType);

    // Generate random password
    static std::string generateRandomPassword(int length = 8);

    // Check if username exists
    static bool usernameExists(const std::string &username, UserType userType);

    // Initialize credential files if they don't exist
    static void initializeCredentialFiles();
};

#endif // AUTH_H
