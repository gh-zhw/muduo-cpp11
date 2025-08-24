# muduo-cpp11

[![Language: C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B11)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/github/actions/workflow/status/YourGitHubUsername/muduo-cpp11/cmake.yml?branch=main)](https://github.com/YourGitHubUsername/muduo-cpp11/actions)
[![Stars](https://img.shields.io/github/stars/YourGitHubUsername/muduo-cpp11?style=social)](https://github.com/YourGitHubUsername/muduo-cpp11/)

**muduo-cpp11** is a modernized refactoring of the renowned muduo network library. This project aims to revitalize the original design by leveraging modern C++11 features, eliminating the dependency on Boost, and maintaining the high-performance, multi-Reactor, multi-thread model that muduo is famous for.

> **Note**: This project is inspired by and based on the original work of [**chenshuo**](https://github.com/chenshuo) and his [**muduo**](https://github.com/chenshuo/muduo) library. It is a study and modernization effort, not an official replacement.

## 🚀 Key Features

*   **Modern C++11**: Utilizes smart pointers (`std::shared_ptr`, `std::unique_ptr`), atomic operations (`std::atomic`), and other STL components for safer and more expressive code.
*   **Zero-Boost Dependency**: All Boost dependencies have been replaced with their C++ Standard Library equivalents, simplifying the build process and reducing the project's footprint.
*   **Core Architecture Preserved**: Faithfully implements the core `non-blocking + IO-multiplexing + thread-pool` architecture and the efficient **one-loop-per-thread** model.
*   **Efficient Buffer Design**: Implements a high-performance buffer class with separate reader and writer indices, minimizing memory copies for enhanced I/O efficiency.
*   **Clear Module Structure**: Well-organized components including `EventLoop`, `Channel`, `Poller` (EPoll), `EventLoopThread`, `EventLoopThreadPool`, and `TcpConnection`.

## 📁 Core Components

*   **EventLoop**: The core of the Reactor, responsible for polling events and executing callbacks.
*   **Channel**: Manages file descriptor events and ties them to user-provided callback functions.
*   **Poller**: An abstraction of the I/O multiplexing mechanism (epoll is implemented).
*   **EventLoopThread**: A thread that runs an `EventLoop`.
*   **EventLoopThreadPool**: A pool of `EventLoopThreads` for handling high concurrency and achieving load balancing across threads.
*   **Buffer**: An optimized input/output buffer for reducing system calls and network latency.
*   **TcpConnection**: Manages a TCP connection, using smart pointers for automatic lifecycle management.
*   **Acceptor**: Handles new incoming TCP connections.
*   **TcpServer**: A composite class that orchestrates all components to build a TCP server.

## 🔧 Build & Installation

### Prerequisites
*   A C++11 compliant compiler (GCC >= 4.8, Clang >= 3.3)
*   CMake (>= 3.10)
*   Linux system (for epoll)

### Steps
1.  Clone the repository:
    ```bash
    git clone https://github.com/gh-zhw/muduo-cpp11.git
    cd muduo-cpp11
    ```
2.  Create a build directory and run CMake:
    ```bash
    mkdir build && cd build
    cmake ..
    ```
3.  Compile the library and examples:
    ```bash
    make -j$(nproc)
    ```
4.  (Optional) Install the library and headers system-wide:
    ```bash
    sudo make install
    ```

## 🧪 Example: Echo Server

A simple echo server is provided to demonstrate the library's usage.

```cpp
// Code snippet from examples/echo_server.cpp
#include "net/TcpServer.h"
#include "net/EventLoop.h"
#include <iostream>

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time) {
    std::string msg = buf->retrieveAllAsString();
    std::cout << "Echoing " << msg.size() << " bytes: " << msg << std::endl;
    conn->send(msg); // Echo back the received data
}

int main() {
    EventLoop loop;
    InetAddress listenAddr(2007);
    TcpServer server(&loop, listenAddr, "EchoServer");

    server.setMessageCallback(onMessage);
    server.start();
    loop.loop();
}
