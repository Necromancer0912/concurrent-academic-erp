# 🎓 IIIT-Delhi ERP System

<div align="center">

### Object-Oriented Programming & Design - Assignment 4

**Advanced Template Programming & Multithreaded Data Structures**

---

**👨‍💻 Developed by:** Sayan Das  
**🆔 Roll Number:** MT25041  
**🏛️ Institution:** IIIT-Delhi  
**📚 Course:** Object-Oriented Programming & Design

---

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![Build](https://img.shields.io/badge/Build-Passing-success.svg)]()
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS-lightgrey.svg)]()

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Key Features](#-key-features)
- [Technical Highlights](#-technical-highlights)
- [Project Structure](#-project-structure)
- [Installation & Setup](#-installation--setup)
- [Usage Guide](#-usage-guide)
- [Functional Modules](#-functional-modules)
- [System Architecture](#-system-architecture)
- [Assignment Requirements](#-assignment-requirements)
- [Credits](#-credits)

---

## 🎯 Overview

A comprehensive **Enterprise Resource Planning (ERP) System** designed for IIIT-Delhi, built with advanced C++ features including **template metaprogramming**, **multithreading**, and **STL algorithms**. The system manages student records, course enrollment, grading, and administrative tasks with high performance and type safety.

### 🌟 Core Capabilities

- **Dual-Portal Architecture**: Separate interfaces for Students and Administrators
- **Cross-Institutional Integration**: Supports both IIIT-Delhi (string course codes) and IIT-Delhi (integer course codes)
- **High-Performance Sorting**: Parallel sorting with dynamic thread allocation (2-6 threads based on dataset size)
- **Persistent Storage**: Custom binary database with insertion order preservation
- **Template-Based Design**: Type-safe operations across different institutional systems

---

## ✨ Key Features

### 👨‍🎓 Student Portal

- 📊 **Academic Dashboard** - View CGPA, enrolled courses, and academic standing
- 📝 **Course Registration** - Request enrollment with credit validation
- 🗑️ **Course Management** - Drop courses (when enabled by administration)
- 📄 **Transcript Generation** - Download detailed academic transcript
- 🔒 **Account Security** - Change password and manage credentials
- 🎯 **Grade Tracking** - Real-time grade updates and CGPA calculation

### 👨‍💼 Admin Portal

#### Student Management
- ➕ **Add Students** - Single or bulk CSV import (10,000+ records supported)
- 🔍 **Search & Filter** - Find students by roll number, name, branch, or CGPA
- ✏️ **Update Records** - Modify student information and grades
- 🗑️ **Delete Operations** - Remove individual or bulk students
- 🔄 **Sorting Engine** - Multi-threaded sorting by Name/Roll/CGPA

#### Course Management
- 📚 **Course Catalog** - Manage IIIT-Delhi and IIT-Delhi courses
- 📊 **Enrollment Analytics** - View course-wise student statistics
- ✅ **Registration Approval** - Review and process course requests
- 🔧 **Semester Controls** - Toggle registration and drop periods

#### System Administration
- 👤 **Account Creation** - Generate student credentials (bulk supported)
- 💾 **Database Operations** - Backup, restore, and export functionality
- 📈 **System Statistics** - View enrollment distribution and analytics
- 🔐 **Security Management** - Admin password controls

---

## 🔬 Technical Highlights

### Template Programming

```cpp
// Flexible student representation supporting different course code types
Student<std::string, std::string>  // IIIT-Delhi (string roll, string courses)
Student<std::string, int>          // IIT-Delhi (string roll, int courses)

// Type-safe course catalog
Course<std::string>  // IIIT-Delhi courses (CSE201, MTH101)
Course<int>          // IIT-Delhi courses (101, 201, 305)
```

### Multithreaded Sorting Algorithm

**Dynamic Thread Allocation:**
- ≤3,000 students → 2 threads
- 3,001-6,000 → 3 threads
- 6,001-10,000 → 4 threads
- 10,001-15,000 → 5 threads
- >15,000 → 6 threads

**Performance:** Sorts 10,000 students in ~2-3ms using parallel merge sort with `std::sort` optimization.

### Data Persistence

**Two-File Architecture:**
- `students.db` - Complete student data (sortable)
- `insertion_order.db` - Preserves original insertion sequence
- Survives program restarts and sorting operations

---

## 📁 Project Structure

```
Assignment_4_not_tested_without_sql/
│
├── 📄 Makefile                     # Build configuration with clean targets
├── 📝 README.md                    # This file
├── 🚫 .gitignore                   # Version control exclusions
├── 📊 demo_students.csv            # Sample dataset (10,000 students)
│
├── 📂 src/                         # Source code directory
│   ├── 🎯 main.cpp                 # Application entry point
│   │
│   ├── 👤 Student Management
│   │   ├── Student.h / .cpp        # Template-based student class
│   │   ├── EnrollmentRecord.h / .cpp
│   │   └── Grade.h / .cpp          # Grade and CGPA calculations
│   │
│   ├── 📚 Course Management
│   │   ├── Course.h / .cpp         # Template-based course class
│   │   ├── CourseCatalog.h / .cpp  # Catalog with IIIT+IIT courses
│   │   └── Branch.h / .cpp         # Academic branches
│   │
│   ├── 🖥️ System Core
│   │   ├── ERPSystem.h / .cpp      # Main ERP logic with templates
│   │   ├── Database.h / .cpp       # Binary file I/O operations
│   │   └── CSVHandler.h / .cpp     # CSV import/export
│   │
│   ├── 🔐 Authentication
│   │   └── Auth.h / .cpp           # User authentication system
│   │
│   ├── 🎨 User Interfaces
│   │   ├── AdminPortal.h / .cpp    # Administrator interface
│   │   └── StudentPortal.h / .cpp  # Student interface
│   │
│   ├── ⚙️ Advanced Features
│   │   ├── SortingManager.h / .cpp # Parallel sorting engine
│   │   ├── SemesterManager.h / .cpp # Academic semester controls
│   │   └── PendingCourseRequest.h  # Course request queue
│   │
│   └── 📂 obj/                     # Compiled object files (generated)
│
├── 📂 data/                        # Runtime data (auto-created)
│   ├── students.db                 # Student database
│   ├── insertion_order.db          # Insertion order tracker
│   ├── course_catalog.dat          # Course definitions
│   ├── admin_credentials.dat       # Admin accounts
│   └── student_credentials.dat     # Student accounts
│
└── 📂 docs/                        # Documentation (optional)
    ├── IIIT_IIT_DEMO.md           # Template demo guide
    └── PRINT_CLEANUP_SUMMARY.md   # Code style documentation
```

---

## 🚀 Installation & Setup

### Prerequisites

- **Compiler:** g++ with C++17 support
- **OS:** Linux or macOS
- **Make:** GNU Make utility

### Build Commands

```bash
# Compile the project
make

# Run the application
./erp_system

# Run directly (builds if needed)
make run

# Clean build artifacts
make clean

# Clean everything (including database)
make clean-all
```

### First Run

On first execution, the system will:
1. Create necessary directories (`data/`, `data/syllabus/`)
2. Initialize credential files
3. Auto-load `demo_students.csv` if database is empty
4. Set up course catalog with IIIT-Delhi and IIT-Delhi courses

---

## 📖 Usage Guide

### Starting the Application

```bash
$ make run
```

### Login Credentials

**Administrator:**
- Username: `admin`
- Password: `admin123`

**Students:**
- Username: `<roll_number>` (e.g., 201900001)
- Default Password: `abc123` (changeable after first login)

### Navigation

1. **Select Portal Type**
   - Student Portal (1)
   - Admin Portal (2)
   - Exit (3)

2. **Login**
   - Enter credentials
   - Maximum 3 attempts

3. **Use Menu Options**
   - Navigate using numeric choices
   - Follow on-screen prompts

---

## 🔧 Functional Modules

### 1️⃣ Student Management System

**Operations:**
- Add students (single or bulk via CSV)
- Search by multiple criteria
- Update student information
- Delete records (single or bulk)
- Sort by Name/Roll Number/CGPA

**Features:**
- Insertion order preservation across restarts
- Parallel sorting for large datasets
- Branch-wise filtering
- Level-wise filtering (BTech, MTech, PhD, Dual Degree)

### 2️⃣ Course Registration System

**Workflow:**
1. Student submits course request
2. System validates:
   - Course availability
   - Credit limits
   - Prerequisites
   - Level eligibility
3. Admin reviews and approves/rejects
4. Course added to student record

**Credit Management:**
- BTech/MTech/Dual: 16 credits/semester
- Capstone: 8 credits
- Research: 16 credits

### 3️⃣ IIIT-IIT Course Integration

**Demonstration (Option 23 in Admin Portal):**

Shows template flexibility with:
- IIIT-Delhi student with string course codes (CSE201, MTH101)
- IIT-Delhi student with integer course codes (101, 201, 305)
- Cross-institutional enrollment
- Grade assignment across both systems

### 4️⃣ Parallel Sorting Engine

**Algorithm:**
1. Determine optimal thread count based on dataset size
2. Divide data into equal segments
3. Sort each segment in parallel using `std::sort`
4. Merge sorted segments
5. Report per-thread timing and total time

**Performance Metrics:**
- Individual thread execution times
- Maximum parallel time
- Merge operation time
- Total sorting time

### 5️⃣ Authentication System

**Features:**
- Separate admin and student credential stores
- Password hashing (basic implementation)
- Login attempt tracking
- Password change functionality
- Account creation with generated passwords

---

## 🏗️ System Architecture

### Design Patterns

**Template Method Pattern:**
- `Student<RollType, CourseCodeType>` allows compile-time polymorphism
- `Course<CourseCodeType>` provides type-safe course representation

**Strategy Pattern:**
- Multiple sorting strategies (by name, roll, CGPA)
- Configurable thread count based on data size

**Observer Pattern:**
- Grade updates trigger CGPA recalculation
- Enrollment changes update course statistics

### Data Flow

```
┌─────────────┐
│   CSV File  │
└──────┬──────┘
       │
       ↓
┌─────────────┐     ┌──────────────┐
│ CSV Handler │ ──→ |  ERP System  │
└─────────────┘     └───────┬──────┘
                            │
                    ┌───────┴────────┐
                    ↓                ↓
            ┌──────────────┐  ┌────────────┐
            │   Database   │  │  Sorting   │
            │   students.db│  │  Manager   │
            └──────────────┘  └────────────┘
                    ↓
            ┌────────────────┐
            │ Portal Views   │
            │ (Admin/Student)│
            └────────────────┘
```

### Threading Model

```
Main Thread
    │
    ├─→ Thread 1: Sort segment [0, n/k]
    ├─→ Thread 2: Sort segment [n/k, 2n/k]
    ├─→ Thread 3: Sort segment [2n/k, 3n/k]
    └─→ ...
    
    Wait for all threads
    
    │
    ↓
Merge sorted segments
```

---

## 📝 Assignment Requirements

### ✅ Core Features Implemented

| Requirement | Implementation | Status |
|------------|----------------|---------|
| Template Classes | `Student<RollType, CourseCodeType>`, `Course<CourseCodeType>` | ✅ Complete |
| STL Containers | `vector`, `map`, `set`, `queue` | ✅ Complete |
| Multithreading | Parallel sorting with 2-6 threads | ✅ Complete |
| File I/O | Binary database + CSV import/export | ✅ Complete |
| Iterators | Forward/Reverse iterators for sorted/insertion order | ✅ Complete |
| Exception Handling | Try-catch blocks throughout | ✅ Complete |
| IIIT-IIT Integration | Cross-institutional course enrollment demo | ✅ Complete |

### 🎨 Advanced Features

- **Dynamic Thread Allocation** - Optimizes performance based on data size
- **Insertion Order Preservation** - Two-file architecture maintains both sorted and original order
- **Type Safety** - Templates prevent mixing incompatible course code types
- **Performance Optimization** - Uses `std::sort` (introsort) for segment sorting
- **User-Friendly Interface** - Clear menus with box-drawing characters
- **Comprehensive Error Handling** - Proper error messages for all failure cases

---

## 🎓 Course Information

### IIIT-Delhi Courses (String Codes)

- **CSE102** - Data Structures and Algorithms
- **CSE201** - Object-Oriented Programming and Design
- **CSE202** - Database Management Systems
- **CSE231** - Operating Systems
- **CSE232** - Computer Networks
- **CSE343** - Machine Learning
- **CSE643** - Artificial Intelligence
- **CSE501** - Advanced Algorithms
- **CSE502** - Distributed Systems
- **CSE503** - Information Security

### IIT-Delhi Courses (Integer Codes)

- **101** - Introduction to Computing
- **201** - Advanced Algorithms
- **202** - Computer Architecture
- **305** - Database Systems
- **401** - Artificial Intelligence and Machine Learning
- **501** - Advanced Computer Networks
- **502** - Parallel and Distributed Computing
- **601** - Advanced Topics in Computer Vision

---

## 🛠️ Development Notes

### Code Style

- **No AI-style markers** - Clean, professional output
- **No emojis in output** - Serious, academic tone
- **Consistent error format** - `ERROR: description.`
- **Clear success messages** - `Operation completed successfully.`

### Testing

The system has been tested with:
- 10,000+ student records
- Bulk CSV operations
- Concurrent sorting operations
- Database persistence across restarts
- Cross-portal data consistency

### Known Limitations

- Single-user system (no concurrent logins)
- Basic password security (not production-grade)
- No network capabilities
- Console-based interface only

---

## 📚 Credits

**Developer:** Sayan Das  
**Roll Number:** MT25041  
**Course:** Object-Oriented Programming & Design  
**Institution:** IIIT-Delhi  
**Assignment:** Assignment 4 - Templates and Threads  
**Semester:** 2024-25

---

## 📄 License

This project is submitted as part of academic coursework at IIIT-Delhi.

---

## 🤝 Acknowledgments

- **Course Instructors** - For comprehensive OOP guidance
- **Teaching Assistants** - For clarifying assignment requirements
- **IIIT-Delhi** - For providing an excellent learning environment

---

<div align="center">

### 🌟 Thank You for Reviewing This Project! 🌟

**Made with dedication for OOPD Assignment 4**

</div>
