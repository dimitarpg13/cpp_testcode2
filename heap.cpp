#include <iostream>
#include <vector>

int Left(int i) {
    return 2*i + 1; 
}

int Right(int i) {
    return 2*i + 2;
}

int Parent(int i) {
   return std::max(0, ( i - 1 ) / 2 ); 
}



void Heapify(std::vector<int>& input, int topIdx) {
   int leftIdx = Left(topIdx);
   int rightIdx = Right(topIdx);

   if (leftIdx < input.size()) {
      if (input[leftIdx] > input[topIdx]) {
         int tmp = input[topIdx];
         input[topIdx] = input[leftIdx];
         input[leftIdx] = tmp;
      }
   } else return; 


   if (rightIdx < input.size()) {
      if (input[rightIdx] > input[topIdx]) {
         int tmp = input[topIdx];
         input[topIdx] = input[rightIdx];
         input[rightIdx] = tmp;
      }
   } else return;

   Heapify(input, leftIdx);
   Heapify(input, rightIdx);
  
}

int main(int argc, char * argv[]) {
   std::vector<int> input { 2, 7, 3, 5, 1, 6, 9 };
   int nonLeafCount=input.size()/2;
   for (int i=nonLeafCount; i>=0; i--)     
       Heapify(input, i);
   for (int i = 0; i < input.size(); i++) {
      std::cout << input[i] << std::endl;
   }

}
