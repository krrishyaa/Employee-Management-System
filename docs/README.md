Employee Management System (EMS) 🏢

Language: C++ | Database: SQLite | Platform: Cross-platform (Windows/Linux/macOS)

Overview

The Employee Management System (EMS) is a robust C++ console application that allows organizations to manage employee records efficiently. It provides secure login/signup functionality, comprehensive employee CRUD operations, and uses SQLite for persistent storage.

This project demonstrates modern C++ programming, database integration, and error handling, making it a strong portfolio piece for internships in software development.

Features ✨

User Authentication:

Secure Signup & Login system.

Password input is hidden for privacy. 🔒

Employee Management (CRUD):

Add, View, Update, and Delete employee records.

Validates inputs (ID uniqueness, age range, positive salary).

Database Integration:

Uses SQLite for persistent storage of users and employees.

Eliminates reliance on flat files (.txt), improving scalability and data integrity.

Error Handling & Validation:

Handles invalid inputs gracefully.

Prevents duplicate IDs and invalid entries.

Robust handling of database errors.

Database Schema 🗄️

Users Table:

Column	Type	Constraints
USERNAME	TEXT	PRIMARY KEY
PASSWORD	TEXT	NOT NULL

Employees Table:

Column	Type	Constraints
ID	INTEGER	PRIMARY KEY
NAME	TEXT	NOT NULL
AGE	INTEGER	NOT NULL
DEPARTMENT	TEXT	NOT NULL
SALARY	REAL	NOT NULL
How to Run ▶️

Clone the repository:

git clone https://github.com/<your-username>/Employee-Management-System.git


Ensure you have SQLite library (sqlite3.h) available and linked with your compiler.

Compile the C++ source code:

g++ main.cpp -lsqlite3 -o EMS


Run the executable:

./EMS

Future Improvements 🔧

Add role-based access (Admin vs Employee).

Implement report generation and search/filter for employee records.

Upgrade to a GUI interface using Qt or another framework.

Why This Project Stands Out 💼

Professional-level database integration using SQLite.

Error handling & validation ensures reliability.

Fully demonstrates C++ programming skills and project management.

Ideal for internship portfolios and interview demonstrations.