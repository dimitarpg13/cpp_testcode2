/*
 * OrderedStatCfarImpl.h
 *
 *  Created on: May 2, 2016
 *      Author: iu24904
 */

#ifndef SRC_ORDEREDSTATCFARIMPL_H_
#define SRC_ORDEREDSTATCFARIMPL_H_

#include <deque>
#include <unordered_map>
#include "OrderedStatCfar.h"

struct Entry {
  float m_val;
  unsigned int m_idx;
};

class OrderedStatCfarImpl
{
public:
  OrderedStatCfarImpl(
      unsigned int maskSelect,
      unsigned int maskOmit,
      float percentile,
      bool debug);

  ~OrderedStatCfarImpl();

  void run(
      unsigned int size,
      const float* input,
      float* output);

private:
  void checkSelectBuffer(long idx, unsigned int selectSz);

  unsigned int m_maskSelect;
  unsigned int m_maskOmit;
  float m_percentile;

  std::deque<Entry> m_selectBuffer;
  void slideSelectWindow(const unsigned int idx, const float* input, const unsigned int size);
  float reduceSelectBuffer();


  long m_maskSelectLeftStart_offset; 
  long m_maskSelectLeftEnd_offset; 
  unsigned int m_maskSelectRightStart_offset; 
  unsigned int m_maskSelectRightEnd_offset; 

  unsigned int m_first;
  unsigned int m_leftLast;
  unsigned int m_rightFirst;
  unsigned int m_last;
  std::deque<Entry>::const_iterator findSlot(const unsigned int idx); 
  bool m_debug;
};

#endif /* SRC_ORDEREDSTATCFARIMPL_H_ */
