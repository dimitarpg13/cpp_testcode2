#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

void print(const boost::system::error_code& /*e*/,
	   boost::asio::deadline_timer* t, int* count)
{
   std::cout << "Hello, world from timer callback!" << std::endl;
   if (*count < 5) {
       std::cout << *count << std::endl;
       ++(*count);
       t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
       t->async_wait(boost::bind(print, 
		 boost::asio::placeholders::error, t, count));
   }
}

int main (int argc, char* argv[]) {
    boost::asio::io_service io;
    int count = 0;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
    t.async_wait(boost::bind(print, 
	  boost::asio::placeholders::error, &t, &count));

    io.run();
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
