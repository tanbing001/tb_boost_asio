#include <iostream>

#include "boost/asio.hpp"
#include "boost/bind/bind.hpp"

// No arguments
void Print(const boost::system::error_code&) {
  std::cout << "Hello, world!" << std::endl;
}

void PrintArgument(const boost::system::error_code& err,
                   boost::asio::steady_timer* timer, int* count) {
  if (*count < 5) {
    std::cout << *count << std::endl;
    ++(*count);
  //timer->expires_at(timer->expiry() + boost::asio::chrono::seconds(1));

  timer->expires_after(std::chrono::seconds(1));

  timer->async_wait(boost::bind(PrintArgument, boost::asio::placeholders::error,
                                timer, count));
  }
}

int main() {
  boost::asio::io_context io_context;

  boost::asio::steady_timer timer(io_context, boost::asio::chrono::seconds(1));

  // timer.async_wait(&Print);

  int count = 0;
  timer.async_wait(boost::bind(PrintArgument, boost::asio::placeholders::error,
                               &timer, &count));

  std::cout << "Before run" << std::endl;

  io_context.run();

  std::cout << "Final count is: " << count << std::endl;

  return 0;
}
