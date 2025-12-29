#ifndef OPTIONS_SIMULATOR_CLIENT_H
#define OPTIONS_SIMULATOR_CLIENT_H

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/ssl.hpp>

#include <string>
#include <functional>
#include <memory>

namespace beast = boost::beast;         
namespace websocket = beast::websocket; 
namespace net = boost::asio;         
namespace ssl = net::ssl;   
using tcp = boost::asio::ip::tcp;

// safe pointer capable of generating additional shared pointers
//for client to stay alive during async operations
class Client : public std::enable_shared_from_this<Client> {
    tcp::resolver resolver_;
    //nesting doll: websocket over ssl over tcp
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> ws_;
    beast::flat_buffer buffer_;
    std::function<void(std::string)> on_message_cb_;
    std::string subscription_msg_;

    std::string host_;
    std::string subscription_path_;
public:
    explicit Client(net::io_context& ioc, ssl::context& ctx); //explicit to avoid implicit conversion
    void run(const std::string& host, const std::string& port, const std::string& subscription_path);

    //async handlers
    void on_resolve(beast::error_code ec, tcp::resolver::results_type results);
    void on_connect(beast::error_code ec, tcp::resolver::endpoint_type ep);
    void on_ssl_handshake(beast::error_code ec);
    void on_handshake(beast::error_code ec);
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void set_callback(std::function<void(std::string)> cb) { on_message_cb_ = cb; }
};

#endif //OPTIONS_SIMULATOR_CLIENT_H