import React, { useState, useEffect } from 'react';
import './App.css';

// Initial Mock Students Database with high-fidelity portraits & academic records
const INITIAL_STUDENTS = [
  {
    name: "Elias Vance",
    rollNumber: "2023CSE101",
    branch: { code: "CSE", name: "Computer Science & Engineering", dept: "Engineering" },
    level: "BTECH",
    startingYear: 2023,
    loadedCgpa: 9.3,
    institution: "Apex Campus",
    creditsEarned: 84,
    totalCredits: 120,
    status: "Optimal",
    portraitUrl: "https://lh3.googleusercontent.com/aida-public/AB6AXuDjWCZYqWSP8XHHmLLIKk0jp8ClM5wZ_2RXhfGigBAy4j7EDR4t8ChkIoS8hiKKdhb7JarLj033NX9Zwco-xWsRRrCs9hQpDxVGJ22XTtq0_N31J-ODEh2b37meMw9yjtdmpfvQP_6ZT6-pB-DBrf5MB1o_bM-4sU0Lu_2pB2WwQRUh2-vAn3P8hsysnVZz-r5xPGK4udRngS_h-oD10vKkvwY91nstyjmtFN2ISWpi6NesTum9ocHuyIfvpW2mrLVTvPCayJ0Z5m-w",
    currentCourses: [
      { code: "CSE201", name: "Advanced Algorithmic Structures", credits: 4, instructor: "Prof. E. Vance" },
      { code: "CSE202", name: "Database Systems", credits: 4, instructor: "Dr. A. Chen" },
      { code: "MTH101", name: "Applied Mathematics", credits: 3, instructor: "Prof. L. Sterling" }
    ],
    grades: { "CSE201": "A", "CSE202": "A-", "MTH101": "A+" }
  },
  {
    name: "Clara Lin",
    rollNumber: "2023CSE102",
    branch: { code: "CSE", name: "Computer Science & Engineering", dept: "Engineering" },
    level: "BTECH",
    startingYear: 2023,
    loadedCgpa: 8.8,
    institution: "Vertex Campus",
    creditsEarned: 56,
    totalCredits: 120,
    status: "Review Required",
    portraitUrl: "https://lh3.googleusercontent.com/aida-public/AB6AXuB6ujTwtsUU2pvt3DnMjMjemWdZhfHDNuVgHHfkJ3Fyx-8__IKo7caiITRVNVVlo_r5ke6EZFPo3aPlkyIWoDPmE8EN_UXysM0zXOvQM-aRkrUV2ry6h6PWu49ZVbyQOU70MzAOVbp4_VTxOAxjiiRPtwOE0-XA_pgi7KODYHnwnGm9SForGD7dFiMXz9O1F5u2-7qMnm53BvpCJX6MyMk6uNuw37O4JFXi5xph1EmoB6lpotDf9528X6-O4QNqsbjcWKjkpQuA_84n",
    currentCourses: [
      { code: "101", name: "Introduction to Computing", credits: 4, instructor: "Prof. S. Banerjee" },
      { code: "201", name: "Advanced Algorithmic Structures", credits: 4, instructor: "Prof. Mausam" },
      { code: "305", name: "Database Systems", credits: 4, instructor: "Prof. S. Sudarshan" }
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
    institution: "Apex Campus",
    creditsEarned: 80,
    totalCredits: 120,
    status: "Optimal",
    portraitUrl: "https://lh3.googleusercontent.com/aida-public/AB6AXuDFxrWHNMKuszWr5RacnaBxVbmzvtlqy1n-LqOpI-alnY0rrtyxqx_y3fVUCMHCegr7mmYYNZOuEyWIvxgN6vIkR32erPTvKfpBSL82VS4In3Oe_SXTkqXFD-PQl4_d8f-23JBgw780xrukwfs6BqXdjoyGwvd4o86rXCEB7_MJmPNVfnhFh0dL5vhOjazsRhiqqolPXYhISJLcamHrJ70MpK7gX5bxbD5X4Vf-XrVWcnGSX_arVOFXQLrhFRev89Qj0CtAH3ypESEn",
    currentCourses: [
      { code: "401", name: "Machine Learning", credits: 4, instructor: "Prof. P. Singla" },
      { code: "505", name: "Computer Vision", credits: 3, instructor: "Prof. C. Arora" },
      { code: "301", name: "Operating Systems", credits: 4, instructor: "Prof. A. Bhatia" }
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
    institution: "Apex Campus",
    creditsEarned: 44,
    totalCredits: 120,
    status: "Optimal",
    portraitUrl: "",
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
    institution: "Apex Campus",
    creditsEarned: 32,
    totalCredits: 64,
    status: "Optimal",
    portraitUrl: "",
    currentCourses: [
      { code: "CSE501", name: "Advanced Algorithmic Structures", credits: 4, instructor: "Dr. Sanjit Kaul" },
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
    institution: "Apex Campus",
    creditsEarned: 16,
    totalCredits: 32,
    status: "Optimal",
    portraitUrl: "",
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
  { code: "CSE201", name: "Advanced Algorithmic Structures", credits: 4, instructor: "Prof. E. Vance" },
  { code: "CSE202", name: "Database Management Systems", credits: 4, instructor: "Dr. Tanmoy Chakraborty", prereqs: "CSE102" },
  { code: "CSE343", name: "Machine Learning", credits: 4, instructor: "Dr. Ganesh Bagler", prereqs: "CSE102" },
  { code: "CSE501", name: "Advanced Algorithmic Structures", credits: 4, instructor: "Dr. Sanjit Kaul" }
];

const VERTEX_COURSES = [
  { code: "101", name: "Introduction to Computing", credits: 4, instructor: "Prof. S. Banerjee" },
  { code: "201", name: "Advanced Algorithmic Structures", credits: 4, instructor: "Prof. Mausam", prereqs: "101" },
  { code: "202", name: "Computer Architecture", credits: 4, instructor: "Prof. Anshul Kumar" },
  { code: "305", name: "Database Systems", credits: 4, instructor: "Prof. S. Sudarshan", prereqs: "101" },
  { code: "401", name: "Artificial Intelligence and Machine Learning", credits: 4, instructor: "Prof. Parag Singla", prereqs: "201" },
  { code: "501", name: "Advanced Computer Networks", credits: 4, instructor: "Prof. Huzur Saran" }
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

  // Form states for registering a student
  const [newName, setNewName] = useState('');
  const [newRoll, setNewRoll] = useState('');
  const [newBranch, setNewBranch] = useState('CSE');
  const [newLevel, setNewLevel] = useState('BTECH');
  const [newYear, setNewYear] = useState(2026);
  const [newCgpa, setNewCgpa] = useState(8.0);
  const [newInst, setNewInst] = useState('Apex Campus');

  // Search/Filter states
  const [searchTerm, setSearchTerm] = useState('');
  const [filterLevel, setFilterLevel] = useState('ALL');
  const [filterCohort, setFilterCohort] = useState('ALL');
  const [filterDept, setFilterDept] = useState('');
  const [filterStanding, setFilterStanding] = useState({
    Optimal: true,
    ReviewRequired: true,
    Suspension: true
  });

  // State-controlled inline record deletion
  const [deletingRoll, setDeletingRoll] = useState(null);

  // Export Manifest animation state
  const [exporting, setExporting] = useState(false);
  const [showExportSuccess, setShowExportSuccess] = useState(false);

  // Ingestion Stream Live Simulation Logs
  const [ingestionLogs, setIngestionLogs] = useState([
    "> Validated academic registry records for the current term.",
    "> Synchronized course catalog entries for all student profiles.",
    "> Reconciled enrollment ledger data across admissions campus offices.",
    "> Verified fiscal archive ledgers for active research grants.",
    "> System status: Fully reconciled and synchronized."
  ]);

  // Live log simulation
  useEffect(() => {
    const activities = [
      "Validated academic standing for registry profile Elias Vance.",
      "Reconciled enrollment catalog indices for CSE201.",
      "Mirrored session variables across admissions registries.",
      "Archived fiscal transactions for research grants.",
      "Verified thesis tracking updates for PhD ledger.",
      "Refreshed course mappings for Vertex Campus catalog.",
      "Updated academic rank index cache for current semester.",
      "Completed standard registry data safety check."
    ];
    const interval = setInterval(() => {
      const randomActivity = activities[Math.floor(Math.random() * activities.length)];
      const timestamp = new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit', second: '2-digit' });
      setIngestionLogs(prev => {
        const next = [...prev, `> [${timestamp}] ${randomActivity}`];
        if (next.length > 7) {
          next.shift();
        }
        return next;
      });
    }, 4500);
    return () => clearInterval(interval);
  }, []);

  // Sorting Visualizer State
  const [sortList, setSortList] = useState([]);
  const [sortState, setSortState] = useState('idle'); // idle, dividing, sorting, merging, completed
  const [activeThreads, setActiveThreads] = useState([]);
  const [sortMetric, setSortMetric] = useState(null);

  // Indexed Search Visualizer State
  const [searchCourseCode, setSearchCourseCode] = useState('CSE201');
  const [searchGradeLetter, setSearchGradeLetter] = useState('A+');
  const [searchPathSteps, setSearchPathSteps] = useState([]);
  const [searchPathProgress, setSearchPathProgress] = useState(0);
  const [searchResultStudents, setSearchResultStudents] = useState([]);

  // Handle adding student
  const handleAddStudent = (e) => {
    e.preventDefault();
    if (!newName || !newRoll) {
      alert("Please provide the complete Student Name and Unique Registry Number.");
      return;
    }
    const exists = students.some(s => s.rollNumber.toLowerCase() === newRoll.toLowerCase());
    if (exists) {
      alert("A student with this Registry Number is already registered!");
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
      creditsEarned: newLevel === 'BTECH' ? 80 : newLevel === 'MTECH' ? 40 : 16,
      totalCredits: newLevel === 'BTECH' ? 120 : newLevel === 'MTECH' ? 64 : 32,
      status: parseFloat(newCgpa) >= 8.5 ? "Optimal" : parseFloat(newCgpa) >= 5.0 ? "Review Required" : "Suspension",
      portraitUrl: "",
      currentCourses: [],
      grades: {}
    };

    setStudents([...students, newStudent]);
    setNewName('');
    setNewRoll('');

    // Highlight feedback
    alert(`Successfully registered student profile for ${newName}.`);
  };

  // Delete student record
  const commitDeleteRecord = (roll) => {
    setStudents(students.filter(s => s.rollNumber !== roll));
    setDeletingRoll(null);
  };

  // Trigger scroll and focus to registration form
  const navigateToNewRecord = () => {
    setCurrentTab('directory');
    setTimeout(() => {
      const formEl = document.getElementById('register-student-form');
      if (formEl) {
        formEl.scrollIntoView({ behavior: 'smooth' });
        const inputEl = formEl.querySelector('input');
        if (inputEl) inputEl.focus();
      }
    }, 100);
  };

  // Handle export manifest animation
  const runExportManifest = () => {
    setExporting(true);
    setTimeout(() => {
      setExporting(false);
      setShowExportSuccess(true);
      setTimeout(() => setShowExportSuccess(false), 3000);
    }, 1500);
  };

  // Run Parallel Rankings Analysis Simulation
  const runSortingSimulation = () => {
    if (students.length === 0) return;
    setSortState('dividing');
    setSortList([...students]);
    setSortMetric(null);
    
    // Step 1: Divide list into cohorts
    setTimeout(() => {
      setSortState('sorting');
      setActiveThreads([
        { id: "runner-a", name: "Analysis Runner Alpha", progress: 0, range: "Cohort A (Admissions 2023)" },
        { id: "runner-b", name: "Analysis Runner Beta", progress: 0, range: "Cohort B (Admissions 2024)" },
        { id: "runner-c", name: "Analysis Runner Gamma", progress: 0, range: "Cohort C (Admissions 2025)" },
        { id: "runner-d", name: "Analysis Runner Delta", progress: 0, range: "Cohort D (Admissions 2026)" }
      ]);
      
      // Step 2: Parallel sorting progress
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
                durationMs: (8.4 + Math.random() * 2).toFixed(1),
                recordsSorted: students.length
              });
            }, 1000);
          }, 800);
        }
      }, 250);
    }, 1000);
  };

  // Grade Index Search Simulation
  const runIndexedSearchSimulation = () => {
    setSearchPathProgress(0);
    setSearchPathSteps([]);
    setSearchResultStudents([]);

    const steps = [
      { id: 1, title: "Ingest Search Request Parameters", desc: `Initiated target query for course [${searchCourseCode}] and grade level [${searchGradeLetter}].` },
      { id: 2, title: "Primary Index Verification", desc: `Locating course catalog index for key "${searchCourseCode}"... Match found.` },
      { id: 3, title: "Secondary Index Verification", desc: `Locating sub-grade mapping records for indicator "${searchGradeLetter}"... Matches found.` },
      { id: 4, title: "Student Record Resolution", desc: "Compiling unique identity tags for all matching students in the sub-grade registry branch." },
      { id: 5, title: "Profile Credentials Retrieval", desc: "Loading academic standings and full records from primary university archives." }
    ];

    let currentStep = 0;
    const runStep = () => {
      if (currentStep < steps.length) {
        setSearchPathProgress(currentStep + 1);
        setSearchPathSteps(prev => [...prev, steps[currentStep]]);
        currentStep++;
        setTimeout(runStep, 700);
      } else {
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
  const apexCount = students.filter(s => s.institution === 'Apex Campus').length;
  const vertexCount = students.filter(s => s.institution === 'Vertex Campus').length;

  // Filter students for ledger display
  const filteredStudents = students.filter(s => {
    const matchesSearch = s.name.toLowerCase().includes(searchTerm.toLowerCase()) || 
                          s.rollNumber.toLowerCase().includes(searchTerm.toLowerCase());
    
    // Cohort filter
    let matchesCohort = true;
    if (filterCohort !== 'ALL') {
      matchesCohort = s.startingYear.toString() === filterCohort;
    }
    
    // Department filter
    let matchesDept = true;
    if (filterDept.trim() !== '') {
      matchesDept = s.branch.name.toLowerCase().includes(filterDept.toLowerCase()) ||
                    s.branch.code.toLowerCase().includes(filterDept.toLowerCase());
    }

    // Level filter
    const matchesLevel = filterLevel === 'ALL' || s.level === filterLevel;
    
    // Status standing matching
    let matchesStanding = false;
    if (s.status === "Optimal" && filterStanding.Optimal) matchesStanding = true;
    if (s.status === "Review Required" && filterStanding.ReviewRequired) matchesStanding = true;
    if (s.status === "Suspension" && filterStanding.Suspension) matchesStanding = true;
    if (!s.status) matchesStanding = true; // Fallback

    return matchesSearch && matchesCohort && matchesDept && matchesLevel && matchesStanding;
  });

  return (
    <div className="app-container">
      {/* Atmospheric Background Blur Shapes */}
      <div className="atmospheric-bg">
        <div className="blur-shape blur-shape-1" aria-hidden="true"></div>
        <div className="blur-shape blur-shape-2" aria-hidden="true"></div>
      </div>

      {/* Navigation Panel (Desktop SideNavBar) */}
      <aside className="nav-panel">
        <div className="nav-header">
          <div style={{ display: 'flex', itemsCenter: 'center', gap: '8px', marginBottom: '8px' }}>
            <img 
              alt="University Seal" 
              className="w-10 h-10 rounded-full border border-outline-variant/30" 
              style={{ width: '40px', height: '40px', borderRadius: '50%', objectFit: 'cover' }}
              src="https://lh3.googleusercontent.com/aida-public/AB6AXuBDsjEDiLgNpZYS1_snb_y3mHlm99KP8RaURfQDF9CkwQvVHhss2b52uGo9YSIatnLoeW2HLBotgIjcekwed6kMb3L1gwycJ1PabM53tMy-F_crd11uUudQRqX0w2SGVkzduVS2ZBf4Jvf_STf0X5I0lbZz4o-tf4gmrXcDmYfskWNwM3xvRuTc1mgFu3HU5YTuJdfpPndCnv0moxLTPVbOAJ5cUQDs6LPf0Wnf2cPIM5LaZSfVbKd-Vtoosjcbvz5PeCAW6is6hxVY"
            />
            <div>
              <h1 style={{ fontSize: '18px', fontWeight: '600', color: 'var(--md-sys-color-primary)', margin: 0 }}>Apex University</h1>
              <div className="subtitle" style={{ margin: 0 }}>Institutional ERP v4.2</div>
            </div>
          </div>
        </div>

        <button 
          onClick={navigateToNewRecord}
          className="mb-lg w-full bg-primary-container text-on-primary-container py-2 rounded-lg font-label-md hover:opacity-90 transition-opacity flex items-center justify-center gap-2"
          style={{ width: '100%', padding: '10px', borderRadius: '12px', border: 'none', backgroundColor: 'var(--md-sys-color-primary-container)', color: 'var(--md-sys-color-on-primary-container)', cursor: 'pointer', display: 'flex', alignItems: 'center', justifyContent: 'center', gap: '8px', marginBottom: '24px' }}
        >
          <span className="material-symbols-outlined" style={{ fontSize: '18px' }}>add</span>
          New Student Record
        </button>

        <nav className="nav-menu">
          <button 
            className={`nav-item ${currentTab === 'dashboard' ? 'active' : ''}`}
            onClick={() => setCurrentTab('dashboard')}
          >
            <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'dashboard' ? "'FILL' 1" : "'FILL' 0" }}>dashboard_customize</span>
            Archival Hub
          </button>
          <button 
            className={`nav-item ${currentTab === 'directory' ? 'active' : ''}`}
            onClick={() => setCurrentTab('directory')}
          >
            <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'directory' ? "'FILL' 1" : "'FILL' 0" }}>history_edu</span>
            Student Ledger
          </button>
          <button 
            className={`nav-item ${currentTab === 'faculty' ? 'active' : ''}`}
            onClick={() => setCurrentTab('faculty')}
          >
            <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'faculty' ? "'FILL' 1" : "'FILL' 0" }}>account_balance</span>
            Faculty Registry
          </button>
          <button 
            className={`nav-item ${currentTab === 'catalog' ? 'active' : ''}`}
            onClick={() => setCurrentTab('catalog')}
          >
            <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'catalog' ? "'FILL' 1" : "'FILL' 0" }}>menu_book</span>
            Academic Catalog
          </button>
          <button 
            className={`nav-item ${currentTab === 'fiscal' ? 'active' : ''}`}
            onClick={() => setCurrentTab('fiscal')}
          >
            <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'fiscal' ? "'FILL' 1" : "'FILL' 0" }}>receipt_long</span>
            Fiscal Archives
          </button>
          <button 
            className={`nav-item ${currentTab === 'reports' ? 'active' : ''}`}
            onClick={() => setCurrentTab('reports')}
          >
            <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'reports' ? "'FILL' 1" : "'FILL' 0" }}>analytics</span>
            Institutional Reports
          </button>
          
          <div style={{ margin: '16px 0 8px 12px', fontSize: '11px', fontWeight: 'bold', color: 'var(--md-sys-color-outline)', textTransform: 'uppercase', letterSpacing: '0.08em' }}>Calculators</div>
          
          <button 
            className={`nav-item ${currentTab === 'sorting' ? 'active' : ''}`}
            onClick={() => setCurrentTab('sorting')}
          >
            <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'sorting' ? "'FILL' 1" : "'FILL' 0" }}>unfold_more</span>
            Parallel Rankings
          </button>
          <button 
            className={`nav-item ${currentTab === 'indexing' ? 'active' : ''}`}
            onClick={() => setCurrentTab('indexing')}
          >
            <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'indexing' ? "'FILL' 1" : "'FILL' 0" }}>search_insights</span>
            Grade Search Index
          </button>
        </nav>

        <div className="nav-footer">
          <p>© 2026 Apex University</p>
          <p style={{ color: 'var(--md-sys-color-secondary)' }}>• Background updates active</p>
          <p>Registry Status: Synchronized</p>
        </div>
      </aside>

      {/* Main Content Area */}
      <main className="main-content">

        {/* Global Top Header Bar */}
        <header className="header-bar">
          <div style={{ display: 'flex', alignItems: 'center', gap: '12px' }}>
            <button className="md:hidden header-btn">
              <span className="material-symbols-outlined">menu</span>
            </button>
            <div style={{ fontFamily: 'var(--font-display)', fontSize: '20px', fontWeight: 'bold', color: 'var(--md-sys-color-primary)' }}>
              {currentTab === 'dashboard' && 'Archival Hub'}
              {currentTab === 'directory' && 'Student Ledger'}
              {currentTab === 'faculty' && 'Faculty Registry'}
              {currentTab === 'catalog' && 'Academic Catalog'}
              {currentTab === 'fiscal' && 'Fiscal Archives'}
              {currentTab === 'reports' && 'Institutional Reports'}
              {currentTab === 'sorting' && 'Parallel Rankings'}
              {currentTab === 'indexing' && 'Grade Search Index'}
            </div>
          </div>

          <div className="header-actions">
            <div className="header-search">
              <span className="material-symbols-outlined" style={{ fontSize: '18px' }}>search</span>
              <input 
                type="text" 
                placeholder="Query records..." 
                value={searchTerm}
                onChange={(e) => {
                  setSearchTerm(e.target.value);
                  if (currentTab !== 'directory') {
                    setCurrentTab('directory');
                  }
                }}
              />
            </div>
            <button className="header-btn" title="View notification updates">
              <span className="material-symbols-outlined">notifications</span>
            </button>
            <button className="header-btn" title="System settings">
              <span className="material-symbols-outlined">settings</span>
            </button>
            <div className="header-avatar" title="Active Admin Session">
              <img 
                src="https://lh3.googleusercontent.com/aida-public/AB6AXuD9HQsOllCBzil4cQMgpmtvbPvzyxNSDeBB3WjXD27vHs34GFUzDJVbvBX7_3RoNDTsYGigXspf0MgYX__VtjfG6B8c4IwjxBo-b7Kot2QBbGgj1cItzhbWxxobCyLQZkqCGDzMvulRamhYAaP81E3yOqDM-3De60t4eywC7wPasIa2bjWJ42CGCMzF4irJZMNXsDmHO_PudlG3xqSGQfFWdC6Ij8bq0KiImYji29IuJm_wxEafVW5TeIvRAqx8AuOM9BKhyavnu6uX" 
                alt="Administrator Avatar" 
              />
            </div>
          </div>
        </header>
        
        {/* VIEW 1: Dashboard (Archival Hub) */}
        {currentTab === 'dashboard' && (
          <div>
            <div className="flex flex-col md:flex-row justify-between items-start md:items-end gap-md mb-xl">
              <div>
                <h2 className="page-title" style={{ fontSize: '32px', marginBottom: '4px' }}>Archival Hub</h2>
                <p className="page-description" style={{ marginBottom: 0 }}>
                  Global overview of institutional data health and real-time archival streams.
                </p>
              </div>
              <div className="flex gap-sm">
                <span className="glass-panel px-md py-xs rounded-full font-label-md text-label-md text-tertiary flex items-center gap-2" style={{ fontSize: '12px', padding: '4px 12px', borderRadius: '9999px' }}>
                  <span className="w-2 h-2 rounded-full" style={{ width: '8px', height: '8px', borderRadius: '50%', backgroundColor: 'var(--md-sys-color-secondary)' }}></span> Data Integrity: Verified
                </span>
                <span className="glass-panel px-md py-xs rounded-full font-label-md text-label-md text-tertiary flex items-center gap-2" style={{ fontSize: '12px', padding: '4px 12px', borderRadius: '9999px' }}>
                  <span className="w-2 h-2 rounded-full" style={{ width: '8px', height: '8px', borderRadius: '50%', backgroundColor: 'var(--md-sys-color-secondary)' }}></span> Sync: 12ms
                </span>
              </div>
            </div>

            <div className="dashboard-grid">
              <div className="stat-card">
                <div className="label">Total Registry Enrollment</div>
                <div className="value">{totalStudents}</div>
                <div className="delta">Active Academic Term</div>
              </div>
              <div className="stat-card">
                <div className="label">Apex Campus Students</div>
                <div className="value">{apexCount}</div>
                <div className="delta">Custom Identifier Structure</div>
              </div>
              <div className="stat-card">
                <div className="label">Vertex Campus Students</div>
                <div className="value">{vertexCount}</div>
                <div className="delta">Numeric Cataloging System</div>
              </div>
              <div className="stat-card">
                <div className="label">Average Registry GPA</div>
                <div className="value">{avgCgpa}</div>
                <div className="delta">Grading Basis: 10.0 Scale</div>
              </div>
            </div>

            <div className="grid grid-cols-1 lg:grid-cols-12 gap-lg mb-xl">
              {/* Institutional Data Map */}
              <div className="lg:col-span-8 glass-panel rounded-xl overflow-hidden flex flex-col relative h-[500px]" style={{ borderRadius: '16px', height: '500px' }}>
                <div className="p-md border-b border-outline-variant/10 flex justify-between items-center bg-surface/50 relative z-10" style={{ padding: '16px 20px', borderBottom: '1px solid rgba(196, 199, 199, 0.3)' }}>
                  <h3 className="font-headline-md text-headline-md text-on-surface flex items-center gap-2" style={{ fontSize: '18px', fontWeight: '600', margin: 0 }}>
                    <span className="material-symbols-outlined text-tertiary-container">public</span>
                    Institutional Data Map
                  </h3>
                  <span style={{ fontSize: '12px', color: 'var(--md-sys-color-on-surface-variant)' }}>Interactive nodes</span>
                </div>
                
                <div className="flex-1 data-map-bg relative overflow-hidden flex items-center justify-center">
                  {/* Decorative vintage map overlay as in design */}
                  <div 
                    className="absolute inset-0 opacity-20 pointer-events-none" 
                    style={{ 
                      backgroundImage: `url('https://lh3.googleusercontent.com/aida-public/AB6AXuBgFqiA0rEHladkVbZaBR_CEmyXv68H8_gkOsu5LzFiJpaykBMCqjOj-bMq4thJOYf5NKvXE-QMgkO519M54n1e20J2UrOVmsJqjoEhQMljx8FbsJtwr8WVeO0eCTCSSJgJFVgpDNr2litF-UHdLXGQG55vOih48idZxtjpEFX0beZlJTC7ydYtN2YrdL38DZk1AbbSCmoQPoaKSlLhhbiBcPg9AEMGti4baAIDvFxJo5H7X96Pde54TQIPEil7xIQuyB4UJlemio3u')`, 
                      backgroundSize: 'cover', 
                      backgroundPosition: 'center', 
                      mixBlendMode: 'overlay' 
                    }}
                  ></div>

                  {/* Abstract Data Nodes */}
                  <div className="absolute top-[25%] left-[20%] group">
                    <div className="w-4 h-4 rounded-full bg-tertiary-container relative z-10 border-2 border-surface cursor-pointer hover:bg-primary transition-colors" style={{ width: '16px', height: '16px', backgroundColor: 'var(--md-sys-color-tertiary)', borderRadius: '50%', border: '2px solid #ffffff' }}></div>
                    <div className="w-4 h-4 rounded-full bg-tertiary-container absolute top-0 left-0 archival-pulse" style={{ width: '16px', height: '16px', backgroundColor: 'var(--md-sys-color-tertiary)', borderRadius: '50%' }}></div>
                    <div className="absolute top-6 left-1/2 -translate-x-1/2 glass-panel px-sm py-xs rounded text-mono-data text-on-surface whitespace-nowrap pointer-events-none" style={{ fontSize: '11px', padding: '2px 6px', borderRadius: '4px' }}>Admissions Office: Synchronized</div>
                  </div>
                  
                  <div className="absolute top-[55%] left-[65%] group">
                    <div className="w-5 h-5 rounded-full bg-secondary relative z-10 border-2 border-surface cursor-pointer hover:bg-secondary-fixed transition-colors" style={{ width: '20px', height: '20px', backgroundColor: 'var(--md-sys-color-secondary)', borderRadius: '50%', border: '2px solid #ffffff' }}></div>
                    <div className="w-5 h-5 rounded-full bg-secondary absolute top-0 left-0 archival-pulse" style={{ width: '20px', height: '20px', backgroundColor: 'var(--md-sys-color-secondary)', borderRadius: '50%' }}></div>
                    <div className="absolute top-6 left-1/2 -translate-x-1/2 glass-panel px-sm py-xs rounded text-mono-data text-on-surface whitespace-nowrap pointer-events-none" style={{ fontSize: '11px', padding: '2px 6px', borderRadius: '4px' }}>Registrar Core: Processing</div>
                  </div>

                  <div className="absolute top-[30%] right-[25%] group">
                    <div className="w-3 h-3 rounded-full bg-on-surface-variant relative z-10 border-2 border-surface cursor-pointer" style={{ width: '12px', height: '12px', backgroundColor: 'var(--md-sys-color-on-surface-variant)', borderRadius: '50%', border: '2px solid #ffffff' }}></div>
                    <div className="absolute top-6 left-1/2 -translate-x-1/2 glass-panel px-sm py-xs rounded text-mono-data text-on-surface whitespace-nowrap pointer-events-none" style={{ fontSize: '11px', padding: '2px 6px', borderRadius: '4px' }}>Legacy Archive: Dormant</div>
                  </div>
                </div>
              </div>

              {/* Priority Alerts & Live Stream log */}
              <div className="lg:col-span-4 flex flex-col gap-lg" style={{ display: 'flex', flexDirection: 'column', gap: '24px', height: '500px' }}>
                <div className="glass-panel rounded-xl p-md flex-1 flex flex-col" style={{ padding: '24px', overflow: 'hidden', display: 'flex', flexDirection: 'column' }}>
                  <h3 className="font-headline-md text-headline-md text-on-surface mb-md flex items-center gap-2" style={{ fontSize: '18px', fontWeight: '600', marginBottom: '16px', margin: 0 }}>
                    <span className="material-symbols-outlined text-error" style={{ color: 'var(--md-sys-color-error)' }}>warning</span>
                    Priority Alerts
                  </h3>
                  <div style={{ display: 'flex', flexDirection: 'column', gap: '12px', overflowY: 'auto', flex: 1, paddingRight: '4px', marginTop: '12px' }}>
                    <div style={{ background: '#ffffff', padding: '12px', borderRadius: '8px', border: '1px solid var(--md-sys-color-outline-variant)' }}>
                      <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '11px', color: 'var(--md-sys-color-outline)', marginBottom: '4px' }}>
                        <span style={{ color: 'var(--md-sys-color-error)', fontWeight: 'bold' }}>High Severity</span>
                        <span>10:42 AM</span>
                      </div>
                      <p style={{ fontSize: '13px', color: 'var(--md-sys-color-on-background)' }}>Data synchronization drift detected in Faculty Dossiers cluster.</p>
                    </div>
                    <div style={{ background: '#ffffff', padding: '12px', borderRadius: '8px', border: '1px solid var(--md-sys-color-outline-variant)' }}>
                      <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '11px', color: 'var(--md-sys-color-outline)', marginBottom: '4px' }}>
                        <span style={{ color: 'var(--md-sys-color-secondary)', fontWeight: 'bold' }}>Notice</span>
                        <span>09:15 AM</span>
                      </div>
                      <p style={{ fontSize: '13px', color: 'var(--md-sys-color-on-background)' }}>Archival registry update batch #492 completed successfully.</p>
                    </div>
                  </div>
                </div>

                {/* Real-time Ingestion Stream terminal log simulation */}
                <div className="glass-panel rounded-xl p-md" style={{ padding: '20px', borderRadius: '12px' }}>
                  <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '12px' }}>
                    <h3 style={{ fontSize: '16px', fontWeight: '600', margin: 0 }}>Ingestion Stream</h3>
                    <span className="w-2 h-2 rounded-full bg-secondary animate-pulse" style={{ width: '8px', height: '8px', backgroundColor: 'var(--md-sys-color-secondary)', borderRadius: '50%' }}></span>
                  </div>
                  <div className="terminal-box">
                    <div className="terminal-fade"></div>
                    <div style={{ display: 'flex', flexDirection: 'column', gap: '6px' }}>
                      {ingestionLogs.map((log, idx) => (
                        <div key={idx} style={{ color: log.startsWith('>') && !log.includes('SUCCESS') ? 'var(--md-sys-color-on-surface-variant)' : 'var(--md-sys-color-secondary)', opacity: 0.6 + (idx / ingestionLogs.length) * 0.4 }}>
                          {log}
                        </div>
                      ))}
                    </div>
                  </div>
                </div>
              </div>
            </div>

            {/* Departmental Nodes */}
            <div style={{ marginTop: '40px' }}>
              <h3 style={{ fontSize: '22px', fontWeight: '600', color: 'var(--md-sys-color-primary)', marginBottom: '20px' }}>Departmental Nodes</h3>
              <div className="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-4 gap-md">
                
                {/* Sciences Card */}
                <div className="glass-panel p-md rounded-xl hover:-translate-y-1 transition-transform cursor-pointer" style={{ borderTop: '4px solid var(--md-sys-color-secondary-container)', padding: '20px', borderRadius: '12px' }}>
                  <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '16px' }}>
                    <div style={{ width: '36px', height: '36px', borderRadius: '6px', backgroundColor: 'var(--md-sys-color-surface-container)', display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
                      <span className="material-symbols-outlined">science</span>
                    </div>
                    <span className="font-mono-data text-on-surface-variant">NODE_01</span>
                  </div>
                  <h4 style={{ fontSize: '18px', fontWeight: '600', margin: 0 }}>Sciences</h4>
                  <div style={{ display: 'flex', alignItems: 'baseline', gap: '6px', margin: '12px 0 4px 0' }}>
                    <span style={{ fontSize: '32px', fontFamily: 'var(--font-display)', fontWeight: 'bold' }}>98.4%</span>
                    <span style={{ color: 'var(--md-sys-color-secondary)', fontSize: '12px', display: 'flex', alignItems: 'center', fontWeight: '600' }}>
                      <span className="material-symbols-outlined" style={{ fontSize: '14px' }}>arrow_upward</span> 1.2%
                    </span>
                  </div>
                  <p style={{ fontSize: '12px', color: 'var(--md-sys-color-on-surface-variant)', margin: 0 }}>Archival integrity score</p>
                </div>

                {/* Humanities Card */}
                <div className="glass-panel p-md rounded-xl hover:-translate-y-1 transition-transform cursor-pointer" style={{ borderTop: '4px solid var(--md-sys-color-surface-variant)', padding: '20px', borderRadius: '12px' }}>
                  <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '16px' }}>
                    <div style={{ width: '36px', height: '36px', borderRadius: '6px', backgroundColor: 'var(--md-sys-color-surface-container)', display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
                      <span className="material-symbols-outlined">palette</span>
                    </div>
                    <span className="font-mono-data text-on-surface-variant">NODE_02</span>
                  </div>
                  <h4 style={{ fontSize: '18px', fontWeight: '600', margin: 0 }}>Humanities</h4>
                  <div style={{ display: 'flex', alignItems: 'baseline', gap: '6px', margin: '12px 0 4px 0' }}>
                    <span style={{ fontSize: '32px', fontFamily: 'var(--font-display)', fontWeight: 'bold' }}>95.1%</span>
                    <span style={{ color: 'var(--md-sys-color-on-surface-variant)', fontSize: '12px', display: 'flex', alignItems: 'center', fontWeight: '600' }}>
                      <span className="material-symbols-outlined" style={{ fontSize: '14px' }}>horizontal_rule</span> 0.0%
                    </span>
                  </div>
                  <p style={{ fontSize: '12px', color: 'var(--md-sys-color-on-surface-variant)', margin: 0 }}>Archival integrity score</p>
                </div>

                {/* Engineering Card */}
                <div className="glass-panel p-md rounded-xl hover:-translate-y-1 transition-transform cursor-pointer" style={{ borderTop: '4px solid var(--md-sys-color-tertiary-container)', padding: '20px', borderRadius: '12px' }}>
                  <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '16px' }}>
                    <div style={{ width: '36px', height: '36px', borderRadius: '6px', backgroundColor: 'var(--md-sys-color-surface-container)', display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
                      <span className="material-symbols-outlined">calculate</span>
                    </div>
                    <span className="font-mono-data text-on-surface-variant">NODE_03</span>
                  </div>
                  <h4 style={{ fontSize: '18px', fontWeight: '600', margin: 0 }}>Engineering</h4>
                  <div style={{ display: 'flex', alignItems: 'baseline', gap: '6px', margin: '12px 0 4px 0' }}>
                    <span style={{ fontSize: '32px', fontFamily: 'var(--font-display)', fontWeight: 'bold' }}>99.9%</span>
                    <span style={{ color: 'var(--md-sys-color-secondary)', fontSize: '12px', display: 'flex', alignItems: 'center', fontWeight: '600' }}>
                      <span className="material-symbols-outlined" style={{ fontSize: '14px' }}>arrow_upward</span> 0.4%
                    </span>
                  </div>
                  <p style={{ fontSize: '12px', color: 'var(--md-sys-color-on-surface-variant)', margin: 0 }}>Archival integrity score</p>
                </div>

                {/* Athletics Card */}
                <div className="glass-panel p-md rounded-xl hover:-translate-y-1 transition-transform cursor-pointer" style={{ borderTop: '4px solid var(--md-sys-color-error)', padding: '20px', borderRadius: '12px' }}>
                  <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '16px' }}>
                    <div style={{ width: '36px', height: '36px', borderRadius: '6px', backgroundColor: 'var(--md-sys-color-surface-container)', display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
                      <span className="material-symbols-outlined">sports_basketball</span>
                    </div>
                    <span className="font-mono-data text-on-surface-variant">NODE_04</span>
                  </div>
                  <h4 style={{ fontSize: '18px', fontWeight: '600', margin: 0 }}>Athletics</h4>
                  <div style={{ display: 'flex', alignItems: 'baseline', gap: '6px', margin: '12px 0 4px 0' }}>
                    <span style={{ fontSize: '32px', fontFamily: 'var(--font-display)', fontWeight: 'bold' }}>82.3%</span>
                    <span style={{ color: 'var(--md-sys-color-error)', fontSize: '12px', display: 'flex', alignItems: 'center', fontWeight: '600' }}>
                      <span className="material-symbols-outlined" style={{ fontSize: '14px' }}>arrow_downward</span> 4.1%
                    </span>
                  </div>
                  <p style={{ fontSize: '12px', color: 'var(--md-sys-color-on-surface-variant)', margin: 0 }}>Archival integrity score</p>
                </div>

              </div>
            </div>
          </div>
        )}

        {/* VIEW 2: Student Ledger (Student Directory) */}
        {currentTab === 'directory' && (
          <div>
            <div className="flex flex-col sm:flex-row sm:items-end justify-between gap-4 mb-xl">
              <div>
                <h2 className="page-title" style={{ fontSize: '32px', marginBottom: '4px' }}>Student Ledger Repository</h2>
                <p className="page-description" style={{ marginBottom: 0 }}>
                  Archival database of active, inactive, and alumni student records. Total index references: {students.length}.
                </p>
              </div>
              <div className="flex gap-2">
                <button 
                  onClick={runExportManifest}
                  disabled={exporting}
                  className="btn btn-secondary"
                  style={{ display: 'flex', alignItems: 'center', gap: '6px' }}
                >
                  <span className="material-symbols-outlined" style={{ fontSize: '16px' }}>download</span>
                  {exporting ? 'Exporting...' : 'Export Manifest'}
                </button>
              </div>
            </div>

            {showExportSuccess && (
              <div style={{ background: 'var(--md-sys-color-secondary-container)', color: 'var(--md-sys-color-on-secondary-container)', padding: '12px 20px', borderRadius: '12px', marginBottom: '20px', fontSize: '14px', fontWeight: '500', display: 'flex', alignItems: 'center', gap: '8px' }}>
                <span className="material-symbols-outlined" style={{ color: 'var(--md-sys-color-secondary)' }}>check_circle</span>
                Student registry manifest exported successfully to CSV spreadsheet.
              </div>
            )}

            <div className="grid grid-cols-1 lg:grid-cols-12 gap-lg items-start">
              {/* Left Filter Sidebar */}
              <aside className="lg:col-span-3 glass-panel rounded-xl p-md sticky" style={{ padding: '20px', borderRadius: '12px' }}>
                <div style={{ display: 'flex', alignItems: 'center', gap: '8px', borderBottom: '1px solid var(--md-sys-color-surface-variant)', paddingBottom: '12px', marginBottom: '16px' }}>
                  <span className="material-symbols-outlined" style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>filter_list</span>
                  <h3 style={{ fontSize: '12px', fontWeight: 'bold', textTransform: 'uppercase', letterSpacing: '0.08em', color: 'var(--md-sys-color-primary)', margin: 0 }}>Refinement Parameters</h3>
                </div>

                <div style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
                  {/* Search bar inside filters */}
                  <div>
                    <label style={{ fontSize: '11px', fontWeight: '600', color: 'var(--md-sys-color-on-surface-variant)', marginBottom: '8px', display: 'block', textTransform: 'uppercase' }}>Search Identity</label>
                    <input 
                      type="text" 
                      placeholder="Search name or roll number..."
                      className="form-control"
                      value={searchTerm}
                      onChange={(e) => setSearchTerm(e.target.value)}
                    />
                  </div>

                  {/* Program Level dropdown */}
                  <div>
                    <label style={{ fontSize: '11px', fontWeight: '600', color: 'var(--md-sys-color-on-surface-variant)', marginBottom: '8px', display: 'block', textTransform: 'uppercase' }}>Program Level</label>
                    <select 
                      className="form-control"
                      value={filterLevel}
                      onChange={(e) => setFilterLevel(e.target.value)}
                    >
                      <option value="ALL">All Programs</option>
                      <option value="BTECH">Undergraduate (B.Tech)</option>
                      <option value="MTECH">Graduate (M.Tech)</option>
                      <option value="PHD">Doctoral (PhD)</option>
                    </select>
                  </div>

                  {/* Matriculation Cohort */}
                  <div>
                    <label style={{ fontSize: '11px', fontWeight: '600', color: 'var(--md-sys-color-on-surface-variant)', marginBottom: '8px', display: 'block', textTransform: 'uppercase' }}>Matriculation Year</label>
                    <select 
                      className="form-control"
                      value={filterCohort}
                      onChange={(e) => setFilterCohort(e.target.value)}
                    >
                      <option value="ALL">All Years</option>
                      <option value="2023">Admissions Cohort 2023</option>
                      <option value="2024">Admissions Cohort 2024</option>
                      <option value="2025">Admissions Cohort 2025</option>
                      <option value="2026">Admissions Cohort 2026</option>
                    </select>
                  </div>

                  {/* Departmental Origin */}
                  <div>
                    <label style={{ fontSize: '11px', fontWeight: '600', color: 'var(--md-sys-color-on-surface-variant)', marginBottom: '8px', display: 'block', textTransform: 'uppercase' }}>Departmental Origin</label>
                    <input 
                      type="text" 
                      placeholder="e.g. Computer Science"
                      className="form-control"
                      value={filterDept}
                      onChange={(e) => setFilterDept(e.target.value)}
                    />
                  </div>

                  {/* Checkbox filters for Academic Standing */}
                  <div>
                    <label style={{ fontSize: '11px', fontWeight: '600', color: 'var(--md-sys-color-on-surface-variant)', marginBottom: '8px', display: 'block', textTransform: 'uppercase' }}>Academic Standing</label>
                    <div style={{ display: 'flex', flexDirection: 'column', gap: '8px', marginTop: '6px' }}>
                      <label style={{ display: 'flex', alignItems: 'center', gap: '8px', fontSize: '13px', cursor: 'pointer' }}>
                        <input 
                          type="checkbox" 
                          checked={filterStanding.Optimal}
                          onChange={(e) => setFilterStanding({ ...filterStanding, Optimal: e.target.checked })}
                          style={{ accentColor: 'var(--md-sys-color-secondary)' }}
                        />
                        Optimal Standing (Sage)
                      </label>
                      <label style={{ display: 'flex', alignItems: 'center', gap: '8px', fontSize: '13px', cursor: 'pointer' }}>
                        <input 
                          type="checkbox" 
                          checked={filterStanding.ReviewRequired}
                          onChange={(e) => setFilterStanding({ ...filterStanding, ReviewRequired: e.target.checked })}
                          style={{ accentColor: 'var(--md-sys-color-surface-tint)' }}
                        />
                        Review Required (Ochre)
                      </label>
                      <label style={{ display: 'flex', alignItems: 'center', gap: '8px', fontSize: '13px', cursor: 'pointer' }}>
                        <input 
                          type="checkbox" 
                          checked={filterStanding.Suspension}
                          onChange={(e) => setFilterStanding({ ...filterStanding, Suspension: e.target.checked })}
                          style={{ accentColor: 'var(--md-sys-color-error)' }}
                        />
                        Academic Suspension
                      </label>
                    </div>
                  </div>
                </div>
              </aside>

              {/* Student Cards List */}
              <section className="lg:col-span-9" style={{ display: 'flex', flexDirection: 'column', gap: '16px' }}>
                <div className="grid grid-cols-1 md:grid-cols-2 gap-md">
                  {filteredStudents.map((s) => (
                    <article 
                      key={s.rollNumber} 
                      className="scholar-card" 
                      style={{ 
                        position: 'relative', 
                        display: 'flex', 
                        flexDirection: 'column', 
                        justifyContent: 'space-between', 
                        minHeight: '230px',
                        borderTop: s.status === 'Optimal' ? '4px solid var(--md-sys-color-secondary)' : s.status === 'Review Required' ? '4px solid var(--md-sys-color-surface-tint)' : '4px solid var(--md-sys-color-error)'
                      }}
                    >
                      <div>
                        <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'flex-start', marginBottom: '12px' }}>
                          <div style={{ display: 'flex', gap: '12px' }}>
                            <div className="student-portrait" style={{ width: '56px', height: '56px', borderRadius: '8px', backgroundColor: 'var(--md-sys-color-surface-container-high)', overflow: 'hidden', display: 'flex', alignItems: 'center', justifyContent: 'center', border: '1px solid var(--md-sys-color-surface-variant)' }}>
                              {s.portraitUrl ? (
                                <img src={s.portraitUrl} alt={s.name} style={{ width: '100%', height: '100%', objectFit: 'cover' }} />
                              ) : (
                                <span className="material-symbols-outlined" style={{ fontSize: '28px', color: 'var(--md-sys-color-outline)' }}>person</span>
                              )}
                            </div>
                            <div>
                              <h3 style={{ fontSize: '16px', fontWeight: '600', color: 'var(--md-sys-color-primary)', margin: 0 }}>{s.name}</h3>
                              <p style={{ fontFamily: 'var(--font-display)', fontSize: '11px', color: 'var(--md-sys-color-on-surface-variant)', marginTop: '2px' }}>ID: {s.rollNumber}</p>
                              <p style={{ fontSize: '12px', color: 'var(--md-sys-color-on-surface)', marginTop: '2px' }}>{s.branch.name}</p>
                            </div>
                          </div>

                          <span className={`badge ${s.status === 'Optimal' ? 'badge-active' : s.status === 'Review Required' ? 'badge-pending' : 'badge-danger'}`}>
                            {s.status || 'Optimal'}
                          </span>
                        </div>

                        <div style={{ borderTop: '1px solid var(--md-sys-color-surface-variant)', paddingTop: '10px', marginTop: '10px', fontSize: '13px' }}>
                          <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '8px' }}>
                            <div>
                              <span style={{ fontSize: '10px', textTransform: 'uppercase', color: 'var(--md-sys-color-on-surface-variant)', letterSpacing: '0.04em' }}>Current GPA</span>
                              <p style={{ fontSize: '15px', fontWeight: 'bold', color: 'var(--md-sys-color-primary)', margin: '2px 0 0 0' }}>{s.loadedCgpa.toFixed(2)}</p>
                            </div>
                            <div>
                              <span style={{ fontSize: '10px', textTransform: 'uppercase', color: 'var(--md-sys-color-on-surface-variant)', letterSpacing: '0.04em' }}>Program & Campus</span>
                              <p style={{ fontSize: '13px', color: 'var(--md-sys-color-primary)', margin: '2px 0 0 0' }}>{s.level} ({s.institution})</p>
                            </div>
                          </div>
                        </div>

                        {/* Credits progression timeline with nodes */}
                        <div style={{ marginTop: '16px' }}>
                          <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '10px', color: 'var(--md-sys-color-on-surface-variant)', marginBottom: '8px', textTransform: 'uppercase', letterSpacing: '0.04em' }}>
                            <span>Degree Progression</span>
                            <span>{s.creditsEarned} / {s.totalCredits} Credits</span>
                          </div>
                          
                          <div className="timeline-bar">
                            <div 
                              className="timeline-fill"
                              style={{ 
                                width: `${(s.creditsEarned / s.totalCredits) * 100}%`, 
                                backgroundColor: s.status === 'Optimal' ? 'var(--md-sys-color-secondary)' : s.status === 'Review Required' ? 'var(--md-sys-color-surface-tint)' : 'var(--md-sys-color-error)'
                              }}
                            />
                            <div className="timeline-node active" style={{ left: '0%' }}></div>
                            <div className={`timeline-node ${s.creditsEarned >= (s.totalCredits * 0.33) ? 'active' : ''}`} style={{ left: '33%' }}></div>
                            <div className={`timeline-node ${s.creditsEarned >= (s.totalCredits * 0.66) ? 'active' : ''} ${s.creditsEarned >= (s.totalCredits * 0.66) && s.creditsEarned < s.totalCredits ? (s.status === 'Optimal' ? 'current' : s.status === 'Review Required' ? 'current-warning' : 'current-danger') : ''}`} style={{ left: '66%' }}></div>
                            <div className={`timeline-node ${s.creditsEarned === s.totalCredits ? 'active' : ''}`} style={{ left: '100%' }}></div>
                          </div>
                          <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '9px', color: 'var(--md-sys-color-on-surface-variant)', marginTop: '4px' }}>
                            <span>Yr 1</span>
                            <span>Yr 2</span>
                            <span>Yr 3</span>
                            <span>Yr 4</span>
                          </div>
                        </div>
                      </div>

                      <div style={{ display: 'flex', justifyContent: 'flex-end', marginTop: '16px', borderTop: '1px solid var(--md-sys-color-surface-variant)', paddingTop: '10px' }}>
                        {deletingRoll === s.rollNumber ? (
                          <div style={{ display: 'flex', gap: '8px', alignItems: 'center' }}>
                            <span style={{ fontSize: '12px', color: 'var(--md-sys-color-error)', fontWeight: '600' }}>Confirm deletion?</span>
                            <button 
                              className="btn btn-danger" 
                              style={{ padding: '4px 10px', fontSize: '11px' }}
                              onClick={() => commitDeleteRecord(s.rollNumber)}
                            >
                              Confirm
                            </button>
                            <button 
                              className="btn" 
                              style={{ padding: '4px 10px', fontSize: '11px' }}
                              onClick={() => setDeletingRoll(null)}
                            >
                              Cancel
                            </button>
                          </div>
                        ) : (
                          <button 
                            className="btn btn-danger"
                            style={{ padding: '4px 10px', fontSize: '11px', display: 'flex', alignItems: 'center', gap: '4px' }}
                            onClick={() => setDeletingRoll(s.rollNumber)}
                          >
                            <span className="material-symbols-outlined" style={{ fontSize: '14px' }}>delete</span>
                            Remove Profile
                          </button>
                        )}
                      </div>
                    </article>
                  ))}
                  {filteredStudents.length === 0 && (
                    <div style={{ gridColumn: 'span 2', textAlign: 'center', padding: '40px', color: 'var(--md-sys-color-on-surface-variant)', background: '#ffffff', borderRadius: '12px', border: '1px solid var(--md-sys-color-outline-variant)' }}>
                      No student profiles match the refinement parameters.
                    </div>
                  )}
                </div>

                {/* Register New Student Form */}
                <div className="visualizer-card" id="register-student-form" style={{ marginTop: '24px', borderRadius: '12px' }}>
                  <h3 style={{ borderBottom: '1px solid var(--md-sys-color-surface-variant)', paddingBottom: '10px', marginBottom: '20px', fontSize: '20px', fontWeight: '600' }}>Register New Student</h3>
                  <form onSubmit={handleAddStudent}>
                    <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(220px, 1fr))', gap: '16px' }}>
                      <div className="form-group">
                        <label>Student Name</label>
                        <input 
                          type="text" 
                          className="form-control"
                          value={newName}
                          onChange={(e) => setNewName(e.target.value)}
                          placeholder="e.g. Jane Doe"
                        />
                      </div>
                      <div className="form-group">
                        <label>Unique Registry Number</label>
                        <input 
                          type="text" 
                          className="form-control"
                          value={newRoll}
                          onChange={(e) => setNewRoll(e.target.value)}
                          placeholder="e.g. 2026CSE999"
                        />
                      </div>
                      <div className="form-group">
                        <label>Admissions Campus</label>
                        <select 
                          className="form-control"
                          value={newInst}
                          onChange={(e) => setNewInst(e.target.value)}
                        >
                          <option value="Apex Campus">Apex Campus (Admissions)</option>
                          <option value="Vertex Campus">Vertex Campus (Admissions)</option>
                        </select>
                      </div>
                      <div className="form-group">
                        <label>Department Branch</label>
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
                        <label>Degree Program</label>
                        <select 
                          className="form-control"
                          value={newLevel}
                          onChange={(e) => setNewLevel(e.target.value)}
                        >
                          <option value="BTECH">B.Tech (Undergraduate)</option>
                          <option value="MTECH">M.Tech (Graduate)</option>
                          <option value="PHD">PhD (Doctoral)</option>
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
                        <label>Initial GPA standing</label>
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
                      Save Student Profile
                    </button>
                  </form>
                </div>
              </section>
            </div>
          </div>
        )}

        {/* VIEW 3: Faculty Registry */}
        {currentTab === 'faculty' && (
          <div>
            <div className="mb-xl flex flex-col md:flex-row md:items-end justify-between gap-md">
              <div>
                <h2 className="page-title" style={{ fontSize: '32px', marginBottom: '4px' }}>Faculty Registry</h2>
                <p className="page-description" style={{ marginBottom: 0 }}>
                  Comprehensive dossier of academic personnel, research focus areas, and departmental hierarchies.
                </p>
              </div>
              <div className="flex gap-sm">
                <button className="btn btn-secondary">Export Dossier Manifest</button>
              </div>
            </div>

            <div className="grid grid-cols-1 lg:grid-cols-12 gap-lg">
              {/* Department Tree Sidebar */}
              <div className="lg:col-span-4">
                <div className="glass-panel p-lg rounded-xl h-full" style={{ padding: '24px', borderRadius: '12px' }}>
                  <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '16px', borderBottom: '1px solid var(--md-sys-color-surface-variant)', paddingBottom: '10px' }}>
                    <h3 style={{ fontSize: '18px', fontWeight: '600', margin: 0 }}>Departmental Tree</h3>
                    <span className="material-symbols-outlined" style={{ color: 'var(--md-sys-color-outline)' }}>account_tree</span>
                  </div>
                  <div style={{ display: 'flex', flexDirection: 'column', gap: '12px', fontSize: '14px' }}>
                    <div>
                      <div style={{ display: 'flex', alignItems: 'center', gap: '8px', padding: '8px', background: 'var(--md-sys-color-surface-container)', borderRadius: '8px', fontWeight: '600' }}>
                        <span className="material-symbols-outlined" style={{ fontSize: '18px' }}>biotech</span>
                        <span>School of Sciences</span>
                      </div>
                      <div style={{ marginLeft: '20px', borderLeft: '1px solid var(--md-sys-color-outline-variant)', paddingLeft: '12px', marginTop: '6px', display: 'flex', flexDirection: 'column', gap: '8px' }}>
                        <span style={{ color: 'var(--md-sys-color-primary)', fontWeight: '500' }}>• Computational Biology</span>
                        <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>• Quantum Physics</span>
                        <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>• Applied Mathematics</span>
                      </div>
                    </div>
                    <div>
                      <div style={{ display: 'flex', alignItems: 'center', gap: '8px', padding: '8px', color: 'var(--md-sys-color-on-surface-variant)' }}>
                        <span className="material-symbols-outlined" style={{ fontSize: '18px' }}>engineering</span>
                        <span>School of Engineering</span>
                      </div>
                    </div>
                    <div>
                      <div style={{ display: 'flex', alignItems: 'center', gap: '8px', padding: '8px', color: 'var(--md-sys-color-on-surface-variant)' }}>
                        <span className="material-symbols-outlined" style={{ fontSize: '18px' }}>palette</span>
                        <span>School of Arts & Humanities</span>
                      </div>
                    </div>
                  </div>
                </div>
              </div>

              {/* Faculty Dossiers list */}
              <div className="lg:col-span-8" style={{ display: 'flex', flexDirection: 'column', gap: '16px' }}>
                <div style={{ background: 'var(--md-sys-color-surface-container-low)', padding: '12px 16px', borderRadius: '8px', border: '1px solid var(--md-sys-color-outline-variant)', fontSize: '13px', display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
                  <span>Department: <strong>Computational Biology</strong></span>
                  <span>2 Active Faculty Dossiers</span>
                </div>

                <div className="grid grid-cols-1 md:grid-cols-2 gap-md">
                  <article className="scholar-card" style={{ display: 'flex', flexDirection: 'column', justifyContent: 'space-between', minHeight: '210px' }}>
                    <div style={{ display: 'flex', gap: '12px' }}>
                      <div style={{ width: '64px', height: '64px', borderRadius: '8px', overflow: 'hidden', border: '1px solid var(--md-sys-color-outline-variant)', shrink: 0 }}>
                        <img src="https://lh3.googleusercontent.com/aida-public/AB6AXuDcbOMpx5UI6LS2LuMYU0sgfNMKE7gpQ7wmU2gGDaeRBUyryyvHmbNvQ6qUu0hutp1j3yKJl-PkmUutyTZrcBdQEPb0TlFrSW8A7wKsKBxDu4jcZBZPa38OqjdXAV0ZoH-rGou-gtAFjmgmzJLdXAbbKK1UC-OX4Dzur1_c2LrhMqZvVN1w3otvIfSgz10lD54GIxqcd2OEwi6SaHo8NZhwRni4SUYtcnhtsePYs61RC_Ri-0plHuaa3_bgDkx0wmT5Shqsa1WXi4a-" alt="Dr. Aris Thorne" style={{ width: '100%', height: '100%', objectFit: 'cover' }} />
                      </div>
                      <div>
                        <h4 style={{ fontSize: '16px', fontWeight: '600', margin: 0 }}>Dr. Aris Thorne</h4>
                        <p style={{ fontSize: '13px', color: 'var(--md-sys-color-on-surface-variant)', marginTop: '2px', margin: 0 }}>Chair, Computational Biology</p>
                        <span className="badge badge-active" style={{ marginTop: '8px' }}>Tenured</span>
                      </div>
                    </div>
                    <div style={{ marginTop: '16px', borderTop: '1px solid var(--md-sys-color-surface-variant)', paddingTop: '12px' }}>
                      <span style={{ fontSize: '10px', textTransform: 'uppercase', color: 'var(--md-sys-color-on-surface-variant)', letterSpacing: '0.04em' }}>Primary Focus</span>
                      <p style={{ fontSize: '14px', color: 'var(--md-sys-color-primary)', marginTop: '2px', fontWeight: '500', margin: 0 }}>Genomic Data Modeling</p>
                    </div>
                  </article>

                  <article className="scholar-card" style={{ display: 'flex', flexDirection: 'column', justifyContent: 'space-between', minHeight: '210px' }}>
                    <div style={{ display: 'flex', gap: '12px' }}>
                      <div style={{ width: '64px', height: '64px', borderRadius: '8px', overflow: 'hidden', border: '1px solid var(--md-sys-color-outline-variant)', shrink: 0 }}>
                        <img src="https://lh3.googleusercontent.com/aida-public/AB6AXuAL-sRontsbGZLdVKB8DfLEFHiqZRYPTO4QKr9kSTTnbjaI7iMGhDjvDo8R09mzNul9OwX7Af27JWvzJK3pmwpohtXNnHFGf4vAjcBT-TrRUysBKQz1ds-cTHg1ZNLD0dAsLHdCxKqViy_oSsACR0F-GAQU_iH3zcJR6Y8B-2n3E4PCAdqPI5BDaZNxZcQDAM5H3_D1XkK9fUUMQnbz84TaulCOSwfrEQeA1OXrpaPrL__2Tc8yGMEVpjGLDcq7jU3Yau3E2Mg2_BY7" alt="Prof. Elias Vance" style={{ width: '100%', height: '100%', objectFit: 'cover' }} />
                      </div>
                      <div>
                        <h4 style={{ fontSize: '16px', fontWeight: '600', margin: 0 }}>Prof. Elias Vance</h4>
                        <p style={{ fontSize: '13px', color: 'var(--md-sys-color-on-surface-variant)', marginTop: '2px', margin: 0 }}>Senior Researcher</p>
                        <span className="badge" style={{ marginTop: '8px', backgroundColor: 'var(--md-sys-color-surface-container)', color: 'var(--md-sys-color-on-surface-variant)', borderColor: 'var(--md-sys-color-outline-variant)' }}>Adjunct</span>
                      </div>
                    </div>
                    <div style={{ marginTop: '16px', borderTop: '1px solid var(--md-sys-color-surface-variant)', paddingTop: '12px' }}>
                      <span style={{ fontSize: '10px', textTransform: 'uppercase', color: 'var(--md-sys-color-on-surface-variant)', letterSpacing: '0.04em' }}>Primary Focus</span>
                      <p style={{ fontSize: '14px', color: 'var(--md-sys-color-primary)', marginTop: '2px', fontWeight: '500', margin: 0 }}>Neural Network Synthesis</p>
                    </div>
                  </article>
                </div>
              </div>
            </div>
          </div>
        )}

        {/* VIEW 4: Academic Catalog (Course Catalog) */}
        {currentTab === 'catalog' && (
          <div>
            <div className="mb-xl">
              <h2 className="page-title" style={{ fontSize: '32px', marginBottom: '4px' }}>Academic Catalog</h2>
              <p className="page-description">
                Browse official course offerings and prerequisites from the academic campuses. The catalog allows students from both campuses to register for cross-institutional courses.
              </p>
            </div>

            <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(360px, 1fr))', gap: '32px' }}>
              
              {/* Apex Campus Catalog */}
              <div className="visualizer-card" style={{ borderRadius: '12px' }}>
                <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '20px', borderBottom: '1px solid var(--md-sys-color-surface-variant)', paddingBottom: '12px' }}>
                  <h3 style={{ margin: 0, fontSize: '20px' }}>Apex Campus Catalog</h3>
                  <span className="badge badge-active">
                    Custom Identifier Structure
                  </span>
                </div>
                <div style={{ display: 'flex', flexDirection: 'column', gap: '16px', maxHeight: '420px', overflowY: 'auto', paddingRight: '8px' }}>
                  {APEX_COURSES.map(c => (
                    <div key={c.code} style={{ background: 'var(--md-sys-color-surface-container-low)', padding: '16px', borderRadius: '8px', border: '1px solid var(--md-sys-color-outline-variant)' }}>
                      <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '8px' }}>
                        <span style={{ fontSize: '12px', fontFamily: 'var(--font-display)', fontWeight: 'bold', color: 'var(--md-sys-color-tertiary)' }}>{c.code}</span>
                        <span style={{ fontSize: '12px', color: 'var(--md-sys-color-on-surface-variant)' }}>{c.credits} Credits</span>
                      </div>
                      <h4 style={{ fontSize: '15px', fontWeight: '600', color: 'var(--md-sys-color-primary)', margin: '4px 0' }}>{c.name}</h4>
                      <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '11px', color: 'var(--md-sys-color-on-surface-variant)', marginTop: '8px' }}>
                        <span>Instructor: {c.instructor}</span>
                        {c.prereqs && <span style={{ color: 'var(--md-sys-color-secondary)', fontWeight: '500' }}>Prerequisite: {c.prereqs}</span>}
                      </div>
                    </div>
                  ))}
                </div>
              </div>

              {/* Vertex Campus Catalog */}
              <div className="visualizer-card" style={{ borderRadius: '12px' }}>
                <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '20px', borderBottom: '1px solid var(--md-sys-color-surface-variant)', paddingBottom: '12px' }}>
                  <h3 style={{ margin: 0, fontSize: '20px' }}>Vertex Campus Catalog</h3>
                  <span className="badge badge-pending">
                    Numeric Cataloging System
                  </span>
                </div>
                <div style={{ display: 'flex', flexDirection: 'column', gap: '16px', maxHeight: '420px', overflowY: 'auto', paddingRight: '8px' }}>
                  {VERTEX_COURSES.map(c => (
                    <div key={c.code} style={{ background: 'var(--md-sys-color-surface-container-low)', padding: '16px', borderRadius: '8px', border: '1px solid var(--md-sys-color-outline-variant)' }}>
                      <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '8px' }}>
                        <span style={{ fontSize: '12px', fontFamily: 'var(--font-display)', fontWeight: 'bold', color: 'var(--md-sys-color-secondary)' }}>{c.code}</span>
                        <span style={{ fontSize: '12px', color: 'var(--md-sys-color-on-surface-variant)' }}>{c.credits} Credits</span>
                      </div>
                      <h4 style={{ fontSize: '15px', fontWeight: '600', color: 'var(--md-sys-color-primary)', margin: '4px 0' }}>{c.name}</h4>
                      <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '11px', color: 'var(--md-sys-color-on-surface-variant)', marginTop: '8px' }}>
                        <span>Instructor: {c.instructor}</span>
                        {c.prereqs && <span style={{ color: 'var(--md-sys-color-secondary)', fontWeight: '500' }}>Prerequisite: {c.prereqs}</span>}
                      </div>
                    </div>
                  ))}
                </div>
              </div>

            </div>
          </div>
        )}

        {/* VIEW 5: Fiscal Archives */}
        {currentTab === 'fiscal' && (
          <div>
            <div className="mb-xl flex flex-col md:flex-row md:items-end justify-between gap-md">
              <div>
                <h2 className="page-title" style={{ fontSize: '32px', marginBottom: '4px' }}>Fiscal Archives</h2>
                <p className="page-description" style={{ marginBottom: 0 }}>
                  Institutional financial records, budgetary allocations, and endowment tracking.
                </p>
              </div>
              <div className="flex gap-sm">
                <button className="btn btn-primary" onClick={() => alert("Reconciling fiscal ledgers... Completed without anomalies.")}>Run Verification</button>
              </div>
            </div>

            <div className="grid grid-cols-1 md:grid-cols-12 gap-lg mb-xl">
              {/* KPI Total Endowment */}
              <div className="md:col-span-4 glass-panel rounded-xl p-lg flex flex-col justify-between" style={{ padding: '24px', minHeight: '160px', borderRadius: '12px' }}>
                <div style={{ display: 'flex', justifyContent: 'space-between', color: 'var(--md-sys-color-on-surface-variant)', fontSize: '11px', textTransform: 'uppercase', fontWeight: 'bold', letterSpacing: '0.04em' }}>
                  <span>Total Endowment</span>
                  <span className="material-symbols-outlined" style={{ color: 'var(--md-sys-color-secondary)', background: 'rgba(73, 104, 0, 0.1)', padding: '4px', borderRadius: '4px' }}>account_balance</span>
                </div>
                <div style={{ marginTop: '16px' }}>
                  <div style={{ fontSize: '36px', fontWeight: '500', fontFamily: 'var(--font-display)', color: 'var(--md-sys-color-primary)' }}>$4.2B</div>
                  <div style={{ fontSize: '12px', color: 'var(--md-sys-color-secondary)', marginTop: '4px', display: 'flex', alignItems: 'center', gap: '2px' }}>
                    <span className="material-symbols-outlined" style={{ fontSize: '14px' }}>trending_up</span>
                    <span>+2.4% vs FY25</span>
                  </div>
                </div>
              </div>

              {/* Operating Budget */}
              <div className="md:col-span-4 glass-panel rounded-xl p-lg flex flex-col justify-between" style={{ padding: '24px', minHeight: '160px', borderRadius: '12px' }}>
                <div style={{ display: 'flex', justifyContent: 'space-between', color: 'var(--md-sys-color-on-surface-variant)', fontSize: '11px', textTransform: 'uppercase', fontWeight: 'bold', letterSpacing: '0.04em' }}>
                  <span>Operating Budget</span>
                  <span className="material-symbols-outlined" style={{ color: 'var(--md-sys-color-tertiary)', background: 'rgba(74, 111, 165, 0.1)', padding: '4px', borderRadius: '4px' }}>pie_chart</span>
                </div>
                <div style={{ marginTop: '16px' }}>
                  <div style={{ fontSize: '36px', fontWeight: '500', fontFamily: 'var(--font-display)', color: 'var(--md-sys-color-primary)' }}>$850M</div>
                  <div style={{ fontSize: '12px', color: 'var(--md-sys-color-on-surface-variant)', marginTop: '4px', display: 'flex', alignItems: 'center', gap: '2px' }}>
                    <span className="material-symbols-outlined" style={{ fontSize: '14px' }}>sync</span>
                    <span>65% Allocated</span>
                  </div>
                </div>
              </div>

              {/* Status */}
              <div className="md:col-span-4 glass-panel rounded-xl p-lg flex flex-col justify-between" style={{ padding: '24px', borderLeft: '4px solid var(--md-sys-color-secondary-container)', minHeight: '160px', borderRadius: '12px' }}>
                <div>
                  <div style={{ fontSize: '11px', textTransform: 'uppercase', fontWeight: 'bold', letterSpacing: '0.04em', color: 'var(--md-sys-color-on-surface-variant)', marginBottom: '8px' }}>Audit Status</div>
                  <div style={{ display: 'flex', alignItems: 'center', gap: '8px', fontSize: '14px', color: 'var(--md-sys-color-primary)' }}>
                    <span style={{ width: '8px', height: '8px', borderRadius: '50%', backgroundColor: 'var(--md-sys-color-secondary)' }}></span>
                    <span>All ledgers reconciled</span>
                  </div>
                </div>
                <span style={{ fontSize: '12px', color: 'var(--md-sys-color-on-surface-variant)', marginTop: '16px' }}>Audit integrity verified by state protocol</span>
              </div>
            </div>

            {/* Allocation Chart */}
            <div className="glass-panel rounded-xl p-lg mb-xl" style={{ padding: '24px', borderRadius: '12px' }}>
              <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '20px' }}>
                <h3 style={{ fontSize: '18px', fontWeight: '600', margin: 0 }}>Allocation vs. Expenditure</h3>
                <span style={{ fontSize: '12px', color: 'var(--md-sys-color-on-surface-variant)' }}>Fiscal Year Quarters</span>
              </div>
              <div style={{ height: '220px', width: '100%', position: 'relative' }}>
                <svg viewBox="0 0 800 220" preserveAspectRatio="none" style={{ width: '100%', height: '100%' }}>
                  <path className="graph-area" d="M0,220 L0,180 Q100,130 200,160 T400,90 T600,110 T800,40 L800,220 Z" fill="rgba(6, 6, 7, 0.05)"></path>
                  <path className="graph-line" d="M0,180 Q100,130 200,160 T400,90 T600,110 T800,40" fill="none" stroke="var(--md-sys-color-primary)" strokeWidth="2"></path>
                  <path d="M0,200 Q100,170 200,190 T400,140 T600,120 T800,80" fill="none" stroke="var(--md-sys-color-tertiary)" strokeDasharray="5,5" strokeWidth="2"></path>
                </svg>
              </div>
            </div>
          </div>
        )}

        {/* VIEW 6: Institutional Reports */}
        {currentTab === 'reports' && (
          <div>
            <div className="mb-xl flex flex-col md:flex-row md:items-end justify-between gap-md">
              <div>
                <h2 className="page-title" style={{ fontSize: '32px', marginBottom: '4px' }}>Institutional Reports</h2>
                <p className="page-description" style={{ marginBottom: 0 }}>
                  Longitudinal Analytics Suite & Query Builder.
                </p>
              </div>
            </div>

            <div className="grid grid-cols-1 md:grid-cols-12 gap-lg">
              {/* Reports List */}
              <div className="md:col-span-8 flex flex-col gap-lg" style={{ display: 'flex', flexDirection: 'column', gap: '24px' }}>
                <div className="grid grid-cols-3 gap-md">
                  <div className="stat-card" style={{ padding: '16px' }}>
                    <span style={{ fontSize: '11px', color: 'var(--md-sys-color-on-surface-variant)', textTransform: 'uppercase' }}>Retention Rate</span>
                    <div style={{ fontSize: '28px', fontWeight: 'bold', margin: '8px 0 0 0', color: 'var(--md-sys-color-primary)', fontFamily: 'var(--font-display)' }}>94.2%</div>
                  </div>
                  <div className="stat-card" style={{ padding: '16px' }}>
                    <span style={{ fontSize: '11px', color: 'var(--md-sys-color-on-surface-variant)', textTransform: 'uppercase' }}>Research Output</span>
                    <div style={{ fontSize: '28px', fontWeight: 'bold', margin: '8px 0 0 0', color: 'var(--md-sys-color-primary)', fontFamily: 'var(--font-display)' }}>1,204</div>
                  </div>
                  <div className="stat-card" style={{ padding: '16px' }}>
                    <span style={{ fontSize: '11px', color: 'var(--md-sys-color-on-surface-variant)', textTransform: 'uppercase' }}>Endowment</span>
                    <div style={{ fontSize: '28px', fontWeight: 'bold', margin: '8px 0 0 0', color: 'var(--md-sys-color-primary)', fontFamily: 'var(--font-display)' }}>Optimal</div>
                  </div>
                </div>

                <div className="glass-panel p-lg rounded-xl" style={{ padding: '24px', borderRadius: '12px' }}>
                  <h3 style={{ fontSize: '18px', fontWeight: '600', marginBottom: '16px', margin: 0 }}>Enrollment Trajectory</h3>
                  <div style={{ height: '200px', width: '100%', marginTop: '16px' }}>
                    <svg viewBox="0 0 800 200" preserveAspectRatio="none" style={{ width: '100%', height: '100%' }}>
                      <path d="M0,160 Q20,140 40,130 T80,80 T100,40" fill="none" stroke="var(--md-sys-color-tertiary)" strokeWidth="2"></path>
                      <path d="M0,160 L0,200 L100,200 L100,40 Q80,80 40,130 T0,160" fill="rgba(74, 111, 165, 0.05)"></path>
                    </svg>
                  </div>
                </div>
              </div>

              {/* Side templates */}
              <div className="md:col-span-4">
                <div className="stat-card" style={{ padding: '20px' }}>
                  <h3 style={{ fontSize: '16px', fontWeight: '600', marginBottom: '16px', borderBottom: '1px solid var(--md-sys-color-surface-variant)', paddingBottom: '8px', margin: 0 }}>Report Templates</h3>
                  <div style={{ display: 'flex', flexDirection: 'column', gap: '12px', fontSize: '13px', marginTop: '12px' }}>
                    <div style={{ padding: '10px', background: 'var(--md-sys-color-surface-container-low)', borderRadius: '6px' }}>
                      <strong>Accreditation Compliance</strong>
                      <p style={{ margin: '4px 0 0 0', fontSize: '11px', color: 'var(--md-sys-color-on-surface-variant)' }}>Processed 2 days ago</p>
                    </div>
                    <div style={{ padding: '10px', background: 'var(--md-sys-color-surface-container-low)', borderRadius: '6px' }}>
                      <strong>Diversity & Inclusion Audit</strong>
                      <p style={{ margin: '4px 0 0 0', fontSize: '11px', color: 'var(--md-sys-color-on-surface-variant)' }}>Processed 1 week ago</p>
                    </div>
                    <div style={{ padding: '10px', background: 'var(--md-sys-color-surface-container-low)', borderRadius: '6px' }}>
                      <strong>Grant Utilization Summary</strong>
                      <p style={{ margin: '4px 0 0 0', fontSize: '11px', color: 'var(--md-sys-color-on-surface-variant)' }}>Processed 1 month ago</p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        )}

        {/* VIEW 7: Parallel Rankings (Concurrent Sorting Visualizer) */}
        {currentTab === 'sorting' && (
          <div>
            <h2 className="page-title" style={{ fontSize: '32px' }}>Academic Rankings</h2>
            <p className="page-description">
              Calculates academic standings by sorting student profiles by GPA. The system partitions records into cohorts, analyzes them in parallel, and merges the results.
            </p>

            <div style={{ display: 'flex', gap: '24px', marginBottom: '32px' }}>
              <button 
                onClick={runSortingSimulation} 
                disabled={sortState !== 'idle' && sortState !== 'completed'}
                className="btn btn-primary"
              >
                {sortState === 'idle' || sortState === 'completed' ? 'Generate Academic Rankings' : 'Analysis in Progress...'}
              </button>
              
              {sortState !== 'idle' && (
                <button 
                  onClick={() => { setSortState('idle'); setActiveThreads([]); setSortMetric(null); }}
                  className="btn"
                >
                  Reset Analysis
                </button>
              )}
            </div>

            <div style={{ display: 'grid', gridTemplateColumns: '1fr 340px', gap: '24px' }}>
              
              {/* Sorting Workspace */}
              <div className="visualizer-card" style={{ borderRadius: '12px' }}>
                <h3 style={{ fontSize: '20px', fontWeight: '600' }}>Analysis Pipeline Progress</h3>
                
                {sortState === 'idle' && (
                  <div style={{ padding: '40px 0', textAlign: 'center', color: 'var(--md-sys-color-on-surface-variant)' }}>
                    Ready to execute. Click "Generate Academic Rankings" to perform a parallel analysis and sort records by GPA.
                  </div>
                )}

                {/* Division Step */}
                {sortState === 'dividing' && (
                  <div style={{ borderLeft: '3px solid var(--md-sys-color-tertiary)', paddingLeft: '16px', margin: '20px 0' }}>
                    <div style={{ color: 'var(--md-sys-color-tertiary)', fontWeight: 'bold', fontSize: '13px' }}>
                      Stage 1: Cohort Division
                    </div>
                    <p style={{ fontSize: '14px', marginTop: '6px', color: 'var(--md-sys-color-on-surface)' }}>
                      Dividing student records into four balanced cohorts for parallel processing...
                    </p>
                  </div>
                )}

                {/* Sorting threads progress */}
                {sortState === 'sorting' && (
                  <div style={{ margin: '20px 0' }}>
                    <div style={{ color: 'var(--md-sys-color-secondary)', fontWeight: 'bold', fontSize: '13px', marginBottom: '16px' }}>
                      Stage 2: Parallel Performance Analysis
                    </div>
                    <div style={{ display: 'flex', flexDirection: 'column', gap: '16px' }}>
                      {activeThreads.map(thread => (
                        <div key={thread.id} style={{ background: 'var(--md-sys-color-surface-container-low)', padding: '16px', borderRadius: '12px', border: '1px solid var(--md-sys-color-outline-variant)' }}>
                          <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '13px', marginBottom: '8px' }}>
                            <span style={{ color: 'var(--md-sys-color-primary)', fontWeight: '600' }}>{thread.name}</span>
                            <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>Target: {thread.range}</span>
                          </div>
                          <div className="progress-bar-container">
                            <div 
                              className="progress-bar-fill"
                              style={{ 
                                width: `${thread.progress}%`
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
                  <div style={{ borderLeft: '3px solid var(--md-sys-color-primary)', paddingLeft: '16px', margin: '20px 0' }}>
                    <div style={{ color: 'var(--md-sys-color-primary)', fontWeight: 'bold', fontSize: '13px' }}>
                      Stage 3: Merging Cohort Data
                    </div>
                    <p style={{ fontSize: '14px', marginTop: '6px' }}>
                      All runners completed analysis. Consolidating sorted subdivisions into a single master rankings record...
                    </p>
                  </div>
                )}

                {/* Completed Results */}
                {sortState === 'completed' && sortMetric && (
                  <div style={{ border: '1px solid var(--md-sys-color-secondary)', padding: '16px', borderRadius: '12px', margin: '20px 0', backgroundColor: 'rgba(73, 104, 0, 0.02)' }}>
                    <div style={{ color: 'var(--md-sys-color-secondary)', fontWeight: 'bold', fontSize: '13px' }}>
                      Stage 4: Performance Sorting Complete
                    </div>
                    <p style={{ fontSize: '14px', marginTop: '6px' }}>
                      Successfully completed performance analysis and sorted student records.
                    </p>
                    <div style={{ fontSize: '13px', marginTop: '8px', color: 'var(--md-sys-color-on-surface-variant)' }}>
                      Consolidation latency: <span style={{ color: 'var(--md-sys-color-primary)', fontWeight: 'bold' }}>{sortMetric.durationMs} milliseconds</span>
                    </div>
                  </div>
                )}

                {/* Sorted list view */}
                {sortState !== 'idle' && (
                  <div style={{ marginTop: '24px' }}>
                    <h4 style={{ fontSize: '13px', color: 'var(--md-sys-color-on-surface-variant)', marginBottom: '12px', textTransform: 'uppercase', letterSpacing: '0.04em' }}>Academic Performance Standings</h4>
                    <div className="table-container">
                      <table className="data-table">
                        <thead>
                          <tr>
                            <th>Rank</th>
                            <th>Registry ID</th>
                            <th>Name</th>
                            <th>GPA</th>
                          </tr>
                        </thead>
                        <tbody>
                          {sortList.map((s, idx) => (
                            <tr key={s.rollNumber} className="data-row">
                              <td>{idx + 1}</td>
                              <td style={{ color: 'var(--md-sys-color-tertiary)' }}>{s.rollNumber}</td>
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
              <div className="visualizer-card" style={{ borderRadius: '12px' }}>
                <h3 style={{ fontSize: '20px', fontWeight: '600' }}>System Processing Analysis</h3>
                <div style={{ marginTop: '20px', fontSize: '13px', display: 'flex', flexDirection: 'column', gap: '16px' }}>
                  <div>
                    <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>Ranking Criteria:</span><br />
                    <strong>GPA (High to Low)</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>Parallel Analysis Tasks:</span><br />
                    <strong>4 Active Analysis Runners</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>Processing Infrastructure:</span><br />
                    <strong>Multi-Core Processor</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>Task Management:</span><br />
                    <strong style={{ color: 'var(--md-sys-color-secondary)' }}>Automated Parallel Joining</strong>
                  </div>
                </div>
              </div>

            </div>
          </div>
        )}

        {/* VIEW 8: Grade Search Index (Indexed Search Visualizer) */}
        {currentTab === 'indexing' && (
          <div>
            <h2 className="page-title" style={{ fontSize: '32px' }}>Academic Search Index</h2>
            <p className="page-description">
              Retrieves student records instantly using a multi-level index. The lookup locates the requested course and target grade without scanning the entire database.
            </p>

            <div className="visualizer-card" style={{ borderRadius: '12px' }}>
              <h3 style={{ fontSize: '20px', fontWeight: '600' }}>Grade Search Parameters</h3>
              
              <div style={{ display: 'flex', gap: '16px', marginTop: '20px', alignItems: 'flex-end' }}>
                <div className="form-group" style={{ margin: 0, width: '220px' }}>
                  <label>Course Offering</label>
                  <select 
                    className="form-control"
                    value={searchCourseCode}
                    onChange={(e) => setSearchCourseCode(e.target.value)}
                  >
                    <option value="CSE201">CSE201 (Apex Campus)</option>
                    <option value="CSE202">CSE202 (Apex Campus)</option>
                    <option value="MTH101">MTH101 (Apex Campus)</option>
                    <option value="ECE101">ECE101 (Apex Campus)</option>
                    <option value="MAT101">MAT101 (Apex Campus)</option>
                    <option value="HSS101">HSS101 (Apex Campus)</option>
                    <option value="CSE501">CSE501 (Apex Campus)</option>
                    <option value="RES001">RES001 (Apex Campus)</option>
                    <option value="101">101 (Vertex Campus)</option>
                    <option value="201">201 (Vertex Campus)</option>
                    <option value="305">305 (Vertex Campus)</option>
                    <option value="401">401 (Vertex Campus)</option>
                    <option value="505">505 (Vertex Campus)</option>
                    <option value="301">301 (Vertex Campus)</option>
                  </select>
                </div>

                <div className="form-group" style={{ margin: 0, width: '140px' }}>
                  <label>Grade Level</label>
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
                  style={{ height: '42px' }}
                >
                  Search Index
                </button>
              </div>
            </div>

            <div style={{ display: 'grid', gridTemplateColumns: '1fr 340px', gap: '24px' }}>
              
              {/* Lookup Path Visualization */}
              <div className="visualizer-card" style={{ borderRadius: '12px' }}>
                <h3 style={{ fontSize: '20px', fontWeight: '600' }}>Query Ingestion Steps</h3>

                {searchPathSteps.length === 0 && (
                  <div style={{ padding: '40px 0', textAlign: 'center', color: 'var(--md-sys-color-on-surface-variant)' }}>
                    Select a Course and Grade level, then click "Search Index" to see the lookup process.
                  </div>
                )}

                <div style={{ display: 'flex', flexDirection: 'column', gap: '16px', marginTop: '16px' }}>
                  {searchPathSteps.map((step, idx) => (
                    <div 
                      key={step.id} 
                      style={{ 
                        borderLeft: `3px solid ${idx === searchPathSteps.length - 1 ? 'var(--md-sys-color-primary)' : 'var(--md-sys-color-surface-variant)'}`,
                        paddingLeft: '16px',
                        paddingBottom: '4px'
                      }}
                    >
                      <div style={{ fontSize: '11px', color: 'var(--md-sys-color-tertiary)', textTransform: 'uppercase', fontWeight: 'bold' }}>
                        Step {step.id}: {step.title}
                      </div>
                      <div style={{ fontSize: '13px', color: 'var(--md-sys-color-on-background)', marginTop: '4px' }}>
                        {step.desc}
                      </div>
                    </div>
                  ))}
                </div>

                {searchPathProgress === 5 && (
                  <div style={{ marginTop: '24px' }}>
                    <h4 style={{ fontSize: '13px', color: 'var(--md-sys-color-on-surface-variant)', marginBottom: '12px', textTransform: 'uppercase', letterSpacing: '0.04em' }}>Matching Student Profiles</h4>
                    <div className="table-container">
                      <table className="data-table">
                        <thead>
                          <tr>
                            <th>Registry ID</th>
                            <th>Student Name</th>
                            <th>Branch</th>
                            <th>Level</th>
                            <th>Grade GP</th>
                          </tr>
                        </thead>
                        <tbody>
                          {searchResultStudents.map(student => (
                            <tr key={student.rollNumber} className="data-row">
                              <td style={{ color: 'var(--md-sys-color-tertiary)', fontWeight: 'bold' }}>{student.rollNumber}</td>
                              <td>{student.name}</td>
                              <td>{student.branch.code}</td>
                              <td>{student.level}</td>
                              <td style={{ color: 'var(--md-sys-color-secondary)', fontWeight: 'bold' }}>
                                {searchGradeLetter}
                              </td>
                            </tr>
                          ))}
                          {searchResultStudents.length === 0 && (
                            <tr>
                              <td colSpan="5" style={{ textAlign: 'center', padding: '24px', color: 'var(--md-sys-color-on-surface-variant)' }}>
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

              {/* Index Analysis Card */}
              <div className="visualizer-card" style={{ borderRadius: '12px' }}>
                <h3 style={{ fontSize: '20px', fontWeight: '600' }}>Search Index Performance</h3>
                <div style={{ marginTop: '20px', fontSize: '13px', display: 'flex', flexDirection: 'column', gap: '16px' }}>
                  <div>
                    <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>Indexing Architecture:</span><br />
                    <strong>Course-to-Grade Hierarchy</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>Primary Level:</span><br />
                    <span>Course Code indexing</span><br />
                    <strong style={{ color: 'var(--md-sys-color-tertiary)' }}>Accelerated search time</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>Secondary Level:</span><br />
                    <span>Letter Grade indexing</span><br />
                    <strong style={{ color: 'var(--md-sys-color-tertiary)' }}>Accelerated search time</strong>
                  </div>
                  <div>
                    <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>Target Records:</span><br />
                    <span>Compiling student lists</span><br />
                    <strong style={{ color: 'var(--md-sys-color-secondary)' }}>Instant profile mapping</strong>
                  </div>
                  <div style={{ borderTop: '1px solid var(--md-sys-color-outline-variant)', paddingTop: '16px' }}>
                    <span style={{ color: 'var(--md-sys-color-on-surface-variant)' }}>Total Search Efficiency:</span><br />
                    <strong style={{ fontSize: '16px', color: 'var(--md-sys-color-primary)' }}>Sub-millisecond instant lookup</strong>
                  </div>
                </div>
              </div>

            </div>
          </div>
        )}

      </main>

      {/* Mobile Bottom Navigation Bar */}
      <nav className="mobile-nav-panel">
        <button 
          className={`mobile-nav-item ${currentTab === 'dashboard' ? 'active' : ''}`}
          onClick={() => setCurrentTab('dashboard')}
        >
          <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'dashboard' ? "'FILL' 1" : "'FILL' 0" }}>dashboard_customize</span>
          <span>Hub</span>
        </button>
        <button 
          className={`mobile-nav-item ${currentTab === 'directory' ? 'active' : ''}`}
          onClick={() => setCurrentTab('directory')}
        >
          <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'directory' ? "'FILL' 1" : "'FILL' 0" }}>history_edu</span>
          <span>Ledger</span>
        </button>
        <button 
          className={`mobile-nav-item ${currentTab === 'faculty' ? 'active' : ''}`}
          onClick={() => setCurrentTab('faculty')}
        >
          <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'faculty' ? "'FILL' 1" : "'FILL' 0" }}>account_balance</span>
          <span>Faculty</span>
        </button>
        <button 
          className={`mobile-nav-item ${currentTab === 'catalog' ? 'active' : ''}`}
          onClick={() => setCurrentTab('catalog')}
        >
          <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'catalog' ? "'FILL' 1" : "'FILL' 0" }}>menu_book</span>
          <span>Catalog</span>
        </button>
        <button 
          className={`mobile-nav-item ${currentTab === 'fiscal' ? 'active' : ''}`}
          onClick={() => setCurrentTab('fiscal')}
        >
          <span className="material-symbols-outlined" style={{ fontVariationSettings: currentTab === 'fiscal' ? "'FILL' 1" : "'FILL' 0" }}>receipt_long</span>
          <span>Fiscal</span>
        </button>
      </nav>
    </div>
  );
}

export default App;
