#include<iostream>
#include<string>
#include<map>
#include<sstream>
#include<fstream>
#include<cstdlib>

using namespace std;

class Employee{
    private:
        int id;
        map<int,int>date_time;
        int overloading_days = 0;
    public:
        //constructor
        Employee(){
            return;
        }
        Employee (int id_ , int date_ , int time_){
            id = id_;
            date_time[date_] = time_;
        }
        void add_and_count_overloading(int date_, int time_){
            if(date_time.find(date_) != date_time.end()){  //date相同 可以計算
                if((abs(time_ - date_time[date_]))>480){
                    overloading_days+=1;
                }
                date_time.erase(date_);
            }else{ //date不同 不可計算 加進map中
                date_time[date_] = time_;
            }
        }
        int get_forgoten_days(void){
            return date_time.size();
        }
        int get_overloading_days(void){
            return overloading_days;
        }
};

map<int , Employee> getAllData(char*file_name[]){
    map<int , Employee> employees;
    ifstream inputFile(file_name[1],ios::in);
    string line_input;
    while(inputFile>>line_input){
        string date_time_str ,id_str , pass; 
        int id , date ,time;
        stringstream ss(line_input);
        getline(ss, id_str,',');                
        id = stoi(id_str); 
        getline(ss, pass,',');
        getline(ss, date_time_str);
        date = stoi(date_time_str.substr(0,8));
        time = (stoi(date_time_str.substr(8,2))*60 + stoi(date_time_str.substr(10,2))); //換成min
        ss.clear(); 

        if(employees.find(id) == employees.end()){
            employees[id] = Employee(id,date,time);
        }else{
            employees[id].add_and_count_overloading(date,time);
        }
    }
    return employees;
}

void output(map<int,Employee> employees){
    for(auto emp : employees){
        cout<<emp.first<<","<<emp.second.get_overloading_days()<<","<<emp.second.get_forgoten_days()<<endl;
    }
}

void solve(char*file_name[]){
    map<int , Employee> employees = getAllData(file_name);
    output(employees);
}

int main(int argc , char*argv[]){
    solve(argv);
    return 0;
}