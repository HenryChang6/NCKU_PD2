#include "employee.h"
#include<set>
#include<ctime>
#include<string>
#include<sstream>
#include<iomanip>

using namespace std;

//constructor
Employee::Employee(int i , time_t T){
    id = i;
    dates.insert(T);
}

Employee::Employee(){return;}

Employee::~Employee() { // 添加析构函数实现
    // 在这里释放资源（当前代码中不需要）
}

void Employee::add_new_date(time_t T){
   dates.insert(T);
}

void Employee::count_max_conti_days(){
    max_conti_day = 1;
    //用來暫存目前連續幾天
    int temp = 1;
    //default state for employees whose max = 1
    from_day = *dates.begin();
    end_day = *dates.begin();
    //iterate through the set to count the max_conti_day and save start day & end day
    for(auto it = dates.begin(); next(it) != dates.end(); ++it){
        double diff = difftime(*next(it),*it);
        if(diff == 86400.0){
            temp++;
            if(temp >= max_conti_day){
                max_conti_day = temp;
                from_day = *prev(it , (temp-2));
                end_day = *next(it);
            }
        }else{
            temp = 1;
        } 
    }
}

int Employee::get_Id(){return id;}

int Employee::get_max_conti_days(){return max_conti_day;}

string Employee::get_from_day(){
    //localtime()將time_t轉成tm
    //再經由put_time()將tm轉成string
    std::ostringstream oss;
    oss << put_time(localtime(&from_day), "%Y%m%d"); // 使用 std::put_time 简化日期格式化
    return oss.str();
}

string Employee::get_end_day(){
    //localtime()將time_t轉成tm
    //再經由put_time()將tm轉成string
    std::ostringstream oss;
    oss << put_time(localtime(&end_day), "%Y%m%d"); // 使用 std::put_time 简化日期格式化
    return oss.str();
}
