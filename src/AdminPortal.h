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
    std::string __admin_username;
    ERPSystem<std::string, std::string> *erpSystem;
    CourseCatalog *courseCatalog;
    SemesterManager *semesterManager;

    // Helper methods
    void display_menu();
    void add_single_student();
    void add_bulk_students();
    void view_all_students();
    void search_student();
    void update_student_marks();
    void delete_student();
    void delete_bulk_students();
    void delete_all_students();
    void manage_courses();
    void manage_semester_settings();
    void view_backups();
    void restore_backup();
    void export_data();
    void view_statistics();
    void create_student_account();
    void bulk_create_student_accounts();
    void change_own_password();
    void view_pending_course_requests();
    void approve_course_request();
    void sort_students_menu();
    void sort_students(int criteria, bool multiThreaded);

    // Iterator and Grade Search methods (Assignment Requirements #4 and #5)
    void view_students_insertion_order();
    void view_students_sorted_order();
    void find_students_by_grade();
    void rebuild_grade_index();

    // Course enrollment view
    void view_course_enrollments();

    // IIIT-IIT Course Integration Demo
    void demo_iiit_iit_course_integration();

public:
    AdminPortal(const std::string &username,
                ERPSystem<std::string, std::string> *erp,
                CourseCatalog *catalog,
                SemesterManager *semMgr);

    // Run admin portal
    void run();
};

#endif // ADMINPORTAL_H
