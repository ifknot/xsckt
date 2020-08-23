#include "tcp_server.h"

#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace xsckt {

	tcp_server::tcp_server(const std::string addr, const unsigned short port) :
		passive_socket(tcp_server_socket(addr, port))
	{
#ifdef VERBOSE
		std::cout << "thread id " << std::this_thread::get_id() << " running tcp_server v0.1 " << passive_socket.hostname() << "@" << addr << ":" << port << "\n";
#endif // VERBOSE
	}

	tcp_server::~tcp_server() {
		/*
		for (auto& t : client_threads) {
			t.join();
		}
		*/
	}

	void tcp_server::run() {
		auto server_name = passive_socket.hostname();

		while (true) {
			sockfd_t active_sockfd;
			try {
				std::cout << "listening...\n" << std::endl;
				active_sockfd = passive_socket.accept_from();
			}
			catch (std::exception& e) {
				throw e;
			}
			std::cout << "spawning...\n";
			client_threads.push_back(std::thread([active_sockfd, server_name]() {
				try {
					tcp_active_socket active_sckt(active_sockfd, blocking_t::NONBLOCKING);
					std::cout << "client thread " << std::this_thread::get_id() << " using active socket handle " << active_sockfd << std::endl;
					while (true) {
						try {
							auto line = active_sckt.read();
							if (line == "quit") {
								throw std::runtime_error("No error.");
							}
							std::for_each(line.begin(), line.end(), [](char& c) {
								c = ::toupper(c);
								});
							active_sckt.write(line);
						}
						catch (const std::exception& e) {
							if (WSAGetLastError() != WSAEWOULDBLOCK) {
								throw(e);
							}
							else {
								std::this_thread::yield();
							}
						}
					}
				}
				catch (const std::exception& e) {
					std::cout << "client thread " << std::this_thread::get_id() << " ended with message:\n" << e.what() << std::endl;
				}
				}));
		}
	}

}