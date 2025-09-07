#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ========= Employee Structure =========
struct Employee {
    int id;
    string name;
    int age;
    string department;
    double salary;
};

// ========= Function Declarations =========
bool signup();
bool login();
void showMenu();
void addEmployee();
void displayEmployees();
void updateEmployee();
void deleteEmployee();

// ========= Main =========
int main() {
    int choice;
    cout << "===== Welcome to Employee Management System =====\n";
    cout << "1. Sign Up\n2. Login\nChoose: ";
    cin >> choice;

    bool accessGranted = false;

    if (choice == 1) {
        accessGranted = signup();
    } else if (choice == 2) {
        accessGranted = login();
    } else {
        cout << "Invalid choice!\n";
        return 0;
    }

    if (!accessGranted) {
        cout << "Access denied. Exiting program...\n";
        return 0;
    }

    // If login/signup successful → show employee menu
    showMenu();

    return 0;
}

// ========= Signup =========
bool signup() {
    string username, password;
    cout << "\n=== Sign Up ===\n";
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter new password: ";
    cin >> password;

    ofstream file("../data/users.txt", ios::app); // save credentials
    if (!file) {
        cout << "Error opening users file!\n";
        return false;
    }
    file << username << " " << password << "\n";
    file.close();

    cout << "✅ Signup successful! You can now login.\n";
    return true;
}

// ========= Login =========
bool login() {
    string username, password, u, p;
    cout << "\n=== Login ===\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream file("../data/users.txt");
    if (!file) {
        cout << "Error: No users registered yet!\n";
        return false;
    }

    while (file >> u >> p) {
        if (u == username && p == password) {
            cout << "✅ Login successful! Welcome, " << username << ".\n";
            return true;
        }
    }

    cout << "❌ Invalid username or password!\n";
    return false;
}

// ========= Employee Menu =========
void showMenu() {
    int choice;
    do {
        cout << "\n===== Employee Management Menu =====\n";
        cout << "1. Add Employee\n";
        cout << "2. Display Employees\n";
        cout << "3. Update Employee\n";
        cout << "4. Delete Employee\n";
        cout << "5. Logout & Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1: addEmployee(); break;
            case 2: displayEmployees(); break;
            case 3: updateEmployee(); break;
            case 4: deleteEmployee(); break;
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while(choice != 5);
}

// ========= Add Employee =========
void addEmployee() {
    Employee emp;
    cout << "\n=== Add Employee ===\n";
    cout << "Enter ID: ";
    cin >> emp.id;
    cout << "Enter Name: ";
    cin >> emp.name;
    cout << "Enter Age: ";
    cin >> emp.age;
    cout << "Enter Department: ";
    cin >> emp.department;
    cout << "Enter Salary: ";
    cin >> emp.salary;

    ofstream file("../data/employees.txt", ios::app);
    if (!file) {
        cout << "Error opening employees file!\n";
        return;
    }

    file << emp.id << " " << emp.name << " " << emp.age << " "
         << emp.department << " " << emp.salary << "\n";
    file.close();

    cout << "✅ Employee added successfully!\n";
}

// ========= Display Employees =========
void displayEmployees() {
    ifstream file("../data/employees.txt");
    if (!file) {
        cout << "❌ No employee records found!\n";
        return;
    }

    Employee emp;
    cout << "\n=== Employee Records ===\n";
    while (file >> emp.id >> emp.name >> emp.age >> emp.department >> emp.salary) {
        cout << "ID: " << emp.id
             << ", Name: " << emp.name
             << ", Age: " << emp.age
             << ", Department: " << emp.department
             << ", Salary: " << emp.salary << "\n";
    }
    file.close();
}

// ========= Update Employee =========
void updateEmployee() {
    int id;
    cout << "\n=== Update Employee ===\n";
    cout << "Enter Employee ID to update: ";
    cin >> id;

    ifstream inFile("../data/employees.txt");
    if (!inFile) {
        cout << "❌ No employee records found!\n";
        return;
    }

    ofstream tempFile("../data/temp.txt");
    Employee emp;
    bool found = false;

    while (inFile >> emp.id >> emp.name >> emp.age >> emp.department >> emp.salary) {
        if (emp.id == id) {
            found = true;
            cout << "Enter new Name: ";
            cin >> emp.name;
            cout << "Enter new Age: ";
            cin >> emp.age;
            cout << "Enter new Department: ";
            cin >> emp.department;
            cout << "Enter new Salary: ";
            cin >> emp.salary;
        }
        tempFile << emp.id << " " << emp.name << " " << emp.age << " "
                 << emp.department << " " << emp.salary << "\n";
    }

    inFile.close();
    tempFile.close();

    remove("../data/employees.txt");
    rename("../data/temp.txt", "../data/employees.txt");

    if (found)
        cout << "✅ Employee updated successfully!\n";
    else
        cout << "❌ Employee ID not found!\n";
}

// ========= Delete Employee =========
void deleteEmployee() {
    int id;
    cout << "\n=== Delete Employee ===\n";
    cout << "Enter Employee ID to delete: ";
    cin >> id;

    ifstream inFile("../data/employees.txt");
    if (!inFile) {
        cout << "❌ No employee records found!\n";
        return;
    }

    ofstream tempFile("../data/temp.txt");
    Employee emp;
    bool found = false;

    while (inFile >> emp.id >> emp.name >> emp.age >> emp.department >> emp.salary) {
        if (emp.id == id) {
            found = true;
            cout << "🗑️ Employee " << emp.name << " (ID " << emp.id << ") deleted.\n";
            continue; // skip writing this employee
        }
        tempFile << emp.id << " " << emp.name << " " << emp.age << " "
                 << emp.department << " " << emp.salary << "\n";
    }

    inFile.close();
    tempFile.close();

    remove("../data/employees.txt");
    rename("../data/temp.txt", "../data/employees.txt");

    if (!found)
        cout << "❌ Employee ID not found!\n";
}