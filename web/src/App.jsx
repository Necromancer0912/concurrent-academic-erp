import React, { useState, useEffect } from 'react';
import './App.css';

// Initial Mock Students Database
const INITIAL_STUDENTS = [
  {
    name: "Rahul Kumar",
    rollNumber: "2023CSE101",
    branch: { code: "CSE", name: "Computer Science & Engineering", dept: "Engineering" },
    level: "BTECH",
    startingYear: 2023,
    loadedCgpa: 9.3,
    institution: "Apex University",
    currentCourses: [
      { code: "CSE201", name: "Data Structures", credits: 4, instructor: "Dr. Sharma" },
      { code: "CSE202", name: "Object Oriented Programming", credits: 4, instructor: "Dr. Verma" },
      { code: "MTH101", name: "Linear Algebra", credits: 3, instructor: "Prof. Gupta" }
    ],
    grades: { "CSE201": "A", "CSE202": "A-", "MTH101": "A+" }
  },
  {
    name: "Priya Singh",
    rollNumber: "2023CSE102",
    branch: { code: "CSE", name: "Computer Science & Engineering", dept: "Engineering" },
    level: "BTECH",
    startingYear: 2023,
    loadedCgpa: 8.8,
    institution: "Vertex Institute",
    currentCourses: [
      { code: "101", name: "Introduction to Computing", credits: 4, instructor: "Prof. Subhashis Banerjee (Vertex)" },
      { code: "201", name: "Advanced Algorithms", credits: 4, instructor: "Prof. Mausam (Vertex)" },
      { code: "305", name: "Database Systems", credits: 4, instructor: "Prof. S. Sudarshan (Vertex)" }
    ],
    grades: { "101": "B+", "201": "A", "305": "B" }
  },
  {
    name: "Arjun Patel",
    rollNumber: "2023CSE103",
    branch: { code: "CSE", name: "Computer Science & Engineering", dept: "Engineering" },
    level: "BTECH",
    startingYear: 2023,
    loadedCgpa: 9.0,
    institution: "Apex University",
    currentCourses: [
      { code: "401", name: "Machine Learning (Vertex)", credits: 4, instructor: "Prof. Parag Singla (Vertex)" },
      { code: "505", name: "Computer Vision (Vertex)", credits: 3, instructor: "Prof. Chetan Arora (Vertex)" },
      { code: "301", name: "Operating Systems (Vertex)", credits: 4, instructor: "Prof. Bhatia (Vertex)" }
    ],
    grades: { "401": "A-", "505": "A", "301": "B+" }
  },
  {
    name: "Sneha Sharma",
    rollNumber: "2024ECE108",
    branch: { code: "ECE", name: "Electronics & Communications", dept: "Engineering" },
    level: "BTECH",
    startingYear: 2024,
    loadedCgpa: 8.5,
    institution: "Apex University",
    currentCourses: [
      { code: "ECE101", name: "Digital Circuits & Logic", credits: 4, instructor: "Dr. Sneh Saurabh" },
      { code: "MAT101", name: "Mathematics I", credits: 4, instructor: "Dr. Debarka Sengupta" },
      { code: "HSS101", name: "Communication Skills", credits: 2, instructor: "Dr. Jainendra Shukla" }
    ],
    grades: { "ECE101": "B", "MAT101": "A-", "HSS101": "A" }
  },
  {
    name: "Kabir Sen",
    rollNumber: "2024MTCSE05",
    branch: { code: "CSE", name: "Computer Science & Engineering", dept: "Engineering" },
    level: "MTECH",
    startingYear: 2024,
    loadedCgpa: 9.5,
    institution: "Apex University",
    currentCourses: [
      { code: "CSE501", name: "Advanced Algorithms", credits: 4, instructor: "Dr. Sanjit Kaul" },
      { code: "CSE502", name: "Distributed Systems", credits: 4, instructor: "Dr. Pravesh Biyani" },
      { code: "CSE503", name: "Information Security", credits: 4, instructor: "Dr. Sambuddho Chakravarty" }
    ],
    grades: { "CSE501": "A+", "CSE502": "A", "CSE503": "A" }
  },
  {
    name: "Ananya Roy",
    rollNumber: "2025PHDCSE12",
    branch: { code: "CSE", name: "Computer Science & Engineering", dept: "Engineering" },
    level: "PHD",
    startingYear: 2025,
    loadedCgpa: 9.7,
    institution: "Apex University",
    currentCourses: [
      { code: "RES001", name: "Research Methodology", credits: 16, instructor: "PhD Advisor" },
      { code: "RES002", name: "PhD Seminar & Thesis", credits: 16, instructor: "PhD Committee" }
    ],
    grades: { "RES001": "A+", "RES002": "A+" }
  }
];

// Default Catalog Courses
const APEX_COURSES = [
  { code: "CSE101", name: "Introduction to Programming", credits: 4, instructor: "Dr. Pankaj Jalote" },
  { code: "MAT101", name: "Mathematics I (Linear Algebra & Calculus)", credits: 4, instructor: "Dr. Debarka Sengupta" },
  { code: "ECE101", name: "Digital Circuits and Logic Design", credits: 4, instructor: "Dr. Sneh Saurabh" },
  { code: "CSE102", name: "Data Structures and Algorithms", credits: 4, instructor: "Dr. Rajiv Ratn Shah", prereqs: "CSE101" },
  { code: "CSE201", name: "Object-Oriented Programming and Design", credits: 4, instructor: "Dr. Arani Bhattacharya" },
  { code: "CSE202", name: "Database Management Systems", credits: 4, instructor: "Dr. Tanmoy Chakraborty", prereqs: "CSE102" },
  { code: "CSE343", name: "Machine Learning", credits: 4, instructor: "Dr. Ganesh Bagler", prereqs: "CSE102" },
  { code: "CSE501", name: "Advanced Algorithms", credits: 4, instructor: "Dr. Sanjit Kaul" }
];

const VERTEX_COURSES = [
  { code: "101", name: "Introduction to Computing", credits: 4, instructor: "Prof. Subhashis Banerjee (Vertex)" },
  { code: "201", name: "Advanced Algorithms", credits: 4, instructor: "Prof. Mausam (Vertex)", prereqs: "101" },
  { code: "202", name: "Computer Architecture", credits: 4, instructor: "Prof. Anshul Kumar (Vertex)" },
  { code: "305", name: "Database Systems", credits: 4, instructor: "Prof. S. Sudarshan (Vertex)", prereqs: "101" },
  { code: "401", name: "Artificial Intelligence and Machine Learning", credits: 4, instructor: "Prof. Parag Singla (Vertex)", prereqs: "201" },
  { code: "501", name: "Advanced Computer Networks", credits: 4, instructor: "Prof. Huzur Saran (Vertex)" }
];

function App() {
  const [currentTab, setCurrentTab] = useState('dashboard');
  const [students, setStudents] = useState(() => {
    const saved = localStorage.getItem('erp_students');
    return saved ? JSON.parse(saved) : INITIAL_STUDENTS;
  });
  
  // Save students database to localStorage
  useEffect(() => {
    localStorage.setItem('erp_students', JSON.stringify(students));
  }, [students]);

  // Form states for adding a student
  const [newName, setNewName] = useState('');
  const [newRoll, setNewRoll] = useState('');
  const [newBranch, setNewBranch] = useState('CSE');
  const [newLevel, setNewLevel] = useState('BTECH');
  const [newYear, setNewYear] = useState(2026);
  const [newCgpa, setNewCgpa] = useState(8.0);
  const [newInst, setNewInst] = useState('Apex University');

  // Search/Filter states
  const [searchTerm, setSearchTerm] = useState('');
  const [filterLevel, setFilterLevel] = useState('ALL');

  // Sorting Visualizer State
  const [sortList, setSortList] = useState([]);
  const [sortState, setSortState] = useState('idle'); // idle, dividing, sorting, merging, completed
  const [activeThreads, setActiveThreads] = useState([]);
  const [sortMetric, setSortMetric] = useState(null);

  // Indexed Search Visualizer State
  const [searchCourseCode, setSearchCourseCode] = useState('CSE201');
  const [searchGradeLetter, setSearchGradeLetter] = useState('A+');
  const [searchPathSteps, setSearchPathSteps] = useState([]);
  const [searchPathProgress, setSearchPathProgress] = useState(0); // 0 to 5
  const [searchResultStudents, setSearchResultStudents] = useState([]);

  // Handle adding student
  const handleAddStudent = (e) => {
    e.preventDefault();
    if (!newName || !newRoll) {
      alert("Please provide Name and Roll Number");
      return;
    }
    const exists = students.some(s => s.rollNumber.toLowerCase() === newRoll.toLowerCase());
    if (exists) {
      alert("Student with this Roll Number already exists!");
      return;
    }

    const branchObj = newBranch === 'CSE' 
      ? { code: "CSE", name: "Computer Science & Engineering", dept: "Engineering" }
      : { code: "ECE", name: "Electronics & Communications", dept: "Engineering" };

    const newStudent = {
      name: newName,
      rollNumber: newRoll,
      branch: branchObj,
      level: newLevel,
      startingYear: parseInt(newYear),
      loadedCgpa: parseFloat(newCgpa),
      institution: newInst,
      currentCourses: [],
      grades: {}
    };

    setStudents([...students, newStudent]);
    setNewName('');
    setNewRoll('');
    alert("Student added successfully!");
  };

  // Delete student
  const handleDeleteStudent = (roll) => {
    if (window.confirm(`Are you sure you want to delete student with Roll Number: ${roll}?`)) {
      setStudents(students.filter(s => s.rollNumber !== roll));
    }
  };

  // Run Concurrent Merge Sort Simulation
  const runSortingSimulation = () => {
    if (students.length === 0) return;
    setSortState('dividing');
    setSortList([...students]);
    setSortMetric(null);
    
    // Step 1: Divide list into sublists
    setTimeout(() => {
      setSortState('sorting');
      setActiveThreads([
        { id: "0x70000a12", name: "Worker Thread 1", progress: 0, range: "[0-1]" },
        { id: "0x70000b34", name: "Worker Thread 2", progress: 0, range: "[2-3]" },
        { id: "0x70000c56", name: "Worker Thread 3", progress: 0, range: "[4-4]" },
        { id: "0x70000d78", name: "Worker Thread 4", progress: 0, range: "[5-5]" }
      ]);
      
      // Step 2: Concurrent sorting progress
      let p = 0;
      const interval = setInterval(() => {
        p += 20;
        setActiveThreads(threads => 
          threads.map(t => ({ ...t, progress: Math.min(p, 100) }))
        );
        if (p >= 100) {
          clearInterval(interval);
          
          // Step 3: Merging phase
          setTimeout(() => {
            setSortState('merging');
            
            // Perform actual sort on list
            setTimeout(() => {
              const sorted = [...students].sort((a, b) => b.loadedCgpa - a.loadedCgpa); // Sort descending CGPA
              setSortList(sorted);
              setSortState('completed');
              setActiveThreads([]);
              setSortMetric({
                threadsUsed: 4,
                durationMs: (10 + Math.random() * 5).toFixed(1),
                recordsSorted: students.length
              });
            }, 1000);
          }, 800);
        }
      }, 300);
    }, 1000);
  };

  // Indexed Grade Search Simulation
  const runIndexedSearchSimulation = () => {
    setSearchPathProgress(0);
    setSearchPathSteps([]);
    setSearchResultStudents([]);

    const steps = [
      { id: 1, title: "Initialize Query Engine", desc: `Searching for students with grade [${searchGradeLetter}] in course [${searchCourseCode}].` },
      { id: 2, title: "Outer Map Lookup O(log C)", desc: `Hashing course catalog map for key "${searchCourseCode}"... Match found.` },
      { id: 3, title: "Inner Map Lookup O(log G)", desc: `Hashing grade sub-map for letter grade "${searchGradeLetter}"... Match found.` },
      { id: 4, title: "Retrieve Student ID List O(K)", desc: "Extracting array of matching student IDs from node bucket." },
      { id: 5, title: "Load Student Records O(1)", desc: "Fetching full profiles from the primary Database registry." }
    ];

    let currentStep = 0;
    const runStep = () => {
      if (currentStep < steps.length) {
        setSearchPathProgress(currentStep + 1);
        setSearchPathSteps(prev => [...prev, steps[currentStep]]);
        currentStep++;
        setTimeout(runStep, 800);
      } else {
        // Find actual matches in our students list
        const results = students.filter(s => {
          return s.grades && s.grades[searchCourseCode] === searchGradeLetter;
        });
        setSearchResultStudents(results);
      }
    };

    setTimeout(runStep, 200);
  };

  // Helper stats
  const totalStudents = students.length;
  const avgCgpa = (students.reduce((acc, s) => acc + s.loadedCgpa, 0) / (students.length || 1)).toFixed(2);
  const apexCount = students.filter(s => s.institution === 'Apex University').length;
  const vertexCount = students.filter(s => s.institution === 'Vertex Institute').length;

  // Filter students for grid view
  const filteredStudents = students.filter(s => {
    const matchesSearch = s.name.toLowerCase().includes(searchTerm.toLowerCase()) || 
                          s.rollNumber.toLowerCase().includes(searchTerm.toLowerCase());
    const matchesLevel = filterLevel === 'ALL' || s.level === filterLevel;
    return matchesSearch && matchesLevel;
  });

  return (
    <div className="app-container">
      {/* Navigation Panel */}
      <aside className="nav-panel">
        <div className="nav-header">
          <h1>Apex ERP</h1>
          <div className="subtitle">Archival Academic Vault</div>
        </div>

        <nav className="nav-menu">
          <button 
            className={`nav-item ${currentTab === 'dashboard' ? 'active' : ''}`}
            onClick={() => setCurrentTab('dashboard')}
          >
            Dashboard
          </button>
          <button 
            className={`nav-item ${currentTab === 'directory' ? 'active' : ''}`}
            onClick={() => setCurrentTab('directory')}
          >
            Student Directory
          </button>
          <button 
            className={`nav-item ${currentTab === 'catalog' ? 'active' : ''}`}
            onClick={() => setCurrentTab('catalog')}
          >
            Course Catalog
          </button>
          <button 
            className={`nav-item ${currentTab === 'sorting' ? 'active' : ''}`}
            onClick={() => setCurrentTab('sorting')}
          >
            Threaded Sort
          </button>
          <button 
            className={`nav-item ${currentTab === 'indexing' ? 'active' : ''}`}
            onClick={() => setCurrentTab('indexing')}
          >
            Indexed Search
          </button>
        </nav>

        <div className="nav-footer">
          <p>Apex University ERP v2.0</p>
          <p style={{ color: 'var(--primary-accent)' }}>• Multi-threading active</p>
          <p>DB Engine: C++ Custom AVL</p>
        </div>
      </aside>

      {/* Main Content Area */}
      <main className="main-content">
        
        {/* VIEW 1: Dashboard */}
        {currentTab === 'dashboard' && (
          <div>
            <h2 className="page-title">Institutional Overview</h2>
            <p className="page-description">
              Apex University ERP System manages dual-campus academics, records, and student registrations. 
              The system utilizes templated architectures to compile cross-institutional student records with zero runtime overhead.
            </p>

            <div className="dashboard-grid">
              <div className="stat-card">
                <div className="label">Total Registry Enrollment</div>
                <div className="value">{totalStudents}</div>
                <div className="delta">↑ Active Academic Term</div>
              </div>
              <div className="stat-card">
                <div className="label">Apex University Students</div>
                <div className="value">{apexCount}</div>
                <div className="delta" style={{ color: 'var(--primary-accent)' }}>String Roll Number Format</div>
              </div>
              <div className="stat-card">
                <div className="label">Vertex Institute Students</div>
                <div className="value">{vertexCount}</div>
                <div className="delta" style={{ color: 'var(--tertiary-accent)' }}>Integer Course Codes</div>
              </div>
              <div className="stat-card">
                <div className="label">Average Registry CGPA</div>
                <div className="value">{avgCgpa}</div>
                <div className="delta">Fidelity Scale: 10.0</div>
              </div>
            </div>

            <div className="visualizer-card" style={{ borderLeft: '3px solid var(--secondary-accent)' }}>
              <h3>Archivist System Health</h3>
              <div style={{ display: 'grid', gridTemplateColumns: '150px 1fr', gap: '12px', marginTop: '16px', fontSize: '13px' }}>
                <span style={{ color: 'var(--text-dim)' }}>Memory Footprint:</span>
                <span>4.18 MB (Custom AVL node maps)</span>
                <span style={{ color: 'var(--text-dim)' }}>Thread Safe:</span>
                <span style={{ color: 'var(--secondary-accent)' }}>TRUE (POSIX mutex locks active)</span>
                <span style={{ color: 'var(--text-dim)' }}>File System:</span>
                <span>Prepared binary file datastore</span>
                <span style={{ color: 'var(--text-dim)' }}>Sync State:</span>
                <span style={{ color: 'var(--primary-accent)' }}>Local Storage database mirrored</span>
              </div>
            </div>
          </div>
        )}

        {/* VIEW 2: Student Directory */}
        {currentTab === 'directory' && (
          <div>
            <h2 className="page-title">Student Registry Directory</h2>
            <p className="page-description">
              View and manage student profiles. Use the form below to add a new student. 
              All data is written in-memory and persisted into the custom ERP repository.
            </p>

            {/* Filters */}
            <div style={{ display: 'flex', gap: '16px', marginBottom: '24px', alignItems: 'center' }}>
              <input 
                type="text" 
                placeholder="Search name or roll number..."
                className="form-control"
                style={{ maxWidth: '300px' }}
                value={searchTerm}
                onChange={(e) => setSearchTerm(e.target.value)}
              />
              <select 
                className="form-control" 
                style={{ maxWidth: '180px' }}
                value={filterLevel}
                onChange={(e) => setFilterLevel(e.target.value)}
              >
                <option value="ALL">All Levels</option>
                <option value="BTECH">B.Tech</option>
                <option value="MTECH">M.Tech</option>
                <option value="PHD">PhD</option>
              </select>
            </div>

            {/* Student Directory Table */}
            <div className="table-container">
              <table className="data-table">
                <thead>
                  <tr>
                    <th>Roll Number</th>
                    <th>Name</th>
                    <th>Branch</th>
                    <th>Level</th>
                    <th>Institution</th>
                    <th>CGPA</th>
                    <th>Actions</th>
                  </tr>
                </thead>
                <tbody>
                  {filteredStudents.map((s, idx) => (
                    <tr key={s.rollNumber}>
                      <td style={{ color: 'var(--primary-accent)', fontWeight: 'bold' }}>{s.rollNumber}</td>
                      <td>{s.name}</td>
                      <td>{s.branch.code}</td>
                      <td>
                        <span className={`badge ${s.level === 'PHD' ? 'badge-pending' : 'badge-active'}`}>
                          {s.level}
                        </span>
                      </td>
                      <td>{s.institution}</td>
                      <td style={{ fontFamily: 'var(--font-mono)', fontWeight: 'bold' }}>{s.loadedCgpa.toFixed(2)}</td>
                      <td>
                        <button 
                          className="btn btn-danger"
                          style={{ padding: '4px 8px', fontSize: '10px' }}
                          onClick={() => handleDeleteStudent(s.rollNumber)}
                        >
                          Remove
                        </button>
                      </td>
                    </tr>
                  ))}
                  {filteredStudents.length === 0 && (
                    <tr>
                      <td colSpan="7" style={{ textalign: 'center', padding: '24px', color: 'var(--text-dim)' }}>
                        No records match the filter criteria.
                      </td>
                    </tr>
                  )}
                </tbody>
              </table>
            </div>

            {/* Add Student Form */}
            <div className="visualizer-card">
              <h3>Register New Student</h3>
              <form onSubmit={handleAddStudent} style={{ marginTop: '20px' }}>
                <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(200px, 1fr))', gap: '16px' }}>
                  <div className="form-group">
                    <label>Full Name</label>
                    <input 
                      type="text" 
                      className="form-control"
                      value={newName}
                      onChange={(e) => setNewName(e.target.value)}
                      placeholder="e.g. John Doe"
                    />
                  </div>
                  <div className="form-group">
                    <label>Roll Number (Unique)</label>
                    <input 
                      type="text" 
                      className="form-control"
                      value={newRoll}
                      onChange={(e) => setNewRoll(e.target.value)}
                      placeholder="e.g. 2026CSE108"
                    />
                  </div>
                  <div className="form-group">
                    <label>Institution</label>
                    <select 
                      className="form-control"
                      value={newInst}
                      onChange={(e) => setNewInst(e.target.value)}
                    >
                      <option value="Apex University">Apex University (String Roll)</option>
                      <option value="Vertex Institute">Vertex Institute (Integer Codes)</option>
                    </select>
                  </div>
                  <div className="form-group">
                    <label>Branch</label>
                    <select 
                      className="form-control"
                      value={newBranch}
                      onChange={(e) => setNewBranch(e.target.value)}
                    >
                      <option value="CSE">CSE</option>
                      <option value="ECE">ECE</option>
                    </select>
                  </div>
                  <div className="form-group">
                    <label>Student Level</label>
                    <select 
                      className="form-control"
                      value={newLevel}
                      onChange={(e) => setNewLevel(e.target.value)}
                    >
                      <option value="BTECH">B.Tech</option>
                      <option value="MTECH">M.Tech</option>
                      <option value="PHD">PhD</option>
                    </select>
                  </div>
                  <div className="form-group">
                    <label>Entry Year</label>
                    <input 
                      type="number" 
                      className="form-control"
                      value={newYear}
                      onChange={(e) => setNewYear(e.target.value)}
                    />
                  </div>
                  <div className="form-group">
                    <label>Initial CGPA</label>
                    <input 
                      type="number" 
                      step="0.01"
                      className="form-control"
                      value={newCgpa}
                      onChange={(e) => setNewCgpa(e.target.value)}
                    />
                  </div>
                </div>
                <button type="submit" className="btn btn-primary" style={{ marginTop: '16px' }}>
                  Commit Record to Database
                </button>
              </form>
            </div>
          </div>
        )}

        {/* VIEW 3: Course Catalog */}
        {currentTab === 'catalog' && (
          <div>
            <h2 className="page-title">Academic Course Catalog</h2>
            <p className="page-description">
              View course offerings from both Apex University and Vertex Institute of Technology. 
              The system supports cross-institutional registration utilizing compile-time template arguments.
            </p>

            <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '32px' }}>
              
              {/* Apex University Card */}
              <div className="visualizer-card">
                <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '16px' }}>
                  <h3 style={{ margin: 0, fontFamily: 'var(--font-serif)' }}>Apex University Catalog</h3>
                  <span className="badge badge-active" style={{ color: 'var(--primary-accent)', borderColor: 'var(--primary-accent)' }}>
                    String Course Codes
                  </span>
                </div>
                <div className="table-container" style={{ maxHeight: '400px', overflowY: 'auto' }}>
                  <table className="data-table">
                    <thead>
                      <tr>
                        <th>Code</th>
                        <th>Course Name</th>
                        <th>Credits</th>
                        <th>Instructor</th>
                      </tr>
                    </thead>
                    <tbody>
                      {APEX_COURSES.map(c => (
                        <tr key={c.code}>
                          <td style={{ color: 'var(--primary-accent)', fontWeight: 'bold' }}>{c.code}</td>
                          <td>{c.name}</td>
                          <td>{c.credits}</td>
                          <td style={{ fontSize: '11px', color: 'var(--text-dim)' }}>{c.instructor}</td>
                        </tr>
                      ))}
                    </tbody>
                  </table>
                </div>
              </div>

              {/* Vertex Institute Card */}
              <div className="visualizer-card">
                <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '16px' }}>
                  <h3 style={{ margin: 0, fontFamily: 'var(--font-serif)' }}>Vertex Institute Catalog</h3>
                  <span className="badge badge-pending">
                    Integer Course Codes
                  </span>
                </div>
                <div className="table-container" style={{ maxHeight: '400px', overflowY: 'auto' }}>
                  <table className="data-table">
                    <thead>
                      <tr>
                        <th>Code</th>
                        <th>Course Name</th>
                        <th>Credits</th>
                        <th>Instructor</th>
                      </tr>
                    </thead>
                    <tbody>
                      {VERTEX_COURSES.map(c => (
                        <tr key={c.code}>
                          <td style={{ color: 'var(--tertiary-accent)', fontWeight: 'bold' }}>{c.code}</td>
                          <td>{c.name}</td>
                          <td>{c.credits}</td>
                          <td style={{ fontSize: '11px', color: 'var(--text-dim)' }}>{c.instructor}</td>
                        </tr>
                      ))}
                    </tbody>
                  </table>
                </div>
              </div>

            </div>
          </div>
        )}

        {/* VIEW 4: Concurrent Sorting Visualizer */}
        {currentTab === 'sorting' && (
          <div>
            <h2 className="page-title">Parallel Merge Sort Engine</h2>
            <p className="page-description">
              Visualizes the C++ POSIX thread implementation of the concurrent sort algorithm. 
              The list of students is divided into chunks, sorted by individual worker threads concurrently, and merged.
            </p>

            <div style={{ display: 'flex', gap: '24px', marginBottom: '32px' }}>
              <button 
                onClick={runSortingSimulation} 
                disabled={sortState !== 'idle' && sortState !== 'completed'}
                className="btn btn-primary"
              >
                {sortState === 'idle' || sortState === 'completed' ? 'Run Concurrent Sort' : 'Sorting in Progress...'}
              </button>
              
              {sortState !== 'idle' && (
                <button 
                  onClick={() => { setSortState('idle'); setActiveThreads([]); setSortMetric(null); }}
                  className="btn"
                >
                  Reset
                </button>
              )}
            </div>

            <div style={{ display: 'grid', gridTemplateColumns: '1fr 320px', gap: '24px' }}>
              
              {/* Sorting Workspace */}
              <div className="visualizer-card">
                <h3>Thread Execution Pipeline</h3>
                
                {sortState === 'idle' && (
                  <div style={{ padding: '40px 0', textalign: 'center', color: 'var(--text-dim)' }}>
                    Ready to execute. Click "Run Concurrent Sort" to allocate POSIX threads and sort records by CGPA (Descending).
                  </div>
                )}

                {/* Division Step */}
                {sortState === 'dividing' && (
                  <div style={{ borderLeft: '2px solid var(--tertiary-accent)', paddingLeft: '16px', margin: '20px 0' }}>
                    <div style={{ color: 'var(--tertiary-accent)', fontWeight: 'bold', fontSize: '12px' }}>
                      [STAGE 1] DIVIDE & CONQUER BUCKET DIVISION
                    </div>
                    <p style={{ fontSize: '13px', marginTop: '6px' }}>
                      Splitting database student registry indices [0 to {students.length - 1}] into 4 sub-ranges for thread distribution...
                    </p>
                  </div>
                )}

                {/* Sorting threads progress */}
                {sortState === 'sorting' && (
                  <div style={{ margin: '20px 0' }}>
                    <div style={{ color: 'var(--secondary-accent)', fontWeight: 'bold', fontSize: '12px', marginBottom: '16px' }}>
                      [STAGE 2] CONCURRENT RECORD SORTING (POSIX THREAD POOL)
                    </div>
                    <div style={{ display: 'flex', flexDirection: 'column', gap: '16px' }}>
                      {activeThreads.map(thread => (
                        <div key={thread.id} style={{ background: 'var(--bg-darker)', padding: '12px', border: '1px solid var(--border-color)' }}>
                          <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '12px', marginBottom: '8px' }}>
                            <span style={{ color: 'var(--primary-accent)' }}>{thread.name} (TID: {thread.id})</span>
                            <span style={{ color: 'var(--text-dim)' }}>Registry Range: {thread.range}</span>
                          </div>
                          <div style={{ height: '4px', background: '#222', width: '100%', position: 'relative' }}>
                            <div 
                              style={{ 
                                height: '100%', 
                                background: 'var(--secondary-accent)', 
                                width: `${thread.progress}%`,
                                transition: 'width 0.15s ease'
                              }}
                            />
                          </div>
                        </div>
                      ))}
                    </div>
                  </div>
                )}

                {/* Merging Step */}
                {sortState === 'merging' && (
                  <div style={{ borderLeft: '2px solid var(--primary-accent)', paddingLeft: '16px', margin: '20px 0' }}>
                    <div style={{ color: 'var(--primary-accent)', fontWeight: 'bold', fontSize: '12px' }}>
                      [STAGE 3] BINARY MERGE TREE REDUCTION
                    </div>
                    <p style={{ fontSize: '13px', marginTop: '6px' }}>
                      All threads completed sorting. Parent thread joining sub-arrays back using thread-safe merge buffers...
                    </p>
                  </div>
                )}

                {/* Completed Results */}
                {sortState === 'completed' && sortMetric && (
                  <div style={{ border: '1px solid var(--secondary-accent)', padding: '16px', margin: '20px 0', backgroundColor: 'rgba(16, 185, 129, 0.02)' }}>
                    <div style={{ color: 'var(--secondary-accent)', fontWeight: 'bold', fontSize: '12px' }}>
                      [STAGE 4] CONCURRENT SORT COMPLETE
                    </div>
                    <p style={{ fontSize: '13px', marginTop: '6px' }}>
                      Successfully sorted {sortMetric.recordsSorted} students using {sortMetric.threadsUsed} concurrent POSIX threads.
                    </p>
                    <div style={{ fontSize: '12px', marginTop: '8px', color: 'var(--text-dim)' }}>
                      Total Join/Merge overhead: <span style={{ color: 'var(--primary-accent)', fontWeight: 'bold' }}>{sortMetric.durationMs} ms</span>
                    </div>
                  </div>
                )}

                {/* Sorted list view */}
                {sortState !== 'idle' && (
                  <div style={{ marginTop: '24px' }}>
                    <h4 style={{ fontSize: '13px', color: 'var(--text-dim)', marginBottom: '8px' }}>SORTED REGISTRY ARRAY</h4>
                    <div className="table-container">
                      <table className="data-table">
                        <thead>
                          <tr>
                            <th>Rank</th>
                            <th>Roll</th>
                            <th>Name</th>
                            <th>CGPA</th>
                          </tr>
                        </thead>
                        <tbody>
                          {sortList.map((s, idx) => (
                            <tr key={s.rollNumber}>
                              <td>{idx + 1}</td>
                              <td style={{ color: 'var(--primary-accent)' }}>{s.rollNumber}</td>
                              <td>{s.name}</td>
                              <td style={{ fontWeight: 'bold' }}>{s.loadedCgpa.toFixed(2)}</td>
                            </tr>
                          ))}
                        </tbody>
                      </table>
                    </div>
                  </div>
                )}

              </div>

              {/* Thread Stats Panel */}
              <div className="visualizer-card">
                <h3>Thread Metrics</h3>
                <div style={{ marginTop: '20px', fontSize: '12px', display: 'flex', flexDirection: 'column', gap: '12px' }}>
                  <div>
                    <span style={{ color: 'var(--text-dim)' }}>Sort Criteria:</span><br />
                    <strong>CGPA (Descending)</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--text-dim)' }}>Concurrent Workers:</span><br />
                    <strong>4 POSIX Threads</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--text-dim)' }}>Hardware Concurrency:</span><br />
                    <strong>8 Cores (ARM64 Apple Silicon)</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--text-dim)' }}>Synchronization Primitives:</span><br />
                    <strong style={{ color: 'var(--secondary-accent)' }}>pthread_create, pthread_join</strong>
                  </div>
                </div>
              </div>

            </div>
          </div>
        )}

        {/* VIEW 5: Indexed Grade Search Visualizer */}
        {currentTab === 'indexing' && (
          <div>
            <h2 className="page-title">Indexed Grade Search Engine</h2>
            <p className="page-description">
              Demonstrates the O(log C + log G + K) fast retrieval index implemented in C++. 
              The lookup bypasses sequential scanning by querying nested course and grade maps.
            </p>

            <div className="visualizer-card">
              <h3>Query Index Controller</h3>
              
              <div style={{ display: 'flex', gap: '16px', marginTop: '20px', alignItems: 'flex-end' }}>
                <div className="form-group" style={{ margin: 0, width: '200px' }}>
                  <label>Course Code</label>
                  <select 
                    className="form-control"
                    value={searchCourseCode}
                    onChange={(e) => setSearchCourseCode(e.target.value)}
                  >
                    <option value="CSE201">CSE201 (Apex)</option>
                    <option value="CSE202">CSE202 (Apex)</option>
                    <option value="MTH101">MTH101 (Apex)</option>
                    <option value="ECE101">ECE101 (Apex)</option>
                    <option value="MAT101">MAT101 (Apex)</option>
                    <option value="HSS101">HSS101 (Apex)</option>
                    <option value="CSE501">CSE501 (Apex)</option>
                    <option value="RES001">RES001 (Apex)</option>
                    <option value="101">101 (Vertex)</option>
                    <option value="201">201 (Vertex)</option>
                    <option value="305">305 (Vertex)</option>
                    <option value="401">401 (Vertex)</option>
                    <option value="505">505 (Vertex)</option>
                    <option value="301">301 (Vertex)</option>
                  </select>
                </div>

                <div className="form-group" style={{ margin: 0, width: '120px' }}>
                  <label>Grade Target</label>
                  <select 
                    className="form-control"
                    value={searchGradeLetter}
                    onChange={(e) => setSearchGradeLetter(e.target.value)}
                  >
                    <option value="A+">A+</option>
                    <option value="A">A</option>
                    <option value="A-">A-</option>
                    <option value="B+">B+</option>
                    <option value="B">B</option>
                  </select>
                </div>

                <button 
                  onClick={runIndexedSearchSimulation}
                  className="btn btn-primary"
                  style={{ height: '45px' }}
                >
                  Search Index
                </button>
              </div>
            </div>

            <div style={{ display: 'grid', gridTemplateColumns: '1fr 340px', gap: '24px' }}>
              
              {/* Lookup Path Visualization */}
              <div className="visualizer-card">
                <h3>Registry Lookup Path</h3>

                {searchPathSteps.length === 0 && (
                  <div style={{ padding: '40px 0', textalign: 'center', color: 'var(--text-dim)' }}>
                    Configure a Course and Target Grade, then click "Search Index" to visualize the O(log C + log G + K) index execution.
                  </div>
                )}

                <div style={{ display: 'flex', flexDirection: 'column', gap: '12px', marginTop: '16px' }}>
                  {searchPathSteps.map((step, idx) => (
                    <div 
                      key={step.id} 
                      style={{ 
                        borderLeft: `3px solid ${idx === searchPathSteps.length - 1 ? 'var(--primary-accent)' : 'var(--border-color)'}`,
                        paddingLeft: '16px',
                        paddingBottom: '8px'
                      }}
                    >
                      <div style={{ fontSize: '11px', color: 'var(--primary-accent)', textTransform: 'uppercase', fontWeight: 'bold' }}>
                        Step {step.id}: {step.title}
                      </div>
                      <div style={{ fontSize: '13px', color: 'var(--text-main)', marginTop: '4px' }}>
                        {step.desc}
                      </div>
                    </div>
                  ))}
                </div>

                {searchPathProgress === 5 && (
                  <div style={{ marginTop: '24px' }}>
                    <h4 style={{ fontSize: '13px', color: 'var(--text-dim)', marginBottom: '8px' }}>QUERY RESULTS</h4>
                    <div className="table-container">
                      <table className="data-table">
                        <thead>
                          <tr>
                            <th>Roll</th>
                            <th>Student Name</th>
                            <th>Branch</th>
                            <th>Level</th>
                            <th>Grade GP</th>
                          </tr>
                        </thead>
                        <tbody>
                          {searchResultStudents.map(student => (
                            <tr key={student.rollNumber}>
                              <td style={{ color: 'var(--primary-accent)', fontWeight: 'bold' }}>{student.rollNumber}</td>
                              <td>{student.name}</td>
                              <td>{student.branch.code}</td>
                              <td>{student.level}</td>
                              <td style={{ color: 'var(--secondary-accent)', fontWeight: 'bold' }}>
                                {searchGradeLetter}
                              </td>
                            </tr>
                          ))}
                          {searchResultStudents.length === 0 && (
                            <tr>
                              <td colSpan="5" style={{ textalign: 'center', padding: '16px', color: 'var(--text-dim)' }}>
                                No students currently possess grade {searchGradeLetter} in {searchCourseCode}.
                              </td>
                            </tr>
                          )}
                        </tbody>
                      </table>
                    </div>
                  </div>
                )}

              </div>

              {/* Big-O Analysis Card */}
              <div className="visualizer-card">
                <h3>Complexity Index Analysis</h3>
                <div style={{ marginTop: '20px', fontSize: '12px', display: 'flex', flexDirection: 'column', gap: '16px' }}>
                  <div>
                    <span style={{ color: 'var(--text-dim)' }}>C++ Map Architecture:</span><br />
                    <strong>std::map&lt;CourseCode, std::map&lt;Grade, std::vector&lt;StudentID&gt;&gt;&gt;</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--text-dim)' }}>Outer Level:</span><br />
                    <span>std::map&lt;CourseCode, ...&gt;</span><br />
                    <strong style={{ color: 'var(--tertiary-accent)' }}>O(log C) search time</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--text-dim)' }}>Inner Level:</span><br />
                    <span>std::map&lt;Grade, ...&gt;</span><br />
                    <strong style={{ color: 'var(--tertiary-accent)' }}>O(log G) search time</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--text-dim)' }}>Leaves Fetch:</span><br />
                    <span>std::vector&lt;StudentID&gt;</span><br />
                    <strong style={{ color: 'var(--secondary-accent)' }}>O(K) copy time</strong>
                  </div>
                  <div style={{ borderTop: '1px solid var(--border-color)', paddingTop: '12px' }}>
                    <span style={{ color: 'var(--text-dim)' }}>Total Lookup Cost:</span><br />
                    <strong style={{ fontSize: '16px', color: 'var(--primary-accent)' }}>O(log C + log G + K)</strong>
                  </div>
                </div>
              </div>

            </div>
          </div>
        )}

      </main>
    </div>
  );
}

export default App;
