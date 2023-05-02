#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<stdint.h>
//Wrong
//#include<bits/stdc++.h>
using namespace std;

int in[200000] = {} , out[200000] ={};

bool determine8hrs(int hour_out,int minute_out,int hour_in,int minute_in){
     
    int counter_min = 0;

    while(hour_in!=hour_out || minute_in!=minute_out){
        minute_in++;
        counter_min++;
        if(minute_in == 60) minute_in = 0 , hour_in++; 
    }

    return (counter_min>480)?true:false;

}

int main(int argc , char* argv[]){
    ifstream inputfile(argv[1],ios::in);

    string input; 

    while(inputfile>>input){
        string tmp_time , tmp_id;
        int id;
        uint64_t time; 
        string sign_state;

        stringstream ss(input);
        getline(ss ,tmp_id,',');
        getline(ss,sign_state,',');
        getline(ss,tmp_time,',');
        id = stoll(tmp_id.c_str());
        time = stoll(tmp_time.c_str());
        if(sign_state == "sign-in")  
            in[id] = time%10000;
        else out[id] = time%10000;
    }
    inputfile.close();
    for(int i=1; i<=200000; i++){
        int overloading_days = 0 , sign_forgot_days = 0; 
        if(in[i]!=0){
            if(out[i] != 0){
                if(determine8hrs(out[i]/100,out[i]%100,in[i]/100,in[i]%100))
                    cout<<i<<","<<1<<","<<0<<endl;
                else cout<<i<<","<<"0,0\n";
            }
            else cout<<i<<","<<0<<","<<1<<endl; 
        }
        else{
            if(out[i]!=0)
                cout<<i<<","<<0<<","<<1<<endl;
        }
    }
}



/*
input format:
employee_id,signing_type,signing_time

output format:
employee_id,overloading_days,sign_forget_days
*/