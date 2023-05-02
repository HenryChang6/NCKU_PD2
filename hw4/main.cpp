#include<iostream>
#include<set>
#include<string>
#include<sstream>
#include<fstream>
#include<ctime>
#include<map>
#include<vector>
#include "employee.h"


using namespace std;

map<int,Employee> getAllData(char*file_name[]);
void solve(char*file_name[]);
vector<Employee> change_map_into_sorted_vector(map<int,Employee> employees);
void output(vector<Employee> employees);

int main(int argc , char*argv[]){
    solve(argv);
    return 0;
}

void solve(char*file_name[]){
    map<int,Employee> employees = getAllData(file_name);
    for(auto it = employees.begin(); it != employees.end(); ++it){
        it->second.count_max_conti_days();
    }
    vector<Employee> sorted_employees = change_map_into_sorted_vector(employees);
    output(sorted_employees);
}

void output(vector<Employee> employees){
    for(int i = 0; i < 3; ++i){
        cout<<employees[i].get_Id()<<","<<employees[i].get_max_conti_days()<<","<<employees[i].get_from_day()<<","<<employees[i].get_end_day()<<endl;
    }
}

map<int,Employee> getAllData(char*file_name[]){
    map<int,Employee> employees;
    ifstream inputFile(file_name[1],ios::in);
    string line_input;
    while(inputFile>>line_input){
        string date_str ,id_str , pass; 
        tm date;
        int id;
        stringstream ss(line_input);
        getline(ss, id_str,',');                 
        getline(ss, pass,',');
        getline(ss, date_str);
        id = stoi(id_str);
        date.tm_year = stoi(date_str.substr(0,4))-1900;
        //tm_mom是從0到11 因此要多減一
        date.tm_mon = stoi(date_str.substr(4,2)) - 1; 
        date.tm_mday = stoi(date_str.substr(6,2)); 
        date.tm_hour = 0;
        date.tm_min = 0;
        date.tm_sec = 0;
        date.tm_isdst = -1;
        time_t date_time = mktime(&date);
        ss.clear(); 

        if(employees.find(id) == employees.end()){
            employees[id] = Employee(id,date_time);
        }else{
            employees[id].add_new_date(date_time);
        }
    }
    return employees;
}

bool max_days_descending_then_id_ascending(Employee &a , Employee &b){
    if(a.get_max_conti_days() != b.get_max_conti_days()){
        return(a.get_max_conti_days() > b.get_max_conti_days());
    }
    return(a.get_Id()< b.get_Id());
}

vector<Employee> change_map_into_sorted_vector(map<int,Employee> employees){
    vector<Employee> sorted_employees;
    for(const auto &employee : employees){
        sorted_employees.push_back(employee.second);
    }
    sort(sorted_employees.begin(),sorted_employees.end(),max_days_descending_then_id_ascending);
    return sorted_employees;
}

