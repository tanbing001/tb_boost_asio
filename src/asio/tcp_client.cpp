#include <iostream>

#include "boost/array.hpp"
#include "boost/asio.hpp"

using boost::asio::ip::tcp;

enum { BUF_SIZE = 1024 };

int main(int argc, char* argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: client <host> <port>" << std::endl;
      return 1;
    }

    const char* host = argv[1];
    const char* port = argv[2];

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);

    tcp::resolver::results_type endpoints =
        resolver.resolve(tcp::v4(), host, port);

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);
    char buf[BUF_SIZE];
    std::size_t length = 0;
    do {
      std::cout << "Enter message: ";
      std::cin.getline(buf, BUF_SIZE);
      length = strlen(buf);
    } while (length == 0);

    boost::asio::write(socket, boost::asio::buffer(buf, length));

    std::cout << "Reply message: ";

    char reply[BUF_SIZE];

    boost::asio::read(socket, boost::asio::buffer(reply, length));

    std::cout.write(reply, length);

    std::cout << std::endl;

  } catch (boost::system::error_code ec) {
  }

  return 0;
}