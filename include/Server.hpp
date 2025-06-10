#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <netinet/in.h>

#include "LogWriter.hpp"

class Server {
 public:
  explicit Server(int port);
  ~Server();

  void Start();

  void HandleClient(int client_file_descriptor, std::string client_ip);

  static bool running_;

 private:
  int port_;
  int server_file_descriptor_ = -1;
  LogWriter writer_;
  const int k_timeout_s_ = 60;
};

#endif
