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
    std::string __roll_number;
    ERPSystem<std::string, std::string> *__erp_system;
    CourseCatalog *courseCatalog;
    SemesterManager *semesterManager;

    // Helper methods
    void display_menu();
    void view_personal_info();
    void view_enrollments();
    void view_pending_requests();
    void register_for_course();
    void drop_course();
    void view_courses_list();
    void view_course_details();
    void search_courses();
    void view_grades();
    void view_transcript();
    void download_transcript();
    void change_password();
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
