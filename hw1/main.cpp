#include <string>
#include "myfunctions.h"

int main(int argc, char **argv) {
  int num = std::stoi(argv[1]) , counter = 0;
  print_the_ans(num);
  return 0;
}
