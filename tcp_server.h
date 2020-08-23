#pragma once

#include <vector>
#include <thread>

#include "libxsckt/socket_factory.h"

#define VERBOSE

namespace xsckt {

	class tcp_server {


	public:

		tcp_server(const address_t addr, const port_t port);

		~tcp_server();

		void run();

	private:

		tcp_server_socket passive_socket;	// created bound and listening
		std::vector<std::thread> client_threads;

	};

}