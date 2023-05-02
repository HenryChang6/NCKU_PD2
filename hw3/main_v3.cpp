#include <iostream>
#include <string>
#include <sstream>
#include<fstream>
#include<vector>
#include<algorithm>
#define ll long long int
using namespace std;
 
class SignInfo{
    private:
        int status;  //in 0 out 1
        ll time;
        int date, hour, min, total_min, year;

    public:
        SignInfo(int s, long long int t): status(s), time(t){
            ll temp = time;
            int time_format[3] = {0,0,0};
            for(int i=0;i<3;i++){
                time_format[i] = temp % 10000;
                temp /= 10000; 
            }
            min = 60 * time_format[0] / 100 + time_format[0] %100;
            date = time_format[1];
            year = time_format[2];
        }
        bool is_overloading(SignInfo target){
            return min - target.min > 480 ? true : false;
        }
        bool is_legal_pair(SignInfo target){
            return status ^ target.status && date == target.date; 
        }
        // TODO : oveeride compare operator
        friend bool operator< (const SignInfo& s1, const SignInfo& s2){
            // cout << e1.employee_id << e2.employee_id << endl;
            return s1.time < s2.time;
        }
        friend ostream& operator<<(ostream& os, const SignInfo& info) {
            os << info.status << " " << info.time;
            return os;
        } 
};

class Employee{
    private:
        int employee_id;
        //下面這兩個用來check它每天有沒有簽到退 x軸為年份 y軸為日期 值為小時＋分鐘
        int overloading_days; //超時工作天數計數器
        int forgotsign_days;  //忘記簽到或簽退天數計數器
    public:
        vector<SignInfo> info;
        Employee(int id,SignInfo sign_info){
            employee_id = id;
            overloading_days = 0;
            forgotsign_days = 0;
            add_new_sign_info(sign_info);
        }
        void add_new_sign_info(SignInfo sign_info){
            info.push_back(sign_info);
        }
        void sort_info(){
            // sort(info.begin(), info.end(), 
            //    [&](const SignInfo* a, const SignInfo* b) -> bool {return a->time < b->time;});
            sort(info.begin(), info.end());
        }
        // TODO : oveeride compare operator
        friend bool operator<(const Employee& e1, const Employee& e2){
            // cout << e1.employee_id << e2.employee_id << endl;
            return e1.employee_id < e2.employee_id;
        }
        friend ostream& operator<<(ostream& os, const Employee& e) {
            os << e.employee_id << "," << e.overloading_days << "," << e.forgotsign_days;
            return os;
        } 
        
        void count_overloadingAndforgot_days(){
            sort_info();
            overloading_days = 0, forgotsign_days = 0;
            // forgotsign_days = info.size() % 2 ? 0 : 1;
            vector<SignInfo> stack;
            for(int i=0;i < info.size();i++){
                if(stack.empty()){
                    stack.push_back(info[i]);
                }
                else if(stack.back().is_legal_pair(info[i])){
                    if(info[i].is_overloading(stack.back())){
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
        int getIndex(){
            return employee_id;
        }
};

int findEmployeeWithIndex(vector<Employee> target,int index){
    for(int i=0;i<target.size();i++){
        if(target[i].getIndex() == index){
            return i;
        }
    }
    return -1;
}

vector<Employee> geAllEmplyeeData(char*argv[]){
    ifstream inputFile(argv[1],ios::in);
    vector<Employee> employees;
    string input;
    // Map<String, Employee> employees;
    while(inputFile>>input){
        string status_str,date_time_str ,id_str; 
        int status, id, index;
        ll time;
        //用stringstream分別讀入 並轉換成整數型別
        stringstream ss(input);
        getline(ss, id_str,',');                
        id = stoi(id_str); 
        getline(ss, status_str,',');
        status = status_str == "sign-in" ? 0 : 1;
        getline(ss, date_time_str);
        time = stoll(date_time_str);
        ss.clear(); 
        // 檢查有沒有在 employees 裡面
        index = findEmployeeWithIndex(employees, id);
        if(index == -1){
            // index does not exists at employees list
            // create new employee
            employees.push_back(Employee(id, SignInfo(status, time)));
        }else{
            // add info into existed employee
            employees[index].add_new_sign_info(SignInfo(status, time));
        }
    }
    inputFile.close();
    // sort(employees.begin(),employees.end(),SortById);
    sort(employees.begin(),employees.end());
    return employees;
}

void solve(char *input[]){
    vector<Employee> employees = geAllEmplyeeData(input);   //已依照id大小排序 
    for(auto emp:employees){
        emp.count_overloadingAndforgot_days(); //count之前先用time再次排序
        cout << emp << endl;
    }
}

int main(int argc , char*argv[]){
    solve(argv);
    return 0;
}


