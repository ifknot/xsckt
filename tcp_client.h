#pragma once

#include <string>

namespace xsckt {

	class tcp_echo_client {

	public:

		tcp_echo_client(const std::string addr, const unsigned short port);

		void run();

	private:

		const std::string addr;

		const unsigned short port;

		static void ctrl_c(int param);

		static volatile bool quit;

	};

}
