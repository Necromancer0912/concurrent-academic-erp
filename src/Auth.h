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
    static std::string create_student_account(const std::string &rollNumber);

    // Create new student account with specific password (for bulk creation)
    static bool create_student_account_with_password(const std::string &rollNumber,
                                                     const std::string &password);

    // Create new admin account
    static bool create_admin_account(const std::string &username,
                                     const std::string &password);

    // Change password
    static bool change_password(const std::string &username,
                                const std::string &oldPassword,
                                const std::string &newPassword,
                                UserType userType);

    // Generate random password
    static std::string generate_random_password(int length = 8);

    // Check if username exists
    static bool username_exists(const std::string &username, UserType userType);

    // Initialize credential files if they don't exist
    static void initializeCredentialFiles();
};

#endif // AUTH_H
