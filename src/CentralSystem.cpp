#include <iostream>

#include "Server.hpp"

#define PORT 4000

int main() {
  Server server(PORT);

  server.Start();

  return 0;
}