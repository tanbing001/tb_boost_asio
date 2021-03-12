#include "boost/asio.hpp"
#include "boost/bind/bind.hpp"
#include "boost/thread/thread.hpp"

#include <iostream>

class Printer {
public:
  Printer(boost::asio::io_context& io_context)
      : strand_(boost::asio::make_strand(io_context)),
        timer1_(io_context, boost::asio::chrono::seconds(1)),
        timer2_(io_context, boost::asio::chrono::seconds(1)),
        count_(0) {
    timer1_.async_wait(boost::asio::bind_executor(
        strand_, boost::bind(&Printer::Print1, this)));

    timer2_.async_wait(boost::asio::bind_executor(
        strand_, boost::bind(&Printer::Print2, this)));
  }

  ~Printer() {
    std::cout << "Final count is " << count_ << std::endl;
  }

  void Print1() {
    if (count_ < 10) {
      std::cout << "Timer 1: " << count_ << std::endl;

      ++count_;
      timer1_.expires_after(std::chrono::seconds(1));
      timer1_.async_wait(boost::asio::bind_executor(
          strand_, boost::bind(&Printer::Print1, this)));
    }
  }

  void Print2() {
    if (count_ < 10) {
      std::cout << "Timer 2: " << count_ << std::endl;

      ++count_;
      timer2_.expires_after(std::chrono::seconds(1));
      timer2_.async_wait(boost::asio::bind_executor(
          strand_, boost::bind(&Printer::Print2, this)));
    }
  }

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  boost::asio::steady_timer timer1_;
  boost::asio::steady_timer timer2_;
  int count_;
};


int main() {
  boost::asio::io_context io_context;

  Printer p(io_context);
  
  boost::thread t(boost::bind(&boost::asio::io_context::run, &io_context));
  
  io_context.run();

  t.join();

	return 0;
}