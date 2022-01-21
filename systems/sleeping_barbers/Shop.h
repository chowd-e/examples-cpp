//--------------------------------------------------------------------
// Shop.h
// Header file for Shop class
// Author: Chase Howard
// Class: CSS 503B [University of Washington Bothell]
// Date: 2021-05-09
//-------------------------------------------------------------------- 
//	Description: 
//    This program functions as a solution for the case of the 'Multiple
//    sleeping barbers' and functions as a monitor for scheduling access 
//    to common resources between barbers and customers through the use 
//    of mutex and condition variables

//    See attached documentation for more in depth description

#ifndef SHOP_H_
#define SHOP_H_
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <cassert>
using namespace std;

#define kDefaultNumChairs 3
#define kDefaultNumBarbers 1

class Shop 
{
private:

   struct Customer;
   struct Barber;
   
   // States of both Customer and Barbers relating to the transaction process
   enum State {WAITING, STAGING, INSERVICE, PAYMENT, COMPLETE, INVALID};

   // structure for containing information of a single customer entering the 
   //    shop
   struct Customer
   {
      int id; // unique id of customer
      int myBarber; // id of the barber cutting customers hair
      enum State state; // state of the customer
      
      Customer(int i) : id(i), myBarber(-1), state(INVALID) {};
   };

   // structure for containing information of a single barber working in the 
   //    shop
   struct Barber
   {
      int id; // unique id
      int currCustomer; // current customer id the barber is working on
      enum State state; // state of the barber

      Barber(int i) : id(i), currCustomer(-1), state(INVALID) {};
   };

   const int max_barbers_; // #barbers in shop
   const int max_waiting_cust_; //#customers to wait in shop
   int cust_drops_; // integer indicating number of customers not served

   queue<Customer*> waiting_chairs_; // queue for holding waiting customers
   deque<Barber*> sleeping_barbers_; // deque for holding sleeping barbers
   vector<Customer*> service_chairs_; // mapped to barber ids for servicing
   vector<Barber*> barbers_; // all barbers by id - responsible for memory

   // Mutexes and condition variables to coordinate threads
   // mutex_ is used in conjuction with all conditional variables
   pthread_mutex_t mutex_;

   // signals customer queue to check for release of customer in front
   pthread_cond_t  cond_next_customer_;  
   // signals barber queue to wake the barber in front of queue
   pthread_cond_t  cond_wake_barber_;

   vector<pthread_cond_t>  cond_customer_served_; //signal cust, cut complete 
   vector<pthread_cond_t> cond_barber_paid_; //signal barb, payment complete
   //------------------------------------------------------------------------
   //                               ACCESSORS
   //------------------------------------------------------------------------
   
   //------------------------------------------------------------------------
   // get_nextCustomerID: return ID of next customer in the wait queue
   //------------------------------------------------------------------------
   // Pre: none
   // Post: returned ID of next customer waiting in line or returned -1 if no
   //    customers are waiting
   int get_nextCustomerID() const;

   //------------------------------------------------------------------------
   // get_nextBarberID: return ID of next barber in the wait queue
   //------------------------------------------------------------------------
   // Pre: none
   // Post: returned ID of next barber waiting in line or returned -1 if no
   //    barbers are waiting
   int get_nextBarberID() const;

   //------------------------------------------------------------------------
   // isEmptyServiceChair: checks if any service chairs are available
   //------------------------------------------------------------------------
   // Pre: none
   // Post: boolean return indicating if there are any empty chairs available
   bool isEmptyServiceChair() const;

   //------------------------------------------------------------------------
   // isEmptyServiceChair: checks if any service chairs are available
   //------------------------------------------------------------------------
   // Pre: none
   // Post: boolean return indicating if there are any empty chairs available
   bool isEmptyServiceChair(int id_barber) const;

   //------------------------------------------------------------------------
   // isCustomerWaiting: checks if any customers are waiting in teh queue
   //------------------------------------------------------------------------
   // Pre: none
   // Post: boolean return indicating if there are any customers currently 
   //    wiating
   bool isCustomerWaiting() const;

   //------------------------------------------------------------------------
   // init: initializes Shop
   //------------------------------------------------------------------------
   // Pre: none
   // Post: shop has been initialized with all condition variables and mutexes
   void init(int num_barbers);

   //------------------------------------------------------------------------
   // int2string: converts integer argument to a string
   //------------------------------------------------------------------------
   // Pre: none
   // Post: integer argument has been converted to a string and returned
   string int2string(int i);

   //------------------------------------------------------------------------
   // print(Customer): prints customer information relating to state
   //------------------------------------------------------------------------
   // Pre: none
   // Post: prints customer ID as well as information relating to its state to
   //    cout
   void print(Customer *customer);

   //------------------------------------------------------------------------
   // print(Barber): prints barber information relating to state
   //------------------------------------------------------------------------
   // Pre: none
   // Post: prints barber ID as well as information relating to its state to
   //    cout
   void print(Barber *barber);
   
public:
   //------------------------------------------------------------------------
   // Shop: Constructor accepting two arguments
   //------------------------------------------------------------------------
   // Pre: none
   // Post: Shop has been created with all conditional variables and mutex
   //       initilized according to number of barbers and chairs inidicated
   //       in the arguments. Vectors have been sized as well. 
   Shop(int num_barbers = kDefaultNumBarbers, 
        int num_chairs = kDefaultNumChairs) : max_barbers_(num_barbers),
        max_waiting_cust_((num_chairs >= 0 ) ? num_chairs : kDefaultNumChairs),
        cust_drops_(0)
   { 
      init(num_barbers); 
   };

   //------------------------------------------------------------------------
   // Shop: Default Constructor
   //------------------------------------------------------------------------
   // Pre: none
   // Post: Shop has been created with all conditional variables and mutex
   //       initilized according to number of barbers and chairs inidicated
   //       by Default Globals. Vectors have been sized as well. 
   // Shop() : max_barbers_(kDefaultNumBarbers), 
   //    max_waiting_cust_(kDefaultNumChairs), cust_drops_(0)
   // { 
   //    init(kDefaultNumBarbers); 
   // };

   //------------------------------------------------------------------------
   // ~Shop: Destructor
   //------------------------------------------------------------------------
   // Pre: none
   // Post: Dynamic memory allocated by shop has been properly handled. 
   ~Shop();

   //------------------------------------------------------------------------
   //                                  MUTATORS
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // createBarber: Creates and dynamically allocates memory for a barber
   //------------------------------------------------------------------------
   // Pre: none
   // Post: New barber has been created with an id equal to integer passed as
   //    argument. pointer to memory on heap has been returned. 
   Barber* createBarber(int id);

   //------------------------------------------------------------------------
   // createCustomer: Creates and dynamically allocates memory for a customer
   //------------------------------------------------------------------------
   // Pre: none
   // Post: New customer has been created with an id equal to integer passed as
   //    argument. pointer to memory on heap has been returned. 
   Customer* createCustomer(int id);

   //------------------------------------------------------------------------
   // visitShop: Customer visits shop for haircut
   //------------------------------------------------------------------------
   // Pre: none
   // Post: New customer has been created and entered the shop. Customer will
   //    first check for an open barber. If a barber is available, they will 
   //    take a seat at the service chair belonging to said barber. If no 
   //    barber is available, the customer will check to see if there is a 
   //    spot for them to wait in line. If there is a spot they will wait 
   //    in line for their turn. If there is not a spot they will leave.
   //    Once it becomes the customers turn for a haircut, they will occupy
   //    the barbers chair who will cut their hair, they then signal they are
   //    ready for a cut to wake up the barber and begin their haircut. 
   //    This method returns the id of the barber who is servicing them, -1 if
   //    there were no available barbers.
   int visitShop(int id_customer);

   //------------------------------------------------------------------------
   // leaveShop: Customers procedure for leaving the Shop
   //------------------------------------------------------------------------
   // Pre: visitShop has been called sucessfully
   // Post: Customer is sitting at a barber chair getting their haircut. They 
   //    will recieve a signal from the barber that the cut is complete, they
   //    will then pay and signal to the barber that their payment is complete
   //    they will then say goodbye and the customer will be deleted - dynamic
   //    memory is handled within here for the customer. 
   void leaveShop(int id_customer);

   //------------------------------------------------------------------------
   // helloCustomer: Barbers procedure for welcoming a customer to their chair
   //------------------------------------------------------------------------
   // Pre: none
   // Post: barber checks to see if there is a customer at their chair. If yes
   //    they will proceed to cut the customers hair waiting for them at the 
   //    service chair. If no, they will check the customer queue. If tehre is
   //    a customer in the queue, they will signal to the customer that there
   //    is a barber waiting to cut their hair. if there is no customer in the
   //    queue, the barber will sleep waiting for customer to signal them that
   //    they are waiting to be serviced in the chair. 
   void helloCustomer(int id_barber);

   //------------------------------------------------------------------------
   // createCustomer: Creates and dynamically allocates memory for a customer
   //------------------------------------------------------------------------
   // Pre: helloCustomer has been called
   // Post: Barber notifies customer in chair that haircut is complete and 
   //    waits for payement from customer. The customer leaves, the chair is 
   //    cleared for a new customer, and the barber enters the queue for a 
   //    new customer
   void byeCustomer(int id_barber);

   //------------------------------------------------------------------------
   //                                  ACCESSORS
   //------------------------------------------------------------------------
   
   //------------------------------------------------------------------------
   // get_cust_drops: return total # of customers unable to be serviced
   //------------------------------------------------------------------------
   // Pre: none
   // Post: returned value of member variable cust_drops
   int get_cust_drops() const;
};
#endif
