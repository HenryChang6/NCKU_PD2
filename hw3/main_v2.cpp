#include <iostream>
#include <string>
#include <sstream>
#include<fstream>
#include<vector>
#include<algorithm>
#define ll long long int
using namespace std;
class SignInfo{
    public:
        int status;  //in 0 out 1
        ll time;
        SignInfo(int s, long long int t): status(s), time(t){}
        int get_Date(){
            return (time - (time%10000))/10000;
        }
        int get_hour(){
            return (time - (time/10000)*10000)/100; 
        }
        int get_min(){
            return (time-(time/10000)*10000)%100;
        }
        int totalMin(){
            return 60 * get_hour() + get_min();    
        }
        bool is_overloading(SignInfo* target){
            return totalMin() - target->totalMin() > 480 ? true : false;
        }
        bool is_legal_pair(SignInfo* target){
            return status ^ target->status && get_Date() == target->get_Date(); 
        }
        // TODO : oveeride compare operator
        // bool operator>(const SignInfo* info)const{
        //     return time > info->time;
        // }
        friend ostream& operator<<(ostream& os, const SignInfo* info) {
            os << info->status << " " << info->time;
            return os;
        } 
};

bool SortByTime(SignInfo* a , SignInfo* b){
    return (a->time < b->time);
}

class Employee{
    public:
        int employee_id;
        //下面這兩個用來check它每天有沒有簽到退 x軸為年份 y軸為日期 值為小時＋分鐘
        int overloading_days; //超時工作天數計數器
        int forgotsign_days;  //忘記簽到或簽退天數計數器
        vector<SignInfo*> info;
        Employee(int id,SignInfo* sign_info){
            employee_id = id;
            add_new_sign_info(sign_info);
        }
        void add_new_sign_info(SignInfo* sign_info){
            info.push_back(sign_info);
        }   
        void sort_info(){
            sort(info.begin() , info.end(),SortByTime);
        }
        // TODO : oveeride compare operator
        // bool operator>(const Employee& other){
        //     return employee_id > other.employee_id;
        // }      
        friend ostream& operator<<(ostream& os, const Employee* emp) {
            os << emp->employee_id << "," << emp->overloading_days << "," << emp->forgotsign_days;
            return os;
        }
        
        void count_overloadingAndforgot_days(){
            sort_info();
            overloading_days = 0, forgotsign_days = 0;
            // forgotsign_days = info.size() % 2 ? 0 : 1;
            vector<SignInfo*> stack;
            for(int i=0;i < info.size();i++){
                if(stack.empty()){
                    stack.push_back(info[i]);
                }
                else if(stack.back()->is_legal_pair(info[i])){
                    if(info[i]->is_overloading(stack.back())){
                        overloading_days += 1;
                    }
                    stack.pop_back();
                }
                else{
                    stack.push_back(info[i]);
                }
            }
            forgotsign_days = stack.size();
        }
};

int findEmployeeWithIndex(vector<Employee*> target,int index){
    for(int i=0;i<target.size();i++){
        if(target[i]->employee_id == index){
            return i;
        }
    }
    return -1;
}

bool SortById(Employee* a, Employee* b){
    return (a->employee_id < b->employee_id);
}


vector<Employee*> geAllEmplyeeData(char*argv[]){
    ifstream inputFile(argv[1],ios::in);
    vector<Employee*> employees;
    //innitialize for input
    string input;
    // Map<String, Employee> employees;
    //處理讀入資料
    while(inputFile>>input){
        string status_str ,date_time_str ,id_str; 
        int status , id;
        ll time;
        //用stringstream分別讀入 並轉換成整數型別
        stringstream ss(input);
        getline(ss,id_str,',');                
        id = stoi(id_str); 
        getline(ss,status_str,',');
        status = status_str == "sign-in" ? 0 : 1;
        getline(ss,date_time_str);
        time = stoll(date_time_str);
        ss.clear(); 
        // 檢查有沒有在 employees 裡面
        int index = findEmployeeWithIndex(employees, id);
        SignInfo* info = new SignInfo(status, time); 
        if(index == -1){
            // index does not exists at employees list
            // create new employee
            employees.push_back(new Employee(id, info));
        }else{
            // add info into existed employee
            employees[index]->add_new_sign_info(info);
        }
    }
    inputFile.close();
    sort(employees.begin(),employees.end(),SortById);
    return employees;
}

int main(int argc , char*argv[]){
    vector<Employee*> employees = geAllEmplyeeData(argv);   //已依照id大小排序 
    for(auto emp:employees){
        emp -> count_overloadingAndforgot_days();
        cout << emp << endl;
    }
    return 0;
}
