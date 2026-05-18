 AskMe System

A high-performance, console-based Q&A application built in C++ utilizing Object-Oriented Programming (OOP) principles, a layered architecture, and an automated text-file persistence engine.

## 🚀 Architectural Overview

The project is structured around a decoupled **Three-Tier Architecture** that enforces a strict separation of concerns between data representations, sub-system business logic, and runtime state coordination.

 
       ┌───────────────────────────────────────────────────┐
       │             AskMe Orchestrator Engine             │
       └─────────────────────────┬─────────────────────────┘
                                 │ (Session Context & Event Loop)
            ┌────────────────────┴────────────────────┐
            ▼                                         ▼
┌───────────────────────┐                 ┌───────────────────────┐
│     User Manager      │                 │   Question Manager    │
├───────────────────────┤                 ├───────────────────────┤
│ ▪ InMemory DB (Map)   │                 │ ▪ InMemory DB (Map)   │
│ ▪ Flat-File Disk Sync │                 │ ▪ Threading (Adjacency│
└───────────────────────┘                 │   List Map Layer)     │
                                          └───────────────────────┘
1. The Entity Layer (User & Question)Encapsulation: Pure data objects wrapping raw system primitives.State Safety: Private attributes accessible exclusively via explicit getter and setter interfaces.
2. The Management Layer (UserManger & QuestionManger)State Decoupling: Each manager acts as an independent execution sandbox managing its own in-memory cache and isolated flat-file database allocations (users.txt and questions.txt).Dependency Injection: The QuestionManger dynamically ingests a reference to the UserManger instance during runtime event handlers to safely cross-validate system states (e.g., identity verification, checking anonymous message permissions).
3. The Orchestration Layer (AskMe)Central Controller Pattern: Acts as the application's central switchboard. It controls the global lifecycle and isolates the Unauthenticated Gatekeeper Loop (Login/Signup/Exit) from the Authenticated Session Dashboard Loop.Session Context Management: Tracks the currently authenticated user (userManger.current_user) and acts as a state router, passing session tokens down to the underlying manager methods.
🛠️ Key Technical Highlights & Data StructuresIn-Memory Cache Hydration: Utilizes std::map<int, Entity> structures to guarantee $O(\log n)$ lookup and modification speeds during runtime execution, preventing expensive disk reads during interactive cycles.Algorithmic Threading Representation: Nested question threads and replies are managed using an adjacency list structure inside an in-memory map: std::map<int, std::vector<int>> Question_threads (Parent ID $\rightarrow$ List of Reply IDs). This enables smooth recursive or prioritized feed rendering.Cascading Deletion Engine: Implements a localized reference dependency cleanup step. If a user deletes a root question, the system automatically finds all child comment references inside the adjacency map and wipes out the dependent records from the database.Stream Serialization Protocols: Text entries are parsed safely by a serialization utility that intercepts whitespace inputs, normalizes them into bounded string structures via non-colliding token placeholders ($), and saves them cleanly onto disk.
💻 Technical StackLanguage: C++17 / C++20Build Automation: CMakeFile Database Systems: Standard Template Library (fstream File I/O Streams)Data Structures: std::map, std::vector, std::pair
📋 Features[x] Secure User Onboarding: System rejects registration requests for duplicate usernames, manages structural incremental hashing for IDs, and configures user privacy settings.[x] Privacy Access Control: Senders can choose to remain anonymous. The system enforces target-user preferences—if a recipient blocks anonymous inputs, the system overrides the flag and forces sender identity visibility.[x] Self-Interaction Guardrails: Prevents logic errors by restricting users from sending messages or questions to themselves.[x] Two-Way Visibility Tracking: Independent views to pull lists of questions sent to the active session or generated from the active session.[x] Global Activity Feed: Renders an application-wide public dashboard showing fully constructed conversation timelines and nesting responses underneath root questions.
🔧 Installation & Local SetupPrerequisitesEnsure you have a C++ compiler installed (GCC/MinGW, Clang, or MSVC) along with CMake.Build and Run via TerminalClone the project code to your local machine:Bashgit clone [https://github.com/Ahmed10969/AskMe-System.git](https://github.com/Ahmed10969/AskMe-System.git)
cd AskMe-System
