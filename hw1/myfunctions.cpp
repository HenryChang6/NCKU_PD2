#include "myfunctions.h"
#include <cstdio>
#include <iostream>

void print_the_ans(int num){
  int max = 0;
  for(int i = 0; i <= num; i++){
    printf("(%d,%d)\n",i,num-i);
    if((i*(num-i))>max) max = (i * (num-i));
  }
  std::cout<<max<<std::endl;
}
