/*
 * OrderedStatCfarImpl.cpp
 *
 *  Created on: May 2, 2016
 *      Author: iu24904
 */

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include "OrderedStatCfarImpl.h"

using std::min;
using std::max;
using std::nth_element;
using std::cout;
using std::endl;

OrderedStatCfarImpl::OrderedStatCfarImpl(
    unsigned int maskSelect,
    unsigned int maskOmit,
    float percentile,
    bool debug) :
        m_maskSelect(maskSelect),
        m_maskOmit(maskOmit),
        m_percentile(percentile),
        m_debug(debug)
{
     m_maskSelectLeftStart_offset = -((long)maskSelect)-(2*((long)maskOmit)-1)/2;
     m_maskSelectLeftEnd_offset   = -1-(2*((long)maskOmit)-1)/2;
     m_maskSelectRightStart_offset= (2*((long)maskOmit)-1)/2+1;
     m_maskSelectRightEnd_offset  = (2*((long)maskOmit)-1)/2+((long)maskSelect);
}

OrderedStatCfarImpl::~OrderedStatCfarImpl()
{
}

std::deque<std::pair<float,unsigned int>>::const_iterator OrderedStatCfarImpl::findSlot(const unsigned int idx) {
	return std::find_if(m_selectBuffer.begin(), m_selectBuffer.end(), 
		       [&idx](const std::pair<float,unsigned int>& arg) { return arg.second == idx; });

}

void OrderedStatCfarImpl::slideSelectWindow(const unsigned int idx, const float* input, 
                                                                                   const unsigned int size)
{
    if (idx > 0) {
       // slide both sides of the window one point to the right
       long ms_ls_idx = idx + m_maskSelectLeftStart_offset; // maskSelect left start running index
       long ms_le_idx = idx + m_maskSelectLeftEnd_offset; // maskSelect left end running index
       auto ms_rs_idx = idx + m_maskSelectRightStart_offset; // maskSelect right start running index
       auto ms_re_idx = idx + m_maskSelectRightEnd_offset; // maskSelect right end running index
       if (ms_ls_idx > 0 && m_leftLast >= m_first && m_leftLast <= m_last) {
          
	  auto res = findSlot(m_first);
	  if (res != m_selectBuffer.end())
	  {
             m_selectBuffer.erase(res);
	     m_first++;
	  }
       }
       
       if (m_rightFirst <= m_last) {
	  auto res = findSlot(m_rightFirst);
	  if (res != m_selectBuffer.end()) {
              m_selectBuffer.erase(res);
	      m_rightFirst++;
	  }
	  else
	  {
	      if (m_debug)
                 std::cout << "Internal Error!" << std::endl;
	  }
       }
       if (ms_le_idx < 0 && ms_rs_idx >= size) {
          // nothing to do at this point which is in the eye of the sliding window 
          return;
       }
       if (ms_le_idx >=0) {
	  if (m_leftLast < m_last) {
	     if (m_rightFirst <= m_last) {
	         auto res = findSlot(m_rightFirst);
                 if (res != m_selectBuffer.end()) {
                    m_selectBuffer.insert(res, std::make_pair(input[ms_le_idx], ms_le_idx));
	            m_leftLast = ms_le_idx;
		    if (m_first > m_leftLast)
		       m_first = m_leftLast;
	         } else {
                    if (m_debug)
		    std::cout << "Internal Error!" << std::endl;
	         }
	     }
	     else {
	         m_selectBuffer.push_front(std::make_pair(input[ms_le_idx], ms_le_idx));
		 m_leftLast = ms_le_idx;
	     }
	  }
	  else if (m_leftLast == m_last) {
             m_selectBuffer.push_front(std::make_pair(input[ms_le_idx], ms_le_idx));
	     m_leftLast = ms_le_idx;
	  } 
       } 
       if (ms_re_idx < size) {
          m_selectBuffer.push_front(std::make_pair(input[ms_re_idx], ms_re_idx));
	  m_last = ms_re_idx;
       }
    } 
    else {
       // perform initial fill of the right window 
       auto end = std::min(m_maskSelectRightEnd_offset, size-1);
       for (auto i =  m_maskSelectRightStart_offset; i <= end; ++i) {
           m_selectBuffer.push_front(std::make_pair(input[i],i));
       }
       m_first = m_maskSelectRightStart_offset;
       m_rightFirst = m_maskSelectRightStart_offset;
       m_leftLast = m_first-1;
       m_last = end;
    }

    if (m_debug) {
       std::cout << std::endl << "Left Window:" << std::endl;
       if (m_first <= m_leftLast && m_leftLast <= m_last) {
	   for (auto i = m_first; i <= m_leftLast; i++) {
	       auto res = findSlot(i);
	       if (res != m_selectBuffer.end()) {
	          std::cout << res->first << ", ";
	       }
	       else {
                   std::cout << "Internal Error!" << std::endl;
	       }
	   }
       }
       std::cout << std::endl;
       std::cout << "Right Window:" << std::endl;
       if ( m_rightFirst <= m_last ) {
	   for (auto i = m_rightFirst; i <= m_last; i++) {
	       auto res = findSlot(i);
	       if (res != m_selectBuffer.end()) {
	          std::cout << res->first << ", ";
	       }
	       else {
                   std::cout << "Internal Error!" << std::endl;
	       }
	   }
       }
       std::cout << std::endl;
    }
}


float OrderedStatCfarImpl::reduceSelectBuffer() {
   auto cmp = [](const std::pair<float, unsigned int> &a, const std::pair<float, unsigned int> &b) {
      return a.first < b.first;
   }; 
   auto nth_el = static_cast<unsigned int>(std::floor( m_percentile * m_selectBuffer.size())) ;
   if (nth_el > 0) nth_el--;
   if (m_debug)
      std::cout << std::endl << "percentile :" << m_percentile << std::endl;
   std::nth_element(m_selectBuffer.begin(), m_selectBuffer.begin() + nth_el, m_selectBuffer.end(), cmp);
   if (m_debug)
      std::cout << "the nth elem: " << m_selectBuffer[nth_el].first << std::endl;
   return m_selectBuffer[nth_el].first;
}

void OrderedStatCfarImpl::run(
    unsigned int size,
    const float* input,
    float* output)
{
    if (m_maskSelectRightStart_offset >= size)
        return; // nothing to do - the omit mask is too wide

    m_selectBuffer.clear();

    unsigned int idx=0;
    do {
        slideSelectWindow(idx, input, size);
        if (!m_selectBuffer.empty())
            output[idx] = reduceSelectBuffer();
    } while (++idx < size);
}

bool compare(const float * output, const float * truth, const unsigned int size) {
    for (unsigned int i = 0; i < size; ++i) {
      if (output[i] != truth[i])
         return false;
    }
    return true;
}

void run_test(const unsigned int testNumb, const int k, const int l, 
                 const float percentile, const unsigned int size, 
                    const float * input, const float * truth, const bool debug) {

   OrderedStatCfarImpl os(k, l, percentile, debug);
   std::ostringstream sstr;
   sstr  << "-------------- Test Case " << testNumb << " --------------";
   std::string testCaseName = sstr.str();
   std::cout << std::endl << testCaseName << " RUNNING.." << std::endl;
   float output[size] = { 0 };
   os.run(size,input,output);
   if (compare(output, truth, size)) {
      std::cout << testCaseName << " PASSED." << std::endl << std::endl;
   }
   else
   {
      std::cout << testCaseName << " FAILED!" << std::endl << std::endl;
   }
}

int main(int argc, char *argv[]) {
   
   const unsigned int size1 = 5;
   float input1[size1] = { 1.2f, 2.3f, 3.1f, 4.7f, 5.9f };
   float truth1[size1] = { 2.3f, 3.1f, 4.7f, 3.1f, 3.1f };
   run_test(1, 2, 1, 0.75, size1, input1, truth1, false);


   const unsigned size2 = 10;
   float input2[size2] = { 1.9f, 2.8f, 3.7f, 4.6f, 5.6f, 6.4f, 7.3f, 8.2f, 9.1f, 10.0f };
   float truth2[size2] = { 4.6f, 5.6f, 6.4f, 6.4f, 7.3f, 8.2f, 5.6f, 6.4f, 6.4f,7.3f };
   run_test(2, 3, 2, 0.75, size2, input2, truth2, false);

   const unsigned int size3 = 20;
   float input3[size3] = { 1.9f, 1.9f, 1.9f, 1.9f, 2.1f, 10.1f, 13.1f, 1.11f, 3.14f, 4.1f,
                                     2.31f, 5.1f, 7.1f, 1.123f, 9.3f, 23.1f, 12.1f, 56.3f, 11.1f, 7.6f };

   float truth3[size3] = { 1.9f, 2.1f, 10.1f, 1.9f, 1.9f, 1.9f, 2.31f, 4.1f, 7.1f, 7.1f, 
                                        7.1f, 4.1f, 9.3f, 12.1f, 11.1f, 7.6f, 7.6f, 9.3f, 12.1f, 23.1f };
   run_test(3, 3, 2, 0.75, size3, input3, truth3, false);

   OrderedStatCfarImpl os4(1, 5, 0.75, true);
   const unsigned int size4 = 20;
   float input4[size4] = { 1.9f, 1.9f, 1.9f, 1.9f, 2.1f, 10.1f, 13.1f, 1.11f, 3.14f, 4.1f,
                                     2.31f, 5.1f, 7.1f, 1.123f, 9.3f, 23.1f, 12.1f, 56.3f, 11.1f, 7.6f };

   float truth4[size4] = { 10.1f, 13.1f, 1.11f, 3.14f, 4.1f, 1.9f, 1.9f, 1.9f, 1.123f, 2.1f, 
                                        10.1f, 12.1f, 1.11f, 3.14f, 4.1f, 2.31f, 5.1f, 7.1f, 1.123f, 9.3f };
   run_test(4, 1, 5, 0.75, size4, input4, truth4, false);


   const unsigned int size5 = 5;
   float input5[size5] = { 1.2f, 2.3f, 3.1f, 4.7f, 5.9f };
   float truth5[size5] = { 0 };
   run_test(5, 1, 5, 0.75, size5, input5, truth5, false);


   const unsigned int size6 = 5;
   float input6[size6] = { 1.2f, 2.3f, 3.1f, 4.7f, 5.9f };
   float truth6[size6] = { 5.9f, 0, 0, 0, 1.2f };
   run_test(6, 1, 4, 0.75, size6, input6, truth6, false);


   const unsigned int size7 = 5;
   float input7[size7] = { 1.2f, 2.3f, 3.1f, 4.7f, 5.9f };
   float truth7[size7] = { 5.9f, 0, 0, 0, 1.2f };
   run_test(7, 1, 4, 0.15, size7, input7, truth7, false);


   const unsigned int size8 = 5;
   float input8[size8] = { 1.2f, 2.3f, 3.1f, 4.7f, 5.9f };
   float truth8[size8] = { 2.3f, 1.2f, 1.2f, 2.3f, 3.1f };
   run_test(8, 2, 1, 0.15, size8, input8, truth8, false);
   
   return 0;
}
