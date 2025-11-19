#include "Auth.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

const std::string Auth::ADMIN_CREDENTIALS_FILE = "data/admin_credentials.dat";
const std::string Auth::STUDENT_CREDENTIALS_FILE = "data/student_credentials.dat";
const std::string Auth::SALT = "IIITD_ERP_2025_SECURE";

std::string Auth::hashPassword(const std::string &_password)
{
    // Simple hash function (in production, use proper cryptographic hashing)
    std::hash<std::string> hasher;
    std::string _salted_password = _password + SALT;
    size_t _hash = hasher(_salted_password);

    std::stringstream _ss;
    _ss << std::hex << std::setfill('0') << std::setw(16) << _hash;
    return _ss.str();
}

std::map<std::string, std::string> Auth::loadCredentials(const std::string &_filename)
{
    std::map<std::string, std::string> _credentials;
    std::ifstream _file(_filename);

    if (!_file.is_open())
    {
        return _credentials;
    }

    std::string _line;
    while (std::getline(_file, _line))
    {
        if (_line.empty() || _line[0] == '#')
            continue;

        size_t _pos = _line.find(':');
        if (_pos != std::string::npos)
        {
            std::string _username = _line.substr(0, _pos);
            std::string _hashed_password = _line.substr(_pos + 1);
            _credentials[_username] = _hashed_password;
        }
    }

    _file.close();
    return _credentials;
}

void Auth::saveCredentials(const std::string &_filename,
                           const std::map<std::string, std::string> &_credentials)
{
    std::ofstream _file(_filename);

    if (!_file.is_open())
    {
        throw std::runtime_error("Unable to open credentials file for writing: " + _filename);
    }

    _file << "# Credentials File - DO NOT EDIT MANUALLY\n";
    _file << "# Format: username:hashed_password\n";

    for (const auto &_pair : _credentials)
    {
        _file << _pair.first << ":" << _pair.second << "\n";
    }

    _file.close();
}

bool Auth::authenticate(const std::string &_username,
                        const std::string &_password,
                        UserType _user_type)
{
    std::string _filename = (_user_type == UserType::ADMIN) ? ADMIN_CREDENTIALS_FILE : STUDENT_CREDENTIALS_FILE;

    auto _credentials = loadCredentials(_filename);

    if (_credentials.find(_username) == _credentials.end())
    {
        return false;
    }

    std::string _hashed_password = hashPassword(_password);
    return _credentials[_username] == _hashed_password;
}

std::string Auth::create_student_account(const std::string &_roll_number)
{
    auto _credentials = loadCredentials(STUDENT_CREDENTIALS_FILE);

    if (_credentials.find(_roll_number) != _credentials.end())
    {
        throw std::runtime_error("Student account already exists: " + _roll_number);
    }

    std::string _password = generate_random_password();
    _credentials[_roll_number] = hashPassword(_password);

    saveCredentials(STUDENT_CREDENTIALS_FILE, _credentials);

    return _password;
}

bool Auth::create_student_account_with_password(const std::string &_roll_number,
                                                const std::string &_password)
{
    auto _credentials = loadCredentials(STUDENT_CREDENTIALS_FILE);

    if (_credentials.find(_roll_number) != _credentials.end())
    {
        return false; // Account already exists
    }

    _credentials[_roll_number] = hashPassword(_password);
    saveCredentials(STUDENT_CREDENTIALS_FILE, _credentials);

    return true;
}

bool Auth::create_admin_account(const std::string &_username,
                                const std::string &_password)
{
    auto _credentials = loadCredentials(ADMIN_CREDENTIALS_FILE);

    if (_credentials.find(_username) != _credentials.end())
    {
        return false;
    }

    _credentials[_username] = hashPassword(_password);
    saveCredentials(ADMIN_CREDENTIALS_FILE, _credentials);

    return true;
}

bool Auth::change_password(const std::string &_username,
                           const std::string &_old_password,
                           const std::string &_new_password,
                           UserType _user_type)
{
    std::string _filename = (_user_type == UserType::ADMIN) ? ADMIN_CREDENTIALS_FILE : STUDENT_CREDENTIALS_FILE;

    auto _credentials = loadCredentials(_filename);

    if (_credentials.find(_username) == _credentials.end())
    {
        return false;
    }

    if (_credentials[_username] != hashPassword(_old_password))
    {
        return false;
    }

    _credentials[_username] = hashPassword(_new_password);
    saveCredentials(_filename, _credentials);

    return true;
}

std::string Auth::generate_random_password(int _length)
{
    const std::string _chars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%";

    std::random_device _rd;
    std::mt19937 _gen(_rd());
    std::uniform_int_distribution<> _dis(0, _chars.length() - 1);

    std::string _password;
    for (int _i = 0; _i < _length; ++_i)
    {
        _password += _chars[_dis(_gen)];
    }

    return _password;
}

bool Auth::username_exists(const std::string &_username, UserType _user_type)
{
    std::string _filename = (_user_type == UserType::ADMIN) ? ADMIN_CREDENTIALS_FILE : STUDENT_CREDENTIALS_FILE;

    auto _credentials = loadCredentials(_filename);
    return _credentials.find(_username) != _credentials.end();
}

void Auth::initializeCredentialFiles()
{
    // Create data directory if it doesn't exist
    system("mkdir -p data");

    // Initialize admin credentials with default admin account
    std::ifstream _admin_file(ADMIN_CREDENTIALS_FILE);
    if (!_admin_file.good())
    {
        std::map<std::string, std::string> _admin_creds;
        _admin_creds["admin"] = hashPassword("admin123");
        _admin_creds["super"] = hashPassword("super123");
        _admin_creds["sayan"] = hashPassword("sayan123"); // Creator/Super user account
        saveCredentials(ADMIN_CREDENTIALS_FILE, _admin_creds);
        std::cout << "Default admin accounts created:\n";
        std::cout << "  Username: admin, Password: admin123\n";
        std::cout << "  Username: super, Password: super123\n";
        std::cout << "  Username: sayan, Password: sayan123 (Creator)\n";
    }
    _admin_file.close();

    // Initialize student credentials file
    std::ifstream _student_file(STUDENT_CREDENTIALS_FILE);
    if (!_student_file.good())
    {
        std::map<std::string, std::string> _student_creds;
        saveCredentials(STUDENT_CREDENTIALS_FILE, _student_creds);
    }
    _student_file.close();
}
