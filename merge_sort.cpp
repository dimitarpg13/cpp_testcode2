#include <iostream>
#include <vector>

void merge_two_pieces_internal(std::vector<int>& a, int start, int middle, int end, std::vector<int>& b) {
   
   int i=start;
   int j=middle;
   for (int k = 0; k < end - start ; k++) {
      if (i < middle && j < end) {
         if (a[i] > a[j]) {
            b[start+k]=a[i++];
         } else {
            b[start+k]=a[j++];
         }
      }
      else 
      {
         if (i < middle)
            b[start+k]=a[i++];
         else
            b[start+k]=a[j++];

      }
   } 
  
   for (int l=start; l < end; l++) {
       a[l] = b[l];
   } 
}

// both a and b are the same size
void merge_sort_internal(std::vector<int>& a, int start, int end, std::vector<int>& b) {

   if (end - start < 2)
       return;   

   int middle = ( start + end ) / 2;
     
   merge_sort_internal(a, start, middle, b);
   merge_sort_internal(a, middle, end, b);

   merge_two_pieces_internal(a, start, middle, end, b);   
}


int main (int argc, char *argv[]) {
   
   std::vector<int> a {1, 9, 2, 7, 5, 6, 4};
   int n = a.size();
   std::vector<int> b (n, 0);
   merge_sort_internal(a, 0, n, b); 

   for (int i = 0; i < b.size(); i++) {
      std::cout << b[i] << std::endl;

   }

   return 0;
}
