#include "api/Client.h"
#include <iostream>

namespace net = boost::asio;
namespace ssl = net::ssl;
namespace websocket = boost::beast::websocket;

// use ssl bc apparently most exchanges require wss? 
Client::Client(net::io_context& ioc, ssl::context& ctx) : resolver_(net::make_strand(ioc)), ws_(net::make_strand(ioc), ctx) {}

// starts async process; resolve hostname into IP address
void Client::run(const std::string& host, const std::string& port, const std::string& subscription_path) {
    host_ = host;
    subscription_path_ = subscription_path;
    subscription_msg_ = R"({"jsonrpc":"2.0","method":"public/subscribe","params":{"channels":["ticker.)" + subscription_path + R"(.100ms"]},"id":1})";

    resolver_.async_resolve(
        host, 
        port, 
        beast::bind_front_handler(&Client::on_resolve, shared_from_this())
    ); // automatically calls on_reolve when done
}

void Client::on_resolve(beast::error_code ec, tcp::resolver::results_type results) {
    if (ec) return std::cerr << "Resolve Error: " << ec.message() << std::endl;

    // connect to the resolved IP address
    beast::get_lowest_layer(ws_).async_connect(
        results,
        beast::bind_front_handler(&Client::on_connect, shared_from_this())
    ); // calls on_connect when done
}

// handler that starts SSL handshake after TCP connection
void Client::on_connect(beast::error_code ec, tcp::resolver::endpoint_type ep) {
    if (ec) return std::cerr << "Connect Error: " << ec.message() << std::endl;

    // does SSL handshake
    ws_.next_layer().async_handshake(
        ssl::stream_base::client,
        beast::bind_front_handler(&Client::on_ssl_handshake, shared_from_this())
    );
}

void Client::on_ssl_handshake(beast::error_code ec) {
    if (ec) return std::cerr << "SSL Handshake Error: " << ec.message() << std::endl;

    // websocket handshake
    ws_.async_handshake(
        host_, 
        "/",
        beast::bind_front_handler(&Client::on_handshake, shared_from_this())
    );
}


void Client::on_handshake(beast::error_code ec) {
    if (ec) return std::cerr << "WebSocket Handshake Error: " << ec.message() << std::endl;

    // send subscription msg
    
    // sub to specific ticker; should prolly customize this
    // std::string sub_json = R"({"jsonrpc":"2.0","method":"public/subscribe","params":{"channels":["ticker.BTC-27DEC24-50000-C.100ms"]},"id":1})";
    
    ws_.async_write(
        net::buffer(subscription_msg_),
        [self = shared_from_this()](beast::error_code ec, std::size_t) {
            if (!ec) self->do_read();
        }
    );
}

void Client::do_read() {
    ws_.async_read(
        buffer_,
        beast::bind_front_handler(&Client::on_read, shared_from_this())
    );
}

void Client::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) {
        std::cerr << "Read Error: " << ec.message() << std::endl;
        return;
    }

    // convert to string & callback
    std::string message = beast::buffers_to_string(buffer_.data());
    if (on_message_cb_) {
        on_message_cb_(std::move(message));
    }

    // clears read buffer and initiate next read
    buffer_.consume(buffer_.size());
    do_read();
}