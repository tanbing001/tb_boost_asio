#include <iostream>
#include "boost/asio.hpp"

int main() {
  // All programs that use asio need to have at least one I/O execution context
  boost::asio::io_context io_context;

  boost::asio::steady_timer timer(io_context, boost::asio::chrono::seconds(5));

  timer.wait();
  std::cout << "Hello, world!" << std::endl;

  return 0;
}