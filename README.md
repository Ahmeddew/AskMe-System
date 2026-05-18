# AskMe System

A high-performance, console-based Q&A application built in C++ utilizing Object-Oriented Programming (OOP) principles, a decoupled layered architecture, and an automated flat-file persistence layer.

---

## 🚀 Architectural Overview

The system is engineered using a **Three-Tier Architecture** that enforces a strict separation of concerns between data encapsulation, sub-system business logic, and runtime execution coordination.

### 1. The Entity Layer (`User` & `Question`)
* **Encapsulation:** Pure data representations wrapping application state primitives.
* **State Integrity:** All private attributes are strictly protected and modified exclusively via explicit public getter and setter interfaces.

### 2. The Management Layer (`UserManger` & `QuestionManger`)
* **State Decoupling:** Each manager acts as an independent execution sandbox managing its own in-memory cache and isolated flat-file database allocations (`users.txt` and `questions.txt`).
* **Dependency Injection:** The `QuestionManger` dynamically ingests a reference to the `UserManger` instance during runtime event handlers to cross-validate cross-subsystem data (e.g., identity verification, checking anonymous message permissions).

### 3. The Orchestration Layer (`AskMe`)
* **Central Controller Pattern:** Acts as the main runtime switchboard. It controls the global lifecycle and cleanly isolates the **Unauthenticated Gatekeeper Loop** (Login/Signup/Exit) from the **Authenticated Session Dashboard Loop**.
* **Session Context Management:** Tracks the currently authenticated user session (`userManger.current_user`) and safely routes context down to the underlying manager methods.

---

## 🛠️ Key Technical Highlights & Data Structures

* **In-Memory Cache Hydration:** Utilizes `std::map<int, Entity>` structures to guarantee $O(\log n)$ lookup and modification speeds during runtime execution, completely eliminating expensive disk I/O bottlenecks during active user sessions.
* **Algorithmic Threading Representation:** Nested question threads and replies are modeled using an adjacency list structure inside an in-memory map: `std::map<int, std::vector<int>> Question_threads` (Parent ID $\rightarrow$ List of Reply IDs). This enables seamless nested timeline rendering.
* **Cascading Deletion Engine:** Implements a strict reference-dependency cleanup loop. When a user deletes a root question, the system automatically runs an internal cascade loop to sweep and purge all child comment entries from the database map.
* **Stream Serialization Protocols:** Text entries are sanitized by a custom serialization utility that replaces whitespace tokens with non-colliding characters (`$`) before saving to disk, ensuring that raw standard spaces do not corrupt the `std::fstream` sequential read alignment.

---

## 💻 Technical Stack

- **Language:** C++17 / C++20
- **Build Automation:** CMake
- **File Database Systems:** Standard Template Library (`fstream` File I/O Streams)
- **Data Structures:** `std::map`, `std::vector`, `std::pair`

---

## 📋 Core System Features

- [x] **Secure User Onboarding:** Restricts duplicate username creation, handles automated incremental primary key generations, and configures user privacy settings.
- [x] **Privacy Access Control:** Senders can opt for anonymous postings. However, the system strictly respects recipient preferences—if a user blocks anonymous entries, the system automatically overrides the sender flag and displays their ID.
- [x] **Self-Interaction Guardrails:** Includes a localized security check that prevents users from sending questions to their own account IDs.
- [x] **Bidirectional Tracking:** Independent dash pipelines to track questions explicitly sent *to* the active session or generated *from* the current user.
- [x] **Global Activity Feed:** Renders a public system-wide dashboard displaying fully constructed conversation timelines and nesting responses hierarchically.

---

