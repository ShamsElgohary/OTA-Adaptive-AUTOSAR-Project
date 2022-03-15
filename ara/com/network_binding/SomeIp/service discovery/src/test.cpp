#include "memory"
#include <iostream>
#include"someip_sd_message.cpp"
//#include "SOMEIP.hpp"
#include <fstream>
#include "servicediscovery.cpp"
using namespace std;
int main()
{ 
   someip::someipHeader h1;
   std::stringstream ss;
   someip::someip_Message m1(h1,ss);
   someip_sd_message s;
   s.header=m1.header;
   /*
   s.setflag(0xFF);
   uint8_t w=s.flags;
   service_entry s1(1,1);
   service_entry *ptr2=&s1;
   unique_ptr<service_entry> ptr = unique_ptr<service_entry>(new service_entry(s1));
   s.AddEntry(ptr);
   ptr2=s1.create_find_service_entry(1,1);
   //ptr2->getTimeToLIve();
    fstream my_file;
    my_file.open("waleed", std::ios_base::out);
	if (!my_file) {
		cout << "File not created!";
	}
	else {
		cout << "File created successfully!";
      std::string s = ss.str();
      my_file<<ptr2->getTimeToLIve();
		my_file.close(); 
	}
   */
  servicediscovery obj;
  obj.offer_service(1,2,3,4);
	return 0;


}
