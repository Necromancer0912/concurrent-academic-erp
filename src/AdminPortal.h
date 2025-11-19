#ifndef ADMINPORTAL_H
#define ADMINPORTAL_H

#include <string>
#include <vector>
#include "ERPSystem.h"
#include "CourseCatalog.h"
#include "SemesterManager.h"
#include "Database.h"
#include "Auth.h"

class AdminPortal
{
private:
    std::string adminUsername;
    ERPSystem<std::string, std::string> *erpSystem;
    CourseCatalog *courseCatalog;
    SemesterManager *semesterManager;

    // Helper methods
    void displayMenu();
    void addSingleStudent();
    void addBulkStudents();
    void viewAllStudents();
    void searchStudent();
    void updateStudentMarks();
    void deleteStudent();
    void deleteBulkStudents();
    void deleteAllStudents();
    void manageCourses();
    void manageSemesterSettings();
    void viewBackups();
    void restoreBackup();
    void exportData();
    void viewStatistics();
    void createStudentAccount();
    void bulkCreateStudentAccounts();
    void changeOwnPassword();
    void viewPendingCourseRequests();
    void approveCourseRequest();
    void sortStudentsMenu();
    void sortStudents(int criteria, bool multiThreaded);

    // Iterator and Grade Search methods (Assignment Requirements #4 and #5)
    void viewStudentsInsertionOrder();
    void viewStudentsSortedOrder();
    void findStudentsByGrade();
    void rebuildGradeIndex();

    // Course enrollment view
    void viewCourseEnrollments();

    // IIIT-IIT Course Integration Demo
    void demoIIITIITCourseIntegration();

public:
    AdminPortal(const std::string &username,
                ERPSystem<std::string, std::string> *erp,
                CourseCatalog *catalog,
                SemesterManager *semMgr);

    // Run admin portal
    void run();
};

#endif // ADMINPORTAL_H
