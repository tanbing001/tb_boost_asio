#include <iostream>
#include <string>

#include "boost/asio.hpp"

using boost::asio::ip::tcp;

// tcp server 流程
// 1. io_context
// 2. acceptor --->accept()->socket
// 3. read/write

enum { BUF_SIZE = 1024 };

void Session(tcp::socket socket) {
  try {
    while (true) {
      std::array<char, BUF_SIZE> data;

      boost::system::error_code ec;

      // 未收到客户端信息之前一直阻塞
      auto size = socket.read_some(boost::asio::buffer(data), ec);

      if (ec == boost::asio::error::eof) {
        std::cout << "Connection closed cleanly by peer." << std::endl;
        break;
      } else if (ec) {
        throw boost::system::system_error(ec);
      }

      boost::asio::write(socket, boost::asio::buffer(data, size));
    }
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << "port" << std::endl;
    return -1;
  }

  unsigned short port = std::stoi(argv[1]);

  try {
    boost::asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

    while (true) {
      std::cout << "New session." << std::endl;
      // 客户端连接之前一直阻塞
      Session(acceptor.accept());
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

	return 0;
}