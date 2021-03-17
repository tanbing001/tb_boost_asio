#include "boost/asio.hpp"
#include "boost/core/ignore_unused.hpp"

#include <iostream>

int main() {
  boost::asio::io_context io_context;
  boost::asio::steady_timer timer(io_context, boost::asio::chrono::seconds(3));

  timer.async_wait([](boost::system::error_code ec) {
    boost::ignore_unused(ec);

    std::cout << "Lambda expression has been run." << std::endl;
  });

  io_context.run();
  return 0;
}
