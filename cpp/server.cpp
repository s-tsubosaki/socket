#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

void on_open(websocketpp::connection_hdl hdl) {
  std::cout << "connectted" << std::endl;
}
void on_close(websocketpp::connection_hdl hdl) {
  std::cout << "disconnectted" << std::endl;
}
void on_message(websocketpp::connection_hdl, server::message_ptr msg) {
  std::cout << msg->get_payload() << std::endl;
}

int main() {
  server s;

  s.set_open_handler(&on_open);
  s.set_close_handler(&on_close);
  s.set_message_handler(&on_message);
  s.set_access_channels(websocketpp::log::alevel::all);
  s.set_error_channels(websocketpp::log::elevel::all);

  s.init_asio();
  s.listen(8080);
  s.start_accept();

  s.run();
}