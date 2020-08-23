#ifdef WIN32

#include "windows_winsock_socket.h"

#include <cassert>
#include <stdexcept>
#include <string.h>

/**
 * \addtogroup xsckt
 * @{
 */
namespace xsckt {

	base_socket::base_socket(const sockfd_t socket) :
        _long_addr(nullptr),
        _hints(addrinfo{ 0 }),
        _raddr(sockaddr_in{ 0 })
    {
        assert(_socket > 0);
        _socket = socket;
        char optval = 1;
        if (setsockopt(_socket, 
            SOL_SOCKET,     // option at the socket level
            SO_REUSEADDR,   // reuse the address
            &optval, 
            sizeof(int)) == SOCKET_ERROR) {
                throw std::runtime_error(_make_error_message());
        }
    }

    base_socket::base_socket(const short address_family, const int socket_type, const int protocol) :
        _address_family(address_family),
        _socket_type(socket_type),
        _protocol(protocol)
    {
        memset(&_hints, 0, sizeof(struct addrinfo));
        _hints.ai_family = address_family;
        _hints.ai_socktype = socket_type;
        _hints.ai_flags = 0;
        _hints.ai_protocol = protocol;

        _socket = socket(_address_family, _socket_type, _protocol);
        if (_socket == INVALID_SOCKET) {
            throw std::runtime_error(_make_error_message());
        }
        char optval = 1;
        if (setsockopt(_socket, 
            SOL_SOCKET,     // option at the socket level
            SO_REUSEADDR,   // reuse the address
            &optval, 
            sizeof(int)) == SOCKET_ERROR) {
                throw std::runtime_error(_make_error_message());
        }
        assert(_socket > 0);
    }

    int base_socket::_assign_address(const std::string& address, const unsigned short port) {
        // get server ip fit args
        return getaddrinfo(address.c_str(), std::to_string(port).c_str(), &_hints, &_long_addr);  
    }



    std::string base_socket::_make_error_message() {
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

        return (!*msgbuf) ?std::string("undefined error") : std::string(msgbuf);
    }

}   /*! @} */

#endif