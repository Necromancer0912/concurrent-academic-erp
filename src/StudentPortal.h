#ifndef STUDENTPORTAL_H
#define STUDENTPORTAL_H

#include <string>
#include "ERPSystem.h"
#include "CourseCatalog.h"
#include "SemesterManager.h"
#include "Student.h"
#include "Auth.h"

class StudentPortal
{
private:
    std::string rollNumber;
    ERPSystem<std::string, std::string> *erpSystem;
    CourseCatalog *courseCatalog;
    SemesterManager *semesterManager;

    // Helper methods
    void displayMenu();
    void viewPersonalInfo();
    void viewEnrollments();
    void viewPendingRequests();
    void registerForCourse();
    void dropCourse();
    void viewCoursesList();
    void viewCourseDetails();
    void searchCourses();
    void viewGrades();
    void viewTranscript();
    void downloadTranscript();
    void changePassword();
    Student<std::string, std::string> getCurrentStudent();

public:
    StudentPortal(const std::string &roll,
                  ERPSystem<std::string, std::string> *erp,
                  CourseCatalog *catalog,
                  SemesterManager *semMgr);

    // Run student portal
    void run();
};

#endif // STUDENTPORTAL_H
