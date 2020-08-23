#pragma once

#ifdef WIN32

    #ifdef __MINGW32__

    #else

        #include "windows_winsock_socket.h"

    #endif // __MINGW32__

#elif __linux__

#include "linux_socket.h"

#elif __APPLE__

#include "darwin_socket.h"

#endif

/**
 * \addtogroup xsckt
 * @{
 */
namespace xsckt {

    //base factory template class
    template<protocol_t, role_t, family_t, socket_t>
    struct multi_socket {};

    //------------product definitions------------
    //udp sockets
    using udp_server_socket = multi_socket<protocol_t::UDP, role_t::server, family_t::IPv4, socket_t::DGRAM>;
    using udp_client_socket = multi_socket<protocol_t::UDP, role_t::client, family_t::IPv4, socket_t::DGRAM>;
    //tcp sockets
    using tcp_server_socket = multi_socket<protocol_t::TCP, role_t::server, family_t::IPv4, socket_t::STREAM>;
    using tcp_active_socket = multi_socket<protocol_t::TCP, role_t::active, family_t::IPv4, socket_t::STREAM>;
    using tcp_client_socket = multi_socket<protocol_t::TCP, role_t::client, family_t::IPv4, socket_t::STREAM>;

    //------------udp_server_socket template------------
    template<>
    struct multi_socket<protocol_t::UDP, role_t::server, family_t::IPv4, socket_t::DGRAM> :
        private base_socket {

        multi_socket(const std::string addr, const unsigned short port);

        std::string hostname() const final;

        std::string read_from(const int flags = 0) final;

        long write_back(const std::string& buffer, const int flags = 0) final;

        virtual ~multi_socket() override = default;

    };

    //------------udp_client_socket template------------
    template<>
    struct multi_socket<protocol_t::UDP, role_t::client, family_t::IPv4, socket_t::DGRAM> :
        private base_socket {

        multi_socket(const std::string addr, const unsigned short port);

        std::string hostname() const final;

        std::string read(const int flags = 0) const final;

        long write(const std::string& buffer, const int flags = 0) const final;

        virtual ~multi_socket() override = default;

    };

    //------------tcp_active_socket template------------
    template<>
    struct multi_socket<protocol_t::TCP, role_t::active, family_t::IPv4, socket_t::STREAM> :
        private base_socket {

        explicit multi_socket(unsigned int socket, blocking_t sync = blocking_t::BLOCKING);

        multi_socket(const multi_socket&) = default;

        multi_socket& operator= (const multi_socket&) = default;

        std::string hostname() const final;

        const size_t peek() const final;

        std::string read(const int flags = 0) const final;

        long write(const std::string& buffer, const int flags = 0) const final;

        virtual ~multi_socket() override = default;

    private:


    };

    //------------tcp_server_socket template------------
    template<>
    struct multi_socket<protocol_t::TCP, role_t::server, family_t::IPv4, socket_t::STREAM> :
        private base_socket {

        multi_socket(const std::string addr, const unsigned short port, blocking_t sync = blocking_t::BLOCKING);

        std::string hostname() const final;

        virtual sockfd_t accept_from() final;

        void stop(action_t action) final;

        virtual ~multi_socket() override = default;
    };

    //------------tcp_client_socket template------------
    template<>
    struct multi_socket<protocol_t::TCP, role_t::client, family_t::IPv4, socket_t::STREAM> :
        private base_socket {

        multi_socket(const std::string addr, const unsigned short port);

        std::string hostname() const final;

        std::string read(const int flags = 0) const final;

        long write(const std::string& buffer, const int flags = 0) const final;

        virtual ~multi_socket() override = default;

    };

}   /*! @} */