#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>

using namespace std;

// Define Employee class
class Employee {
public:
    // Declare member variables
    int Id;
    int age;
    char gender;
    int salary;

    // Constructor to initialize member variables
    Employee(int t_Id, int t_age, char t_gender, int t_salary)
        : Id(t_Id), age(t_age), gender(t_gender), salary(t_salary) {}

    // Overload the less-than operator for the Employee class
    bool operator<(const Employee& other) const {
        if (salary != other.salary)
            return salary < other.salary;

        if (age != other.age)
            return age < other.age;

        if (gender != other.gender)
            return gender == 'f';

        return Id < other.Id;
    }
};

int main(int argc, char **argv) {
    // Get input file name from command line arguments
    const char* inputfile = argv[1];
    // Declare a vector to store Employee objects
    vector<Employee> employees;
    // Open the input file
    ifstream infile(inputfile, ios::in);

    // Declare temporary variables to store data from the input file
    int t_Id; int t_age; char t_gender, comma; int t_salary;

    // Read data from the input file and create Employee objects
    while (infile >> t_Id >> comma >> t_age >> comma >> t_gender >> comma >> t_salary) {
        Employee emp(t_Id, t_age, t_gender, t_salary);
        employees.push_back(emp);
    }
    // Close the input file
    infile.close();
    // Sort employees in ascending order using the overloaded less-than operator
    sort(employees.begin(), employees.end());

    // Initialize a variable to store the current salary
    int current_salary = -1;
    // Iterate through the sorted employees and print their information
    for (const Employee& emp : employees) {
        if (emp.salary != current_salary) {
            if (current_salary != -1) cout << endl;
            cout << emp.salary << ",";
            current_salary = emp.salary;
        }
        else cout << ",";

        cout << emp.Id;
    }

    cout << endl;

    return 0;
}
