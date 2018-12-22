/*
 * OrderedStatCfarImpl.h
 *
 *  Created on: May 2, 2016
 *      Author: iu24904
 */

#ifndef SRC_ORDEREDSTATCFARIMPL_H_
#define SRC_ORDEREDSTATCFARIMPL_H_

#include <deque>
#include "OrderedStatCfar.h"

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

  std::deque<float> m_selectLeft;
  std::deque<float> m_selectRight;
  void slideSelectWindow(const unsigned int idx, const float* input, const unsigned int size);
  float reduceSelectBuffer();


  long m_maskSelectLeftStart_offset; 
  long m_maskSelectLeftEnd_offset; 
  unsigned int m_maskSelectRightStart_offset; 
  unsigned int m_maskSelectRightEnd_offset; 
  long m_maskOmitStart_offset;  
  long m_maskOmitEnd_offset; 
 
 
  bool m_debug;
};

#endif /* SRC_ORDEREDSTATCFARIMPL_H_ */
