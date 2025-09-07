#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <conio.h>
#include "sqlite3.h" // The SQLite header

using namespace std;

// ========= Global Constants and Database Pointers =========
const string USERS_FILE = "users.txt"; // Not used directly, but kept for context
const string EMPLOYEES_FILE = "employees.txt"; // Not used directly
const string DATABASE_FILE = "employees.db";

sqlite3* db; // Global database connection pointer

// ========= Employee Structure =========
struct Employee {
    int id;
    string name;
    int age;
    string department;
    double salary;
};

// ========= Function Declarations =========
bool openDatabase();
void closeDatabase();
bool createTables();
string getPasswordInput();
bool signup();
bool login();
void showMenu();
bool isIdExists(int id);
void addEmployee();
void displayEmployees();
void updateEmployee();
void deleteEmployee();

// ========= MAIN FUNCTION =========
int main() {
    cout << "===== Employee Management System =====\n";
    
    if (!openDatabase()) {
        cout << "Database connection failed. Exiting...\n";
        return 1;
    }
    
    if (!createTables()) {
        cout << "Database table creation failed. Exiting...\n";
        closeDatabase();
        return 1;
    }

    int choice;
    cout << "1. Sign Up\n2. Login\nEnter choice: ";
    
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cout << "Invalid choice. Please enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (choice == 1) {
        if (signup()) {
            cout << "Signup successful! Please login.\n";
        } else {
            cout << "Signup failed.\n";
            closeDatabase();
            return 0;
        }
    }

    if (login()) {
        showMenu();
    } else {
        cout << "Login failed. Exiting...\n";
    }

    closeDatabase();
    return 0;
}

// ========= DATABASE FUNCTIONS =========
bool openDatabase() {
    int rc = sqlite3_open(DATABASE_FILE.c_str(), &db);
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    return true;
}

void closeDatabase() {
    sqlite3_close(db);
}

bool createTables() {
    const char* sql_employees =
        "CREATE TABLE IF NOT EXISTS Employees ("
        "ID INTEGER PRIMARY KEY,"
        "NAME TEXT NOT NULL,"
        "AGE INTEGER NOT NULL,"
        "DEPARTMENT TEXT NOT NULL,"
        "SALARY REAL NOT NULL);";

    const char* sql_users =
        "CREATE TABLE IF NOT EXISTS Users ("
        "USERNAME TEXT PRIMARY KEY NOT NULL,"
        "PASSWORD TEXT NOT NULL);";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql_employees, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error (Employees): " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    rc = sqlite3_exec(db, sql_users, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error (Users): " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// ========= HIDDEN PASSWORD INPUT =========
string getPasswordInput() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        } else {
            cout << '*';
            password += ch;
        }
    }
    return password;
}

// ========= SIGNUP =========
bool signup() {
    string username, password;
    cout << "Choose a username: ";
    cin >> username;
    cout << "Choose a password: ";
    password = getPasswordInput();
    cout << endl;

    string sql = "INSERT INTO Users (USERNAME, PASSWORD) VALUES ('" + username + "', '" + password + "');";
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// ========= LOGIN =========
bool login() {
    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    password = getPasswordInput();
    cout << endl;

    string sql = "SELECT USERNAME FROM Users WHERE USERNAME = '" + username + "' AND PASSWORD = '" + password + "';";
    
    bool authenticated = false;
    auto callback = [](void* data, int argc, char** argv, char** azColName) {
        bool* auth = static_cast<bool*>(data);
        *auth = true;
        return 0;
    };

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, &authenticated, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    return authenticated;
}

// ========= MENU =========
void showMenu() {
    int choice;
    do {
        cout << "\n===== Employee Menu =====\n";
        cout << "1. Add Employee\n";
        cout << "2. Display Employees\n";
        cout << "3. Update Employee\n";
        cout << "4. Delete Employee\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number from 1 to 5: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: addEmployee(); break;
            case 2: displayEmployees(); break;
            case 3: updateEmployee(); break;
            case 4: deleteEmployee(); break;
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 5);
}

// ========= HELPER: CHECK FOR ID =========
bool isIdExists(int id) {
    string sql = "SELECT ID FROM Employees WHERE ID = " + to_string(id) + ";";
    bool exists = false;
    auto callback = [](void* data, int argc, char** argv, char** azColName) {
        bool* ex = static_cast<bool*>(data);
        *ex = true;
        return 0;
    };
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, &exists, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    return exists;
}

// ========= ADD EMPLOYEE =========
void addEmployee() {
    Employee emp;
    cout << "Enter ID: ";
    while (!(cin >> emp.id) || emp.id <= 0 || isIdExists(emp.id)) {
        if (cin.fail()) {
            cout << "Invalid input. Please enter a positive number for ID: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else if (emp.id <= 0) {
            cout << "ID must be a positive number. Please enter a valid ID: ";
        } else {
            cout << "Error: An employee with this ID already exists. Please use a unique ID: ";
        }
        cin >> emp.id;
    }

    cout << "Enter Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, emp.name);
    
    cout << "Enter Age: ";
    while (!(cin >> emp.age) || emp.age <= 18 || emp.age >= 100) {
        cout << "Invalid age. Please enter a number between 18 and 100: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter Department: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, emp.department);
    
    cout << "Enter Salary: ";
    while (!(cin >> emp.salary) || emp.salary <= 0) {
        cout << "Invalid salary. Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string sql = "INSERT INTO Employees (ID, NAME, AGE, DEPARTMENT, SALARY) VALUES ("
               + to_string(emp.id) + ", '" + emp.name + "', "
               + to_string(emp.age) + ", '" + emp.department + "', "
               + to_string(emp.salary) + ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Employee added successfully!\n";
    }
}

// ========= DISPLAY EMPLOYEES =========
void displayEmployees() {
    string sql = "SELECT * FROM Employees;";
    auto callback = [](void* data, int argc, char** argv, char** azColName) {
        cout << "ID: " << argv[0]
             << " | Name: " << argv[1]
             << " | Age: " << argv[2]
             << " | Department: " << argv[3]
             << " | Salary: " << argv[4] << "\n";
        return 0;
    };
    
    cout << "\n--- Employee Records ---\n";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

// ========= UPDATE EMPLOYEE =========
void updateEmployee() {
    int targetId;
    cout << "Enter Employee ID to update: ";
    while (!(cin >> targetId)) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (!isIdExists(targetId)) {
        cout << "Employee not found!\n";
        return;
    }

    string newName, newDepartment;
    int newAge;
    double newSalary;

    cout << "Enter new Name: ";
    cin.ignore();
    getline(cin, newName);
    
    cout << "Enter new Age: ";
    while (!(cin >> newAge) || newAge <= 18 || newAge >= 100) {
        cout << "Invalid age. Please enter a number between 18 and 100: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    cout << "Enter new Department: ";
    cin.ignore();
    getline(cin, newDepartment);
    
    cout << "Enter new Salary: ";
    while (!(cin >> newSalary) || newSalary <= 0) {
        cout << "Invalid salary. Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string sql = "UPDATE Employees SET NAME = '" + newName + "', AGE = " + to_string(newAge) + 
                 ", DEPARTMENT = '" + newDepartment + "', SALARY = " + to_string(newSalary) + 
                 " WHERE ID = " + to_string(targetId) + ";";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Employee updated successfully!\n";
    }
}

// ========= DELETE EMPLOYEE =========
void deleteEmployee() {
    int targetId;
    cout << "Enter Employee ID to delete: ";
    while (!(cin >> targetId)) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (!isIdExists(targetId)) {
        cout << "Employee not found!\n";
        return;
    }

    string sql = "DELETE FROM Employees WHERE ID = " + to_string(targetId) + ";";
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Employee deleted successfully!\n";
    }
}