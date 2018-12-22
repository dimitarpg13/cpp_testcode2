/*
 * OrderedStatCfar.h
 *
 *  Created on: May 5, 2016
 *      Author: iu24904
 */

#ifndef SRC_ORDEREDSTATCFAR_H_
#define SRC_ORDEREDSTATCFAR_H_

class OrderedStatCfar
{
public:
  /*
   * The mask array defined by the two constructor parameters as:
   * |1..............1|0..............0|0|0..............0|1..............1|
   * |<- maskSelect ->|<- maskOmit-1 ->| |<- maskOmit-1 ->|<- maskSelect ->|
   */
  OrderedStatCfar(
      unsigned int maskSelect,
      unsigned int maskOmit,
      float percentile);

  virtual ~OrderedStatCfar();

  /*
   * Input and output are array of same length "size"
   */
  virtual void run(
      unsigned int size,
      const float* input,
      float* output) = 0;
};

#endif /* SRC_ORDEREDSTATCFAR_H_ */
