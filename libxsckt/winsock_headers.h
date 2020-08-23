#pragma once

#ifdef WIN32

#pragma comment(lib, "WS2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>
#include <sstream>
#include <stdexcept>

/**
 * \addtogroup xsckt
 * @{
 */
namespace xsckt {

	using address_t = const std::string;
	using sockfd_t = SOCKET;
	using port_t = const unsigned short;
	using flag_t = const int;

    static std::string _make_error_message() {
        auto err = WSAGetLastError();
        char msgbuf[256];   // for a message up to 255 bytes.
        msgbuf[0] = '\0';    // Microsoft doesn't guarantee this on man page.

        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,   // flags
            NULL,                // lpsource
            err,                 // message id
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),    // languageid
            msgbuf,              // output buffer
            sizeof(msgbuf),     // size of msgbuf, bytes
            NULL);               // va_list of arguments

        return (!*msgbuf) ? std::string("undefined error") : std::string(msgbuf);
    }

    //Windows specific...

    enum versions { v1_0 = 0x1, v1_1 = 0x101, v2_0 = 0x2, v2_1 = 0x102, v2_2 = 0x202 };

    /**
     * @brief startup - performs Windows specific initialization
     * @return string - winsock description and version info
     */
    static std::string startup() {
        WSADATA wsa_data;
        if (WSAStartup(v2_2, &wsa_data)) {
            throw std::runtime_error(_make_error_message());
        }
        std::stringstream ss;
        ss << "description: " << std::string(wsa_data.szDescription) << " - " << std::string(wsa_data.szSystemStatus);
        ss << "\nversion: " << std::to_string(wsa_data.wVersion & 0xFF) << "." << std::to_string((wsa_data.wVersion >> 8) & 0xFF);
        ss << "\nrequested: " << std::to_string(v2_2 & 0xFF) << "." << std::to_string((v2_2 >> 8) & 0xFF);
        return ss.str();
    }


    /**
     * @brief cleanup - performs Windows winsock specific closedown
     */
    static inline void cleanup() {
        if (WSACleanup()) {
            throw std::runtime_error(_make_error_message());
        }
    }

}   /*! @} */

#endif
