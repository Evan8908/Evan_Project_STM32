#ifndef __FixtureRun_H__
#define __FixtureRun_H__


#include "..\FIXTURE\IO_def.h"
#include "usart.h"



#define  state0  0
#define  state1  1
#define  state2  2
#define  state3  3
#define  state4  4
#define  state5  5
#define  state6  6
#define  state7  7
#define  state8  8
#define  state9  9
#define  state10  10


#define  state30  30



extern volatile uint8_t DUT1_STATE;
extern _flag flag;

void FixtureRun(void);


#endif
