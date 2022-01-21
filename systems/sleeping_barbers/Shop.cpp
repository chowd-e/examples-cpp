//--------------------------------------------------------------------
// Shop.cpp
// Implementation file for Shop class
// Author: Chase Howard
// Class: CSS 503B [University of Washington Bothell]
// Date: 2021-05-09
//-------------------------------------------------------------------- 
//	Description: 
//    See header file

//    See attached documentation for more in depth description
#include "Shop.h"

//------------------------------------------------------------------------
//                                  MUTATORS
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// init: initializes Shop
//------------------------------------------------------------------------
// Pre: none
// Post: shop has been initialized with all condition variables and mutexes
void Shop::init(int num_barbers) 
{
   pthread_mutex_init(&mutex_, NULL);
   pthread_cond_init(&cond_next_customer_, NULL);
   pthread_cond_init(&cond_wake_barber_, NULL);

   for(int i = 0; i < num_barbers; i++)
   {
      service_chairs_.push_back(nullptr);

      pthread_cond_t served;
      pthread_cond_init(&served, NULL);
      cond_customer_served_.push_back(served);

      pthread_cond_t paid;
      pthread_cond_init(&paid, NULL);
      cond_barber_paid_.push_back(paid);

      Barber *barber = new Barber(i);
      barbers_.push_back(barber);
   }
}

//------------------------------------------------------------------------
// ~Shop: Destructor
//------------------------------------------------------------------------
// Pre: none
// Post: Dynamic memory allocated by shop has been properly handled. 
Shop::~Shop()
{
   for(long unsigned int i = 0; i < barbers_.size(); i++)
   {
      delete barbers_[i];
   }
}

//------------------------------------------------------------------------
// createBarber: Creates and dynamically allocates memory for a barber
//------------------------------------------------------------------------
// Pre: none
// Post: New barber has been created with an id equal to integer passed as
//    argument. pointer to memory on heap has been returned. 
Shop::Barber* Shop::createBarber(int id)
{
   Barber *newBarber = new Barber(id);
   barbers_.push_back(newBarber);
   return newBarber;
}

//------------------------------------------------------------------------
// createCustomer: Creates and dynamically allocates memory for a customer
//------------------------------------------------------------------------
// Pre: none
// Post: New customer has been created with an id equal to integer passed as
//    argument. pointer to memory on heap has been returned. 
Shop::Customer* Shop::createCustomer(int id)
{
   Customer *newCust = new Customer(id);
   return newCust;
}

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
int Shop::visitShop(int id_customer) 
{
   pthread_mutex_lock(&mutex_);
   
   Customer *customer = createCustomer(id_customer);
   
   if(!isEmptyServiceChair())
   {
      // If all chairs are full then leave shop
      if((const int)waiting_chairs_.size() >= max_waiting_cust_) 
      {
         print(customer);
         ++cust_drops_;
         delete customer;
         customer = nullptr;
         pthread_mutex_unlock(&mutex_);
         return -1;
      }

      // if all barbers are busy but chairs aren't full, take a seat and wait
      // or if there is a sleeping barber with a customer in their chair, wait
      //    in queue
      if(sleeping_barbers_.empty() || !isEmptyServiceChair())
      {
         waiting_chairs_.push(customer);
         customer->state = WAITING;
         print(customer);
         // first customer in line waits for chair to open up
         while( get_nextCustomerID() != id_customer || !isEmptyServiceChair())
            pthread_cond_wait(&cond_next_customer_, &mutex_);

         waiting_chairs_.pop();
      } 
   }
   
   // getup from waiting chairs and find which chair / barber to go to
   customer->myBarber = get_nextBarberID();
   customer->state = STAGING;
   print(customer);

   // customer sits at service chair for next barber [must be empty]
   assert(service_chairs_[customer->myBarber] == nullptr);

   service_chairs_[customer->myBarber] = customer;
   customer->state = INSERVICE;
   print(customer);

   // notify barbers of customer sitting in chair
   pthread_cond_broadcast(&cond_wake_barber_);

   pthread_mutex_unlock(&mutex_); 
   return customer->myBarber;
}

//------------------------------------------------------------------------
// leaveShop: Customers procedure for leaving the Shop
//------------------------------------------------------------------------
// Pre: visitShop has been called sucessfully
// Post: Customer is sitting at a barber chair getting their haircut. They 
//    will recieve a signal from the barber that the cut is complete, they
//    will then pay and signal to the barber that their payment is complete
//    they will then say goodbye and the customer will be deleted - dynamic
//    memory is handled within here for the customer. 
void Shop::leaveShop(int id_barber) 
{
   pthread_mutex_lock( &mutex_ );
   
   // customer is waiting in chair belonging to barber
   Barber* barber = barbers_[id_barber];
   Customer* customer = service_chairs_[id_barber];

   // wait to pay once signal from barber the cut is done
   pthread_cond_wait(&cond_customer_served_[barber->id], &mutex_);
   customer->state = PAYMENT;
   // print(customer); // no desired output here

   // Pay the barber and signal barber appropriately
   pthread_cond_signal(&cond_barber_paid_[barber->id]);

   // payment completed - say bye now
   customer->state = COMPLETE;
   print(customer);
   
   // destroy customer
   delete customer;
   customer = nullptr;
   service_chairs_[barber->id] = nullptr;


   pthread_mutex_unlock(&mutex_);
}

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
void Shop::helloCustomer(int id_barber) 
{
   pthread_mutex_lock(&mutex_);
   Barber* barber = barbers_[id_barber];

   // initialization state - just started working
   if(barber->state == INVALID)
   {
      barber->state = WAITING;
      sleeping_barbers_.push_back(barber);

      if(!isCustomerWaiting()) 
         print(barber);
   } 
   
   // while barber chair is empty and not first in line - loop on
   while (isEmptyServiceChair(barber->id)) 
   {
      //front of the line singal customer
      if (get_nextBarberID() == barber->id)
      {
         pthread_cond_broadcast(&cond_next_customer_);
      }
      // wait for customer to signal their sitting in a chair
      pthread_cond_wait(&cond_wake_barber_, &mutex_);
   }
   
   // barber woke up -> go to chair to cut customer who woke them
   sleeping_barbers_.pop_front();
   
   // identify customer from service chair
   barber->currCustomer = service_chairs_[barber->id]->id;
   barber->state = STAGING;
   print(barber);
   
   pthread_mutex_unlock( &mutex_ );
}

 //------------------------------------------------------------------------
// createCustomer: Creates and dynamically allocates memory for a customer
//------------------------------------------------------------------------
// Pre: helloCustomer has been called
// Post: Barber notifies customer in chair that haircut is complete and 
//    waits for payement from customer. The customer leaves, the chair is 
//    cleared for a new customer, and the barber enters the queue for a 
//    new customer
void Shop::byeCustomer(int id_barber) 
{
   pthread_mutex_lock(&mutex_);
   Barber *barber = barbers_[id_barber];

   // Hair Cut-Service is done so signal customer and wait for payment
   barber->state = PAYMENT;
   print(barber);
   // signal to the customer sitting in the chair that haircut is complete
   // use barber id as index -> equivalent to the seat the customer sits in
   pthread_cond_signal(&cond_customer_served_[barber->id]);

   // wait for customer to pay the barber
   pthread_cond_wait(&cond_barber_paid_[barber->id], &mutex_);
   barber->state = COMPLETE;
   print(barber);

   barber->currCustomer = -1;

   // barber has re-entered queue - either front or back depending if there
   // are customers waiting
   barber->state = WAITING;
   if(isCustomerWaiting())
   {
      sleeping_barbers_.push_front(barber);
      pthread_cond_broadcast(&cond_next_customer_);
   }else
   {
      sleeping_barbers_.push_back(barber); // head to wait queue
      print(barber);
   }

   pthread_mutex_unlock( &mutex_ );  // unlock
}

//------------------------------------------------------------------------
//                                  ACCESSORS
//------------------------------------------------------------------------
   
//------------------------------------------------------------------------
// get_cust_drops: return total # of customers unable to be serviced
//------------------------------------------------------------------------
// Pre: none
// Post: returned value of member variable cust_drops  
int Shop::get_cust_drops() const
{
    return cust_drops_;
}

//------------------------------------------------------------------------
// get_nextCustomerID: return ID of next customer in the wait queue
//------------------------------------------------------------------------
// Pre: none
// Post: returned ID of next customer waiting in line or returned -1 if no
//    customers are waiting
int Shop::get_nextCustomerID() const
{
   if(!waiting_chairs_.empty())
      return waiting_chairs_.front()->id;

   return -1;
}

//------------------------------------------------------------------------
// get_nextBarberID: return ID of next barber in the wait queue
//------------------------------------------------------------------------
// Pre: none
// Post: returned ID of next barber waiting in line or returned -1 if no
//    barbers are waiting
int Shop::get_nextBarberID() const
{
   if(!sleeping_barbers_.empty())
      return sleeping_barbers_.front()->id;

   return 1;
}

//------------------------------------------------------------------------
// isEmptyServiceChair: checks if any service chairs are available
//------------------------------------------------------------------------
// Pre: none
// Post: boolean return indicating if there are any empty chairs available
bool Shop::isEmptyServiceChair() const
{
   // no barbers sleeping -> they're all working
   if(sleeping_barbers_.empty())
      return false; // no empty chairs
   // > 0 barbers sleeping, does next barber have customer in chair already
   return service_chairs_[get_nextBarberID()] == nullptr;
}

//------------------------------------------------------------------------
// isEmptyServiceChair: checks if any service chairs are available
//------------------------------------------------------------------------
// Pre: none
// Post: boolean return indicating if there are any empty chairs available
bool Shop::isEmptyServiceChair(int id_barber) const
{
   // no barbers sleeping -> they're all working
   if(sleeping_barbers_.empty())
      return false; // no empty chairs
   // > 0 barbers sleeping, does next barber have customer in chair already
   return service_chairs_[id_barber] == nullptr;
}

//------------------------------------------------------------------------
// isCustomerWaiting: checks if any customers are waiting in teh queue
//------------------------------------------------------------------------
// Pre: none
// Post: boolean return indicating if there are any customers currently 
//    wiating
bool Shop::isCustomerWaiting() const
{
   return !waiting_chairs_.empty();
}

//------------------------------------------------------------------------
// int2string: converts integer argument to a string
//------------------------------------------------------------------------
// Pre: none
// Post: integer argument has been converted to a string and returned
string Shop::int2string(int i) 
{
   stringstream out;
   out << i;
   return out.str( );
}

//------------------------------------------------------------------------
// print(Customer): prints customer information relating to state
//------------------------------------------------------------------------
// Pre: none
// Post: prints customer ID as well as information relating to its state to
//    cout
void Shop::print(Customer *customer)
{
   string msg, id_cust, id_barb;
   id_cust = int2string(customer->id);
   id_barb = int2string(customer->myBarber);
   switch(customer->state) 
   {
      case WAITING:
         msg = "takes a waiting chair. # waiting seats available = "
            + int2string(max_waiting_cust_ - waiting_chairs_.size());
         break;
      case STAGING:
         msg = "moves to service chair [" + id_barb 
            + "]. # waiting seats available = "
            + int2string(max_waiting_cust_ - waiting_chairs_.size());
         break;
      case INSERVICE:
         msg = "wait for barber [" + id_barb + "] to be done with hair-cut";
         break;
      case PAYMENT:
         msg = "pays barber [" + id_barb + "]";
         break;
      case COMPLETE:
         msg = "says goodbye to barber [" + id_barb + "]";
         break;
      case INVALID:
         msg = "leaves the shop, no available waiting chairs.";
         break;
      default:
         msg = "ERROR";
         break;
   }
   cout << "customer[" << customer->id << "]: " << msg << endl;
}

//------------------------------------------------------------------------
// print(Barber): prints barber information relating to state
//------------------------------------------------------------------------
// Pre: none
// Post: prints barber ID as well as information relating to its state to
//    cout
void Shop::print(Barber *barber)
{
   string msg, id_cust;
   id_cust = int2string(barber->currCustomer);

   switch(barber->state) 
   {
      case WAITING:
         msg = "sleeps because of no customers.";
         break;
      case STAGING:
         msg = "starts a hair-cut service for customer[" 
            + id_cust + "]";         
         break;
      case INSERVICE:
         //msg = "starts a haircut service for customer [" + id_cust + "]";
         break;
      case PAYMENT:
         msg = "completed a hair-cut service for customer [" + id_cust + "]";
         break;
      case COMPLETE:
         msg = "calls in another customer";
         break;
      default:
         msg = "Error";
         break;
   }
   cout << "barber  [" << barber->id << "]: " << msg << endl;
}