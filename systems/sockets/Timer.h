//--------------------------------------------------------------------
// Timer.h
// Implementation file for Timer class
// Author: Chase Howard
// Class: CSS 503B [University of Washington Bothell]
// Date: 2021-06-06
//-------------------------------------------------------------------- 
//	Description: 
//    Used for calculating duration during program execution.
//    Call start() to start the timer
//    Call finish() to stop the timer and return the duration

#include <chrono>
using namespace std::chrono;

class Timer {
private:
    steady_clock::time_point startTime;   // start time
    steady_clock::time_point endTime;     // end time

public:
   //------------------------------------------------------------------------
   // Timer: Default Constructor
   //------------------------------------------------------------------------
   // Pre: none
   // Post: Object created and start time has been set 
    Timer();

   //------------------------------------------------------------------------
   // Timer: Destructor
   //------------------------------------------------------------------------
   // Pre: object created
   // Post: object destroyed
    ~Timer();

   //------------------------------------------------------------------------
   // Start: start the timer
   //------------------------------------------------------------------------
   // Pre: none
   // Post: Sets the 'start time' at the time of calling
    void start();

   //------------------------------------------------------------------------
   // Stop: stop the timer
   //------------------------------------------------------------------------
   // Pre: none
   // Post: sets the stop time at the time of calling, returns the difference 
   //    between start and stop time in seconds
    double finish();
};