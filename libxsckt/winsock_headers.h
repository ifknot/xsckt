#pragma once

#ifdef WIN32

#pragma comment(lib, "WS2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

/**
 * \addtogroup xsckt
 * @{
 */
namespace xsckt {

	using address_t = const std::string;
	using sockfd_t = SOCKET;
	using port_t = const unsigned short;
	using flag_t = const int;

}   /*! @} */

#endif
