#include "tcp_client.h"

#include <string>
#include <iostream>
#include <csignal>
#include <cassert>
#include <thread>

#include "libxsckt/socket_factory.h"

namespace xsckt {

	tcp_echo_client::tcp_echo_client(const std::string addr, const unsigned short port) :
		addr(addr),
		port(port)
	{
		assert(signal(SIGINT, ctrl_c) != SIG_ERR); // install handler
	}

	void tcp_echo_client::run() {


		std::cout << "thread id " << std::this_thread::get_id() << " running Echo Client v 0.1\t(press Ctrl_C to quit)\n" << std::endl;

		std::string line;
		try {
			std::cout << "attempting to connect to " << addr << ":" << port << "\n\n";
			tcp_client_socket sckt(addr, port);
			std::cout << "tcp_client " << sckt.hostname() << "@" << addr << ":" << port << "\n";
			//init handshake protocol
			//s.write - info about client
			//s.read - info about server
			while (!quit) {
				std::getline(std::cin, line);
				if (!quit) {
					if (std::cin.fail()			// str.max_size() characters have been stored
						|| std::cin.eof()) {	// or end-of-file condition on input
						std::cin.clear();		// reset cin state
					}
					else {
						sckt.write(line);
						std::cout << sckt.read() << std::endl;
					}
				}
			}
			sckt.write("quit");
		}
		catch (std::runtime_error& e) {
			std::cerr << e.what() << "\n\n";
		}

		signal(SIGINT, SIG_DFL); // restore default handler
	}

	void tcp_echo_client::ctrl_c(int param) {
		std::cout << "\npress <enter> to quit" << std::endl;
		quit = true;
	}

	volatile bool tcp_echo_client::quit = false;

}