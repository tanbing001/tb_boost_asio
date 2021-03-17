#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "boost/asio.hpp"
#include "boost/core/ignore_unused.hpp"

using boost::asio::ip::tcp;

enum { BUF_SIZE = 1024 };

class TcpClient {
public:
  TcpClient(boost::asio::io_context& io_context, const std::string& host,
            const std::string& port);

private:
  void OnResolve(boost::system::error_code ec, tcp::endpoint endpoint);

  void OnConnect(boost::system::error_code ec);

  void DoWrite();

  void OnWrite(boost::system::error_code ec, size_t length);

  void OnRead(boost::system::error_code ec, std::size_t length);

private:
  tcp::socket socket_;
  tcp::resolver resolver_;

  char cin_buf_[BUF_SIZE];
  std::array<char, BUF_SIZE> buf_;
};

TcpClient::TcpClient(boost::asio::io_context& io_context,
                     const std::string& host, const std::string& port)
    : socket_(io_context), resolver_(io_context) {
  resolver_.async_resolve(
      tcp::v4(), host, port,
      std::bind(&TcpClient::OnResolve, this, std::placeholders::_1,
                std::placeholders::_2));
}

void TcpClient::OnResolve(boost::system::error_code ec, tcp::endpoint endpoint) {
  if (ec) {
    std::cerr << "Resolve: " << ec.message() << std::endl;
  } else {
    boost::asio::async_connect(
        socket_, endpoint,
        std::bind(&TcpClient::OnConnect, this, std::placeholders::_1));
  }
}

void TcpClient::OnConnect(boost::system::error_code ec) {
  if (ec) {
    std::cerr << "Connect: " << ec.message() << std::endl;
    socket_.close();
  } else {
    DoWrite();
  }
}

void TcpClient::DoWrite() {
  std::size_t length = 0;

  std::cout << "Enter message: ";
  std::cin.getline(cin_buf_, BUF_SIZE);
  length = strlen(cin_buf_);

  boost::asio::async_write(
      socket_, boost::asio::buffer(cin_buf_, length),
      std::bind(&TcpClient::OnWrite, this, std::placeholders::_1,
                std::placeholders::_2));
}

void TcpClient::OnWrite(boost::system::error_code ec, size_t length) {
  boost::ignore_unused(length);

  if (!ec) {
    std::cout << "Reply is: ";
    socket_.async_read_some(
        boost::asio::buffer(buf_),
        std::bind(&TcpClient::OnRead, this, std::placeholders::_1,
                  std::placeholders::_2));
  }
}

void TcpClient::OnRead(boost::system::error_code ec, std::size_t length) {
  if (!ec) {
    std::cout.write(buf_.data(), length);
    std::cout << std::endl;
  }

  std::cout << "Press any key to continue!" << std::endl;

  getchar();

  std::cout << "Shutdown socket..." << std::endl;

  boost::system::error_code ec2;
  socket_.shutdown(tcp::socket::shutdown_both, ec2);

  if (ec2) {
    std::cerr << "Shutdown socket error: " << ec2.message() << std::endl;
    ec2.clear();
  }

  getchar();

  socket_.close(ec2);

  if (ec2) {
    std::cerr << " Socket close error: " << ec2.message() << std::endl;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << "<host> <port>" << std::endl;
    return -1;
  }

  std::string host = argv[1];
  std::string host = argv[2];

  boost::asio::io_context io_context;

  TcpClient(io_context, host, port);
  io_context.run();

  return 0;
}