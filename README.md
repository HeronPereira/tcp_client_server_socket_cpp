# Embedded Software Technical Challenge

This project implements a **TCP server** (POSIX) in C++ capable of receiving simultaneous connections from sensors, logging the received data, and managing the connections.

## Table of Contents

- [Requirements](#requirements)  
- [Safety and Concurrency](#safety-and-concurrency)  
- [Code Styling](#code-styling)  
- [Execution](#execution)  
- [Running with Docker](#running-with-docker)  
- [Notes](#notes)  
- [References](#references)  
- [Embedded Software Technical Challenge Description](#embedded-software-technical-challenge-description)  

## Requirements  
- C++17 or higher  
- CMake (version 3.13 or higher)  
- POSIX-compliant compiler (Linux recommended)  
- Docker  

## Safety and Concurrency  
File writes are protected with `std::mutex` to prevent corruption from multiple threads.

## Code Styling  
Configured with `.clang-format` to follow best practices in formatting and readability, based on the Google C++ Style Guide.

## Execution  
To clone the project:  
```bash
git clone https://github.com/HeronPereira/tcp_client_server_socket_cpp.git

cd tcp_client_server_socket_cpp
```

To run the server:  
```bash
mkdir build

cd build

cmake ..

make

./CentralSystem
```

## Running with Docker  

To run the program using Docker, in a terminal execute:  
```bash
docker build -t central-system .

docker run -p 4000:4000 central-system
```

To test client access, the Linux network tool **NetCat** was used. In another terminal, after the server is running, run:  
```bash
nc localhost 4000
{"temp":{"value":11,"unit":"ºC"},"loc":{"lat":-27.602409,"lon":-48.519126}}
```

## Notes  

During the development of this challenge:  
 - No unit tests were implemented.  
 - It was assumed there is no guaranteed newline at the end of the message.  
 - A mechanism was added to ensure the server shuts down even without active connections.  
 - Automatic disconnection was implemented after 60 seconds.  

## References  
Beej's Guide to Network Programming  
Using Internet Sockets: https://beej.us/guide/bgnet/html/

GeeksforGeeks - Socket Programming in C++:  
https://www.geeksforgeeks.org/socket-programming-in-cpp/

## Embedded Software Technical Challenge Description  

You are part of a project involving multiple sensors, and there is a need to centralize and store the collected data on a single device, called the Central System, which is connected to the same local (IPv4) network as the sensors.  
Your task is to implement a service that will run on the Central System. This service must allow the sensors to establish (and maintain) a socket connection to send their data, and the Central System must log the data into a single file.

## Project Requirements:  
* Implementation must be done using C++.  
* The application must be POSIX-compliant.  
* It is assumed that sensors know the IP address of the Central System.  
* Port 4000 must be used for communication.  
* The service must support simultaneous connections from different sensors.  
* Data must be written to a file in ASCII using the following format:  
```bash
<ip>:<message>
```

### Example:  
`File: SensorData.log`  
```bash
192.168.1.71:{"temp":{"value":11,"unit":"ºC"},"loc":{"lat":-27.602409,"lon":-48.519126}}
192.168.1.66:{"temp":{"value":29,"unit":"ºC"},"loc":{"lat":-3.116272,"lon":-59.983247}}
192.168.1.82:{"temp":{"value":21,"unit":"ºC"},"loc":{"lat":-15.790505,"lon":-47.885149}}
```

## Optional Additional Features (bonus points):  
* Considering the Central System might be cross-platform, develop a solution that runs the service inside a Docker container.  
* If a connection remains open for more than 60 seconds without receiving data, it must be closed by the server.  
* It is recommended to follow the Google C++ Style Guide.  
* Unit Tests.  

## Recommendations and Guidelines:  
* Aim for cohesive implementation with modular resource organization and meaningful naming.  
* The repository should include an example source code for using the library, including documentation with instructions for building the project and running the example. If there are other tests, include instructions for running them as well.  
* The software must be runnable on a recent Linux desktop distribution.  
* You may choose the build system you're most comfortable with (CMake, Makefile, etc.).