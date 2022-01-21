//--------------------------------------------------------------------
// Timer.cpp
// Implementation file for Timer class
// Author: Chase Howard
// Class: CSS 503B [University of Washington Bothell]
// Date: 2021-06-06
//-------------------------------------------------------------------- 
//	Description: 
//    See header file Timer.h

#include "Timer.h"

using namespace std::chrono;

//------------------------------------------------------------------------
// Timer: Default Constructor
//------------------------------------------------------------------------
// Pre: none
// Post: Object created and start time has been set 
Timer::Timer() 
{
   start();
}

//------------------------------------------------------------------------
// Timer: Destructor
//------------------------------------------------------------------------
// Pre: object created
// Post: object destroyed
Timer::~Timer() 
{
   finish();
}

//------------------------------------------------------------------------
// Start: start the timer
//------------------------------------------------------------------------
// Pre: none
// Post: Sets the 'start time' at the time of calling
void Timer::start() 
{
   startTime = steady_clock::now();
}

//------------------------------------------------------------------------
// Stop: stop the timer
//------------------------------------------------------------------------
// Pre: none
// Post: sets the stop time at the time of calling, returns the difference 
//    between start and stop time in seconds
double Timer::finish()
{
   endTime = steady_clock::now();
   duration<double> seconds = (endTime - startTime);
   return seconds.count();
}
