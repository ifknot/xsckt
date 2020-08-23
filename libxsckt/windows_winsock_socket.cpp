#ifdef WIN32

#include "windows_winsock_socket.h"

#include <cassert>
#include <stdexcept>
#include <string.h>
#include <array>

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
                throw std::runtime_error(make_error_message());
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
            throw std::runtime_error(make_error_message());
        }
        char optval = 1;
        if (setsockopt(_socket, 
            SOL_SOCKET,     // option at the socket level
            SO_REUSEADDR,   // reuse the address
            &optval, 
            sizeof(int)) == SOCKET_ERROR) {
                throw std::runtime_error(make_error_message());
        }
        assert(_socket > 0);
    }

    base_socket::~base_socket() {
        shutdown(_socket, SD_BOTH);
        closesocket(_socket);
    }

    void base_socket::bind_to(address_t& address, port_t port) {
        auto e = _getaddrinfo(address, port);
        if (e == 0) {
            bind(_socket, _long_addr->ai_addr, _long_addr->ai_addrlen);
        }
        else {
            throw std::runtime_error(make_error_message());
        }
    }

    void base_socket::listen_to() {
        //marks the socket referred to by _socket as a passive socket, that is,
        //as a socket that will be used to accept incoming connection requests using accept
        if (listen(_socket, MAX_BACKLOG) < 0) {
            //MAX_BACKLOG defines the maximum length to which the queue of pending connections for _socket may grow
            throw std::runtime_error(make_error_message());
        }
    }

    bool base_socket::is_listening() const {
        char val;
        int len = sizeof(val);
        if (getsockopt(_socket,
            SOL_SOCKET, //get options at the sockets API level
            SO_ACCEPTCONN, //can it accepts connections i.e. passive listening
            &val, &len) == SOCKET_ERROR)
        {
            throw std::runtime_error(make_error_message());
        }
        return val;
    }

    sockfd_t base_socket::accept_from() {
        assert(is_listening());
        socklen_t len_raddr = sizeof(_raddr);
        auto s = accept(_socket, //this bound and listening socket's file descriptor
            reinterpret_cast<struct sockaddr*>(&_raddr), //filled in with the remote address of this peer socket
            &len_raddr);
        if (s == INVALID_SOCKET) {
            throw std::runtime_error(make_error_message());
        }
        return s; //the newly created socket using the connected file descriptor
    }

    void base_socket::connect_to(address_t& address, port_t port) {
        if (connect(_socket, _long_addr->ai_addr, _long_addr->ai_addrlen) == SOCKET_ERROR) {
            throw std::runtime_error(make_error_message());
        }
    }

    void base_socket::be_non_blocking() { 
        //FNBIO enables or disables the blocking mode for the socket based on the value of mode.
        // 0 = blocking is enabled 
        // 1 = non-blocking mode is enabled.
        u_long mode = 1; 
        if (ioctlsocket(_socket, FIONBIO, &mode) == SOCKET_ERROR) {
            throw std::runtime_error(make_error_message());
        }
    }

    const size_t base_socket::peek() const {
        std::array<char, DEFAULT_BUFFER_SIZE>buffer;
        auto i = recv(_socket, &buffer.front(), buffer.size(), MSG_PEEK);
        if (i == SOCKET_ERROR) {
            throw std::runtime_error(make_error_message());
        }
        return i;
    }

    std::string base_socket::read(flag_t flags) const {
        std::array<char, DEFAULT_BUFFER_SIZE>buffer;
        auto i = recv(_socket, &buffer.front(), buffer.size(), flags);
        if (i == SOCKET_ERROR) {
            throw std::runtime_error(make_error_message());
        }
        buffer[static_cast<size_t>(i)] = '\0';
        return std::string(buffer.data());
    }

    long base_socket::write(address_t& buffer, flag_t flags) const {
        auto i = send(_socket, buffer.c_str(), static_cast<int>(buffer.size()), flags);
        if (i == SOCKET_ERROR) {
            throw std::runtime_error(make_error_message());
        }
        return i;
    }

    std::string base_socket::read_from(flag_t flags) {
        std::array<char, DEFAULT_BUFFER_SIZE> buffer;
        int len_raddr = sizeof(_raddr);
        auto i = recvfrom(_socket,
            &buffer.front(),
            buffer.size(),
            flags,
            reinterpret_cast<struct sockaddr*>(&_raddr),
            &len_raddr);
        if (i == SOCKET_ERROR) { //return the number of bytes received, or -1 if an error occurred.
            throw std::runtime_error(make_error_message());
        }
        buffer[static_cast<size_t>(i)] = '\0';
        return std::string(buffer.data());
    }

    long base_socket::write_back(const std::string& buffer, flag_t flags) {
        //transmit message in buffer
        auto i = sendto(_socket,
            buffer.c_str(),
            static_cast<int>(buffer.size()),
            flags,
            reinterpret_cast<struct sockaddr*>(&_raddr),
            sizeof(_raddr));
        if (i == SOCKET_ERROR) { //return the number of bytes sent, or -1 if an error occurred.
            throw std::runtime_error(make_error_message());
        }
        return i;
    }

    std::string base_socket::hostname() const
    {
        return std::string();
    }

    void base_socket::reset() {
        char optval = 1;            //option data depends on command here 1 enables reuse
        auto optlen = sizeof(char); //length of the option data here a single byte field
        if (setsockopt(_socket,
            SOL_SOCKET,       //manipulates options at the sockets API level
            SO_REUSEADDR,     //Enables fast restart by telling kernel to reuse even if busy
            &optval,
            static_cast<int>(optlen)) == INVALID_SOCKET)
        {
            throw std::runtime_error(make_error_message());
        }
    }

    void base_socket::stop(action_t action) {
        switch (action) {
        case action_t::WRITE:
            if (shutdown(_socket, SD_SEND) < 0) {
                throw std::runtime_error(make_error_message());
            }
            break;
        case action_t::READ:
            if (shutdown(_socket, SD_RECEIVE) < 0) {
                throw std::runtime_error(make_error_message());
            }
            break;
        case action_t::READ_AND_WRITE:
            if (shutdown(_socket, SD_BOTH) < 0) {
                throw std::runtime_error(make_error_message());
            }
            break;
        }
    }

    int base_socket::_getaddrinfo(const std::string& address, const unsigned short port) {
        // get server ip fit args
        return getaddrinfo(address.c_str(), std::to_string(port).c_str(), &_hints, &_long_addr);  
    }

}   /*! @} */

#endif