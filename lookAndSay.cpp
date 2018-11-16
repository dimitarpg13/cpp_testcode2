#include <stdlib.h>
#include <iostream>
#include <stack>

int processNidentical(std::stack<int>& input, std::stack<int>& output) {
    int count = 1;
    while (input.size() > 0) {
       int top = input.top();
       input.pop();
       while (input.size() > 0 && top == input.top()) {
          count++;
          input.pop();
       }
       output.push(count);
       output.push(top);
    }
    return 0;
}

int printStack(std::stack<int>& s) {
  int ret = s.size();
  while (s.size() > 0) {
     std::cout << s.top() << std::endl;
     s.pop();
  }    
  return ret;
}

int main(int argc, char* argv[]) {
   std::stack<int> s;

   for (int i = 1; i < argc; i++)
     s.push(atoi(argv[i]));

   std::stack<int> o;

   processNidentical(s,o);

   printStack(o);

   return 0;
}
