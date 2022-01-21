#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
#include "Shop.h"
using namespace std;

void *barber(void *);
void *customer(void *);

// customer comes in and hops in queue

// ThreadParam class
// This class is used as a way to pass more
// than one argument to a thread. 
class ThreadParam
{
public:
    ThreadParam(Shop* myShop, int id, int service_time) :
        shop(myShop), id(id), service_time(service_time) {};
    Shop* shop;         
    int id;             
    int service_time;    
};

int main(int argc, char *argv[]) 
{

   if (argc != 5)
   {
       cout << "Usage: num_barbers num_chairs num_customers service_time"
          << endl;
       return -1;
   }

   int num_barbers = atoi(argv[1]);
   int num_chairs = atoi(argv[2]);
   int num_customers = atoi(argv[3]);
   int service_time = atoi(argv[4]);

   if(num_barbers < 1 || num_chairs < 0 || num_customers < 0 ||
      service_time < 0)
   {
      cout << "Usage: only positive values, must have > 0 barbers" << endl;
      return -1;
   }

   //Single barber, one myShop, many customers
   vector<pthread_t> barber_threads;
   vector<pthread_t> customer_threads;

   Shop myShop(num_barbers, num_chairs);

   // start thread for each barber with params defined
   for (int i = 0; i < num_barbers; i++)
   {
      ThreadParam *barber_param = new ThreadParam(&myShop, i, service_time);
      pthread_t newThread;
      barber_threads.push_back(newThread);
      pthread_create(&barber_threads[i], NULL, barber, barber_param);
   }

   for (int i = 0; i < num_customers; i++) 
   {
      usleep(rand() % 1000);
      int id = i + 1;
      ThreadParam* customer_param = new ThreadParam(&myShop, id, 0);
      pthread_t newThread;
      customer_threads.push_back(newThread);
      pthread_create(&customer_threads[i], NULL, customer, customer_param);
   }

   // Wait for customers to finish and cancel barber
   for (int i = 0; i < num_customers; i++)
   {
      pthread_join(customer_threads[i], NULL);
   }

   // customers have all left, cancel barber threads
   for (int i = 0; i < num_barbers; i++)
   {
      pthread_cancel(barber_threads[i]);
      // pthread_join(barber_threads[i], NULL);
   }

   cout << "# customers who didn't receive a service = " 
      << myShop.get_cust_drops() << endl;
   return 0;
}

void *barber(void *arg) 
{
   ThreadParam* barber_param = (ThreadParam*) arg;
   Shop& myShop = *barber_param->shop;
   int service_time = barber_param->service_time;
   int id_barber = barber_param->id;
   delete barber_param;

   while(true) 
   {
      myShop.helloCustomer(id_barber);
      usleep(service_time);
      myShop.byeCustomer(id_barber);
   }

   return nullptr;
}

void *customer(void *arg) 
{
   ThreadParam* customer_param = (ThreadParam*)arg;
   Shop& myShop = *customer_param->shop;
   int id_customer = customer_param->id;
   delete customer_param;
   int myBarberID = myShop.visitShop(id_customer);

   if (myBarberID >= 0)
   {
       myShop.leaveShop(myBarberID);
   }
   return nullptr;
}
