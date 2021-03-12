#include "boost/asio.hpp"
#include "boost/bind/bind.hpp"

#include <iostream>

class Printer {
public:
  Printer(boost::asio::io_context& io_context)
      : timer_(io_context, boost::asio::chrono::seconds(1)), count_(0) {
    timer_.async_wait(boost::bind(&Printer::Print, this));
  }

  ~Printer() {
    std::cout << "Final count is " << count_ << std::endl;
  }

  void Print() {
    if (count_ < 5) {
      std::cout << count_ << std::endl;
      ++count_;

      timer_.expires_after(std::chrono::seconds(1));
      timer_.async_wait(boost::bind(&Printer::Print, this));
    }
  }

private:
  boost::asio::steady_timer timer_;
  int count_;
};

int main() {
  boost::asio::io_context io_context;
  Printer p(io_context);
  io_context.run();

  return 0;
}
