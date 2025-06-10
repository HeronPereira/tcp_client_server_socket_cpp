#include "Server.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <csignal>
#include <iostream>
#include <thread>

bool Server::running_ = true;

// constructor
Server::Server(int port) : port_(port) {
  signal(SIGINT, [](int sig) {
    std::cout << "Interrupt handle " << sig << std::endl;
    running_ = false;
  });
}

// destructor
Server::~Server() {}

void Server::Start() {
  // create server socket file descriptor
  std::cout << "Generating Socket...\n";
  server_file_descriptor_ = socket(AF_INET, SOCK_STREAM, 0);
  if (server_file_descriptor_ < 0) {
    std::cout << "Error generating socket\n";
    return;
  }

  // allows to use the same address and same port when reseting the application
  int opt = 1;
  setsockopt(server_file_descriptor_, SOL_SOCKET, SO_REUSEADDR, &opt,
             sizeof(opt));
  setsockopt(server_file_descriptor_, SOL_SOCKET, SO_REUSEPORT, &opt,
             sizeof(opt));

  // set address values to create the bind
  sockaddr_in server_address{};
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(port_);

  // do the bind with the real address and port
  const int bind_value =
      bind(server_file_descriptor_, (sockaddr*)&server_address,
           sizeof(server_address));
  if (bind_value < 0) {
    std::cout << "Error generating bind\n" << errno << "\n";
    return;
  }

  // start listen for connections
  std::cout << "Listenning...\n";
  if (listen(server_file_descriptor_, SOMAXCONN) < 0) {
    std::cout << "Error listenning\n" << errno << "\n";
    return;
  }

  // keep listenning for multiple connections
  while (running_) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(server_file_descriptor_, &read_fds);

    timeval timeout{};
    timeout.tv_usec = 100000;

    // verifies each 100ms if any connection was tried
    int activity = select(server_file_descriptor_ + 1, &read_fds, nullptr,
                          nullptr, &timeout);

    if (activity < 0 && errno != EINTR) {
      std::cout << "select error\n" << errno << "\n";
      break;
    }

    // if code is interrupted break out of the loop allowing to close the server
    // file descriptor
    if (!running_) {
      break;
    }

    // Only if a connection was tried follows up to the accept
    if (activity > 0 && FD_ISSET(server_file_descriptor_, &read_fds)) {
      // accept connections
      sockaddr_in client_address{};
      socklen_t client_len;

      int connection_file_descriptor = accept(
          server_file_descriptor_, (sockaddr*)&client_address, &client_len);
      if (connection_file_descriptor < 0) {
        std::cout << "Error generating connection socket\n" << errno << "\n";
        break;
      }
      std::string client_ip = inet_ntoa(client_address.sin_addr);
      std::thread(&Server::HandleClient, this, connection_file_descriptor,
                  client_ip)
          .detach();
    }
  }
  std::cout << "closing server...\n";
  close(server_file_descriptor_);
}

void Server::HandleClient(int client_file_descriptor, std::string client_ip) {
  char buffer[4096];

  while (true) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(client_file_descriptor, &read_fds);

    timeval timeout{};
    timeout.tv_sec = k_timeout_s_;

    int activity = select(client_file_descriptor + 1, &read_fds, nullptr,
                          nullptr, &timeout);
    if (activity <= 0) {
      std::cout << "Connection timed out: " << client_ip << std::endl;
      break;
    }

    ssize_t buffer_len =
        recv(client_file_descriptor, buffer, sizeof(buffer), 0);
    if (buffer_len <= 0) {
      break;
    }
    std::string data(buffer, buffer_len);
    std::cout << "Message from client: " << data << std::endl;

    writer_.Write(client_ip, buffer);
  }

  close(client_file_descriptor);
}
