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

    // Static comparison function for sorting
    static bool cmp(const Employee &emp1, const Employee &emp2) {
        if (emp1.salary != emp2.salary)
            return emp1.salary < emp2.salary;

        if (emp1.age != emp2.age)
            return emp1.age < emp2.age;

        if (emp1.gender != emp2.gender)
            return emp1.gender == 'f';

        return emp1.Id < emp2.Id;
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
    // Sort employees in ascending order using the static comparison function
    sort(employees.begin(), employees.end(), Employee::cmp);

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