#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include<set>
#include<ctime>
using namespace std;

class Employee {
    private:
        int id;
        set<time_t> dates;
        int max_conti_day;
        time_t from_day;
        time_t end_day;
    public:
        Employee(int i , time_t T);
        Employee();
        ~Employee(); 
        void count_max_conti_days();
        void add_new_date(time_t T);
        int get_max_conti_days();
        int get_Id();
        string get_from_day();
        string get_end_day();

};

#endif
