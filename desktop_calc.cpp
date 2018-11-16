#include <iostream>
#include <stack>
#include <cmath>

int unwind_stack(std::stack<int>& st) {
   int res = 0;
   int n = 0;
   while (!st.empty()) {
     res += (int) std::pow(10,n++) * st.top();
     st.pop();

   }

   return res;
}

int main(int argc, char * argv[]) {
  const char * input = argv[1];
  int sz = strlen(input);
  std::stack<int> num;
  std::stack<int> op;
  std::stack<int> vals;
  int leftRes=0;
  int rightRes=0;
  int finalRes=0;
  for (int i = 0; i < sz; i++) {
     //std::cout << input[i] << std::endl;
     if (input[i] >= '0' && input[i] <= '9') {
       char c[2]; c[0] = input[i]; c[1] = '\0';
       num.push(atoi(c));
       std::cout << input[i] << std::endl;
     } else {
        //std::cout << "Res=" << unwind_stack(num) << std::endl;
        if (!op.empty()) {
            vals.push(unwind_stack(num));
            if (op.top() == 2) {
               rightRes = vals.top();
               vals.pop();
               leftRes = vals.top();
               vals.pop();
               finalRes = leftRes * rightRes;
               vals.push(finalRes);
               op.pop();
               
            } 
        }
        else
           vals.push(unwind_stack(num));
         
        if (input[i] == '*') {
           op.push(2);
        } else if (input[i] == '+') {
           op.push(1);
        }
     }


        if (i == sz - 1) {
           vals.push(unwind_stack(num));
               while (!op.empty()) {
                  leftRes = vals.top();
                  vals.pop();
                  rightRes = vals.top();
                  vals.pop();
                  if (op.top()==1) {
                     vals.push(leftRes + rightRes); 
                  } else if (op.top()==2) {
                     vals.push(leftRes * rightRes);
                  }
                  op.pop();
               }
               finalRes=0;
               while (!vals.empty()) { 
                  finalRes+=vals.top();
                  vals.pop();
               }
        }
  } 

  std::cout << "Final result: " << finalRes << std::endl; 
}
