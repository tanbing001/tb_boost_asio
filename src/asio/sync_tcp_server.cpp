#include <iostream>
#include <string>

#include "boost/asio.hpp"

using boost::asio::ip::tcp;

enum { BUF_SIZE = 1024 };

void Session(tcp::socket socket) {
  try {
    while (true) {
      std::array<char, BUF_SIZE> data;

      boost::system::error_code ec;

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

    //tcp::socket socket(io_context);

    //// Block and wait for connection.
    //acceptor.accept(socket);

    while (true) {
      //std::array<char, BUF_SIZE> data;

      //boost::system::error_code ec;

      //auto size = socket.read_some(boost::asio::buffer(data), ec);

      //if (ec == boost::asio::error::eof) {
      //  std::cout << "Connection closed cleanly by peer." << std::endl;
      //  break;
      //} else if (ec) {
      //  throw boost::system::system_error(ec);
      //}

      //boost::asio::write(socket, boost::asio::buffer(data, size));
      Session(acceptor.accept());
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

	return 0;
}