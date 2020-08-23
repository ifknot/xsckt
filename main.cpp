#include <iostream>

#include "tcp_server.h"

#define SERVER

int main() {

#ifdef WIN32
	std::cout << xsckt::startup() << "\n";
	//net::startup();
#endif 

#ifdef SERVER
	try {
		xsckt::tcp_server s(xsckt::LOOPBACK_ADDR, xsckt::DEFAULT_PORT);
		s.run();
	}
	catch (std::runtime_error& e) {
		std::cerr << e.what();
	}
#else
	try {
		xsckt::tcp_echo_client c(net::LOOPBACK_ADDR, net::DEFAULT_PORT);
		c.run();
	}
	catch (std::runtime_error& e) {
		std::cerr << e.what() << "\n\n";
	}
#endif

#ifdef WIN32
	xsckt::cleanup();
#endif

	system("pause");
	return 0;
  
}
