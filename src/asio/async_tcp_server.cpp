#include <ctime>
#include <iostream>
#include <string>

#include "boost/asio.hpp"
#include "boost/bind/bind.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"

using boost::asio::ip::tcp;

std::string MakeDaytimeString() {
  std::time_t now = time(nullptr);

  return ctime(&now);
}

class TcpConnection : public boost::enable_shared_from_this<TcpConnection> {
public:
  typedef boost::shared_ptr<TcpConnection> pointer;

  static pointer Create(boost::asio::io_context& io_context) {
    return pointer(new TcpConnection(io_context));
  }

  tcp::socket& socket() {
    return socket_;
  }

  void Start() {
    message_ = MakeDaytimeString();
    boost::asio::async_write(
        socket_, boost::asio::buffer(message_),
        boost::bind(&TcpConnection::HandleWrite, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

private:
  TcpConnection(boost::asio::io_context& io_context) : socket_(io_context) {
  }

  void HandleWrite(const boost::system::error_code& error,
                   size_t bytes_transferred) {
  }

private:
  tcp::socket socket_;

  std::string message_;
};

class TcpServer {
public:
  TcpServer(boost::asio::io_context& io_context)
      : io_context_(io_context),
        acceptor_(io_context, tcp::endpoint(tcp::v4(), 13)) {
    StartAccept();
  }

private:
  void StartAccept() {
    TcpConnection::pointer new_connection = TcpConnection::Create(io_context_);

    acceptor_.async_accept(
        new_connection->socket(),
        boost::bind(&TcpServer::HandleAccept, this, new_connection,
                    boost::asio::placeholders::error));
  }

  void HandleAccept(TcpConnection::pointer new_connection,
                    const boost::system::error_code& error) {
    if (!error) {
      new_connection->Start();
    }

    StartAccept();
  }

private:
  boost::asio::io_context& io_context_;
  tcp::acceptor acceptor_;
};

int main() {
  try {
    boost::asio::io_context io_context;

    TcpServer server(io_context);

    io_context.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}