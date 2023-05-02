#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>

using namespace std;

struct Employee{
        int Id;
        int age;
        char gender;
        int salary;
};
//加上“&” 將pass by value(要先copy一份) 變成pass by reference (效能提升)
//加上const可以確保裡面的值不被改變 （畢竟變成passbyreference）
bool cmp(const Employee &emp1 , const Employee&emp2 ){
    if(emp1.salary != emp2.salary)
        return emp1.salary < emp2.salary;
    
    if(emp1.age != emp2.age)
        return emp1.age < emp2.age;
    
    if(emp1.gender != emp2.gender)
        return emp1.gender == 'f';
    
    return emp1.Id < emp2.Id; 

}

int main(int argc, char **argv){
    const char* inputfile = argv[1];
    vector <Employee> employees;
    ifstream infile(inputfile , ios::in);

    int t_Id; int t_age; char t_gender , comma; int t_salary;

    while(infile>>t_Id>>comma>>t_age>>comma>>t_gender>>comma>>t_salary){
        Employee emp = {t_Id,t_age,t_gender,t_salary};
        employees.push_back(emp);
    }
    infile.close();
    //sort employeees in ascending order
    sort(employees.begin() , employees.end(), cmp);
    
    int current_salary = -1;
    for(const Employee& emp : employees){
        if(emp.salary != current_salary){
            if(current_salary != -1) cout<<endl;
            cout<<emp.salary<<",";
            current_salary = emp.salary;
        }
        else cout<<",";
        
        cout<<emp.Id;
    }
    cout<<endl;
    
    return 0;

}