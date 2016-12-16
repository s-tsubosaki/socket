#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
volatile static uint c_count = 0;

void on_open(websocketpp::connection_hdl hdl) {
  c_count++;
  std::cout << "connectted:" << c_count << std::endl;
}
void on_close(websocketpp::connection_hdl hdl) {
  c_count--;
  std::cout << "disconnectted:" << c_count << std::endl;
}

int main() {
  server s;

  s.set_open_handler(&on_open);
  s.set_close_handler(&on_close);
  s.clear_access_channels(websocketpp::log::alevel::all);

  s.init_asio();
  s.listen(8080);
  s.start_accept();

  s.run();
}