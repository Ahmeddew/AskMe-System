# AskMe System

A fully functional console-based Q&A application built in C++ utilizing Object-Oriented Programming (OOP) principles and a file-based storage architecture.

## Features
- **User Authentication:** Multi-user secure registration and login system.
- **Session Management:** Tracks active session state and validates user actions dynamically.
- **Threaded Questions:** Supports standalone inquiries as well as nested conversation replies using map hierarchies.
- **Database Persistence:** Custom text parsing layer that safely flushes memory objects to disk storage using relative file handling.

## Project Structure
- `main.cpp`: Contains the core application engine.
- `users.txt` / `questions.txt`: Automated local data persistence layers (git-ignored).
