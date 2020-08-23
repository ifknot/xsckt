#include "socket_factory.h"

/**
 * \addtogroup xsckt
 * @{
 */
namespace xsckt {

    //------------udp_server_socket implementation------------
    udp_server_socket::multi_socket(const std::string addr, const unsigned short port) :
        base_socket(AF_INET, SOCK_DGRAM, 0) {
        bind_to(addr, port);
    }

    std::string udp_server_socket::hostname() const {
        return base_socket::hostname();
    }

    std::string udp_server_socket::read_from(const int flags) {
        return  base_socket::read_from(flags);
    }

    long udp_server_socket::write_back(const std::string& buffer, const int flags) {
        return base_socket::write_back(buffer, flags);
    }

    //------------udp_client_socket implementation------------
    udp_client_socket::multi_socket(const std::string addr, const unsigned short port) :
        base_socket(AF_INET, SOCK_DGRAM, 0) {
        connect_to(addr, port);
    }

    std::string udp_client_socket::hostname() const {
        return base_socket::hostname();
    }

    std::string udp_client_socket::read(const int flags) const {
        return base_socket::read(flags);
    }

    long udp_client_socket::write(const std::string& buffer, const int flags) const {
        return  base_socket::write(buffer, flags);
    }

    //------------tcp_active_socket implementation------------
    tcp_active_socket::multi_socket(unsigned int socket, blocking_t sync) :
        base_socket(socket) 
    {
        if (sync == blocking_t::NONBLOCKING) {
           base_socket::be_non_blocking();
        }
    }

    std::string tcp_active_socket::hostname() const {
        return base_socket::hostname();
    }

    const size_t tcp_active_socket::peek() const {
        return base_socket::peek();
    }

    std::string tcp_active_socket::read(const int flags) const {
        return base_socket::read(flags);
    }

    long tcp_active_socket::write(const std::string& buffer, const int flags) const {
        return  base_socket::write(buffer, flags);
    }

    //------------tcp_server_socket implementation------------
    tcp_server_socket::multi_socket(const std::string addr, const unsigned short port, blocking_t sync) :
        base_socket(AF_INET, SOCK_STREAM, 0) 
    {
        if (sync == blocking_t::NONBLOCKING) {
            base_socket::be_non_blocking();
        }
        bind_to(addr, port);
        listen_to();
    }

    std::string tcp_server_socket::hostname() const {
        return base_socket::hostname();
    }

    sockfd_t tcp_server_socket::accept_from() {
        return base_socket::accept_from();
    }
  

    void tcp_server_socket::stop(action_t action) {
        base_socket::stop(action);
    }

    //------------tcp_client_socket implementation------------
    tcp_client_socket::multi_socket(const std::string addr, const unsigned short port) :
        base_socket(AF_INET, SOCK_STREAM, 0) {
        connect_to(addr, port);
    }

    std::string tcp_client_socket::hostname() const {
        return base_socket::hostname();
    }

    std::string tcp_client_socket::read(const int flags) const {
        return base_socket::read(flags);
    }

    long tcp_client_socket::write(const std::string& buffer, const int flags) const {
        return  base_socket::write(buffer, flags);
    }

}   /*! @} */