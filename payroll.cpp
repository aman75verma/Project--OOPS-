#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <limits>
using namespace std;

// Employee structure to store employee details
struct Employee {
    int code;
    char name[50];
    char address[100];
    char phone[15];
    int day, month, year;
    char designation[50];
    char grade;
    char house;
    char convense;
    float loan;
    float basicSalary;
};

// Function to clear the screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Function to display the main menu
void displayMenu() {
    clearScreen();
    cout << "\n\033[1;36mPayroll Management System\033[0m\n"; // Cyan color for heading
    cout << "1. Add New Employee\n";
    cout << "2. Display Employee Details\n";
    cout << "3. List All Employees\n";
    cout << "4. Delete Employee\n";
    cout << "5. Generate Salary Slip\n";
    cout << "6. Search Employee by Name\n";
    cout << "7. Edit Employee Details\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

// Function to validate date
bool isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > 29 || (day == 29 && !isLeap)) return false;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }
    return true;
}

// Function to add a new employee
void addEmployee() {
    Employee emp;
    cout << "\nEnter Employee Code: ";
    cin >> emp.code;
    cin.ignore();
    cout << "Enter Name: ";
    cin.getline(emp.name, 50);
    cout << "Enter Address: ";
    cin.getline(emp.address, 100);
    cout << "Enter Phone: ";
    cin.getline(emp.phone, 15);

    // Validate date of joining
    bool validDate = false;
    while (!validDate) {
        cout << "Enter Date of Joining (DD MM YYYY): ";
        cin >> emp.day >> emp.month >> emp.year;
        validDate = isValidDate(emp.day, emp.month, emp.year);
        if (!validDate) {
            cout << "\033[1;31mInvalid date! Please try again.\033[0m\n"; // Red color for error
        }
    }
    cin.ignore();

    cout << "Enter Designation: ";
    cin.getline(emp.designation, 50);
    cout << "Enter Grade (A/B/C/D/E): ";
    cin >> emp.grade;
    cout << "House Allowance (Y/N): ";
    cin >> emp.house;
    cout << "Convense Allowance (Y/N): ";
    cin >> emp.convense;
    cout << "Enter Loan Amount: ";
    cin >> emp.loan;
    cout << "Enter Basic Salary: ";
    cin >> emp.basicSalary;

    // Save employee to file
    ofstream file("employees.dat", ios::app | ios::binary);
    if (!file) {
        cerr << "\033[1;31mError opening file!\033[0m\n";
        return;
    }
    file.write(reinterpret_cast<char*>(&emp), sizeof(Employee));
    file.close();
    cout << "\033[1;32mEmployee added successfully!\033[0m\n"; // Green color for success
    cin.ignore();
    cin.get();
}

// Function to display employee details
void displayEmployee() {
    int code;
    cout << "\nEnter Employee Code: ";
    cin >> code;

    ifstream file("employees.dat", ios::in | ios::binary);
    if (!file) {
        cerr << "\033[1;31mError opening file!\033[0m\n";
        return;
    }

    Employee emp;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        if (emp.code == code) {
            cout << "\n\033[1;36mEmployee Details:\033[0m\n";
            cout << "Code: " << emp.code << "\n";
            cout << "Name: " << emp.name << "\n";
            cout << "Address: " << emp.address << "\n";
            cout << "Phone: " << emp.phone << "\n";
            cout << "Date of Joining: " << emp.day << "/" << emp.month << "/" << emp.year << "\n";
            cout << "Designation: " << emp.designation << "\n";
            cout << "Grade: " << emp.grade << "\n";
            cout << "House Allowance: " << emp.house << "\n";
            cout << "Convense Allowance: " << emp.convense << "\n";
            cout << "Loan: " << emp.loan << "\n";
            cout << "Basic Salary: " << emp.basicSalary << "\n";
            found = true;
            break;
        }
    }
    file.close();

    if (!found) {
        cout << "\033[1;31mEmployee not found!\033[0m\n";
    }
    cin.ignore();
    cin.get();
}

// Function to list all employees
void listEmployees() {
    ifstream file("employees.dat", ios::in | ios::binary);
    if (!file) {
        cerr << "\033[1;31mError opening file!\033[0m\n";
        return;
    }

    Employee emp;
    cout << "\n\033[1;36mList of Employees:\033[0m\n";
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        cout << "\nEmployee Code: " << emp.code << "\n";
        cout << "Name: " << emp.name << "\n";
        cout << "Designation: " << emp.designation << "\n";
        cout << "Grade: " << emp.grade << "\n";
        cout << "Basic Salary: " << emp.basicSalary << "\n";
        cout << "------------------------\n";
    }
    file.close();
    cin.ignore();
    cin.get();
}

// Function to delete an employee
void deleteEmployee() {
    int code;
    cout << "\nEnter Employee Code to delete: ";
    cin >> code;

    ifstream inFile("employees.dat", ios::in | ios::binary);
    ofstream outFile("temp.dat", ios::out | ios::binary);
    if (!inFile || !outFile) {
        cerr << "\033[1;31mError opening file!\033[0m\n";
        return;
    }

    Employee emp;
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        if (emp.code != code) {
            outFile.write(reinterpret_cast<char*>(&emp), sizeof(Employee));
        } else {
            found = true;
        }
    }
    inFile.close();
    outFile.close();

    if (found) {
        remove("employees.dat");
        rename("temp.dat", "employees.dat");
        cout << "\033[1;32mEmployee deleted successfully!\033[0m\n";
    } else {
        cout << "\033[1;31mEmployee not found!\033[0m\n";
        remove("temp.dat");
    }
    cin.ignore();
    cin.get();
}

// Function to generate salary slip
void generateSalarySlip() {
    int code;
    cout << "\nEnter Employee Code: ";
    cin >> code;

    ifstream file("employees.dat", ios::in | ios::binary);
    if (!file) {
        cerr << "\033[1;31mError opening file!\033[0m\n";
        return;
    }

    Employee emp;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        if (emp.code == code) {
            cout << "\n\033[1;36mSalary Slip:\033[0m\n";
            cout << "Name: " << emp.name << "\n";
            cout << "Designation: " << emp.designation << "\n";
            cout << "Basic Salary: " << emp.basicSalary << "\n";
            cout << "Loan Deduction: " << emp.loan << "\n";
            cout << "Net Salary: " << emp.basicSalary - emp.loan << "\n";
            found = true;
            break;
        }
    }
    file.close();

    if (!found) {
        cout << "\033[1;31mEmployee not found!\033[0m\n";
    }
    cin.ignore();
    cin.get();
}

// Function to search employee by name
void searchEmployeeByName() {
    char name[50];
    cout << "\nEnter Employee Name: ";
    cin.ignore();
    cin.getline(name, 50);

    ifstream file("employees.dat", ios::in | ios::binary);
    if (!file) {
        cerr << "\033[1;31mError opening file!\033[0m\n";
        return;
    }

    Employee emp;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        if (strcmp(emp.name, name) == 0) {
            cout << "\n\033[1;36mEmployee Details:\033[0m\n";
            cout << "Code: " << emp.code << "\n";
            cout << "Name: " << emp.name << "\n";
            cout << "Designation: " << emp.designation << "\n";
            cout << "Grade: " << emp.grade << "\n";
            cout << "Basic Salary: " << emp.basicSalary << "\n";
            found = true;
            break;
        }
    }
    file.close();

    if (!found) {
        cout << "\033[1;31mEmployee not found!\033[0m\n";
    }
    cin.get();
}

// Function to edit employee details
void editEmployee() {
    int code;
    cout << "\nEnter Employee Code to edit: ";
    cin >> code;

    fstream file("employees.dat", ios::in | ios::out | ios::binary);
    if (!file) {
        cerr << "\033[1;31mError opening file!\033[0m\n";
        return;
    }

    Employee emp;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        if (emp.code == code) {
            cout << "\nCurrent Details:\n";
            cout << "Name: " << emp.name << "\n";
            cout << "Address: " << emp.address << "\n";
            cout << "Phone: " << emp.phone << "\n";
            cout << "Designation: " << emp.designation << "\n";
            cout << "Grade: " << emp.grade << "\n";
            cout << "House Allowance: " << emp.house << "\n";
            cout << "Convense Allowance: " << emp.convense << "\n";
            cout << "Loan: " << emp.loan << "\n";
            cout << "Basic Salary: " << emp.basicSalary << "\n";

            cout << "\nEnter New Details:\n";
            cout << "Enter Name: ";
            cin.ignore();
            cin.getline(emp.name, 50);
            cout << "Enter Address: ";
            cin.getline(emp.address, 100);
            cout << "Enter Phone: ";
            cin.getline(emp.phone, 15);
            cout << "Enter Designation: ";
            cin.getline(emp.designation, 50);
            cout << "Enter Grade (A/B/C/D/E): ";
            cin >> emp.grade;
            cout << "House Allowance (Y/N): ";
            cin >> emp.house;
            cout << "Convense Allowance (Y/N): ";
            cin >> emp.convense;
            cout << "Enter Loan Amount: ";
            cin >> emp.loan;
            cout << "Enter Basic Salary: ";
            cin >> emp.basicSalary;

            file.seekp(-static_cast<int>(sizeof(Employee)), ios::cur);
            file.write(reinterpret_cast<char*>(&emp), sizeof(Employee));
            found = true;
            break;
        }
    }
    file.close();

    if (found) {
        cout << "\033[1;32mEmployee details updated successfully!\033[0m\n";
    } else {
        cout << "\033[1;31mEmployee not found!\033[0m\n";
    }
    cin.ignore();
    cin.get();
}

int main() {
    int choice;
    do {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                addEmployee();
                break;
            case 2:
                displayEmployee();
                break;
            case 3:
                listEmployees();
                break;
            case 4:
                deleteEmployee();
                break;
            case 5:
                generateSalarySlip();
                break;
            case 6:
                searchEmployeeByName();
                break;
            case 7:
                editEmployee();
                break;
            case 8:
                cout << "\033[1;32mExiting...\033[0m\n";
                break;
            default:
                cout << "\033[1;31mInvalid choice! Try again.\033[0m\n";
                cin.ignore();
                cin.get();
        }
    } while (choice != 8);

    return 0;
}