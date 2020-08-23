#pragma once

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN

#include "xsckt.h"

/**
 * \addtogroup xsckt
 * @{
 */
namespace xsckt {

    /**
     * @brief The multipurpose base_socket class provides WINDOWS OS specific *both* client and server behaviour for (all) protocols.
     * @note Only TCP & UDP implemented so far
     * @implements socketable interface
     * @version 0.5
     * Although somewhat monolithic it encapsulates all of the OS specific networking logic in one place
     * which simplifies cross-platform development and makes for a very flexible base for a factory to use.
     */
    class base_socket : public bsd_interface {

        static const int MAX_BACKLOG = SOMAXCONN;
        //static constexpr int MAX_BACKLOG = SOMAXCONN_HINT(200);

    public:

        /**
         * @brief base_socket::base_socket - constructs an easily restartable socket from a socket file descriptor.
         * @param socket - sockfd_t socket file descriptor
         */
        explicit base_socket(const sockfd_t socket);

        /**
         * @brief base_socket
         * @param address_family
         * @param socket_type
         * @param protocol
         */
        base_socket(const short address_family, const int socket_type, const int protocol);

        base_socket(const base_socket&) = default;

        base_socket& operator= (const base_socket&) = default;

        virtual ~base_socket() override;

        /**
         * @brief bind -  server side, associates a socket with an address.
         * When an xsckt is constructed, it is only given a protocol family, but not assigned an address.
         * This association must be performed before the socket can accept connections from other hosts.
         * It is normally necessary to assign a local address using bind before a SOCK_STREAM socket may receive connections.
         * @param address - text format Internet address
         * @param port - port number
         */
        virtual void bind(address_t& address, port_t port) final;

        /**
         * @brief listen() -  server side, prepares it for incoming connections, *after* a socket has been associated with an address.
         * However, this is only necessary for the stream-oriented (connection-oriented) data modes, i.e., for socket types (SOCK_STREAM, SOCK_SEQPACKET).
         * @return int newly created socket file descriptor
         */
        virtual void listen() final;

        /**
         * @brief accept -  server side, when listening for stream-oriented connections, it creates a new *active* socket for each connection and removes the connection from the listening queue.
         * @note datagram sockets do not require processing by accept() since the receiver may immediately respond to the request using the listening socket.
         * @return unsigned int newly created socket file descriptor
         */
        virtual sockfd_t accept() final;

        /**
         * @brief connect -  client side, establishes a direct communication link to a specific remote host identified by its address and port number.
         * In case of a TCP socket, it causes an attempt to establish a new TCP connection.
         * @version 0.5
         * @note on failure throws an exception containing the system call error message.
         * @param address - the address to which datagrams are sent by default, and the only address from which datagrams are received.
         * @param port - port number
         */
        virtual void connect(address_t& address, port_t port) final;

        /**
         * @brief be_non_blocking - set this socket to non-blocking mode.
         * by default sockets are created in blocking mode
         *
         */
        virtual void be_non_blocking() final;

        /**
        * @brief peek - Peeks at the incoming data.
        * The data is copied into the buffer, but is not removed from the input queue.
        * The function subsequently returns the amount of data that can be read in a single call to the recv
        * (or recvfrom) function, which may not be the same as the total amount of data queued on the socket.
        */
        virtual const size_t peek() const final;

        /**
         * @brief read - read a message from this socket if connected
         * @param flags - formed by ORing one or more of: MSG_CMSG_CLOEXEC, MSG_DONTWAIT, MSG_ERRQUEUE, MSG_OOB, MSG_PEEK, MSG_TRUNC, MSG_WAITALL - defaults to none
         * @return string - the message
         */
        virtual std::string read(flag_t flags = 0) const final;

        /**
         * @brief write - write a message to this socket if connected
         * @param buffer - the message string to write
         * @param flags - formed by ORing one or more of: MSG_CONFIRM, MSG_DONTROUTE, MSG_DONTWAIT, MSG_EOR, MSG_MORE, MSG_NOSIGNAL, MSG_OOB - defaults to none.
         * @return long - the number of bytes written
         */
        virtual long write(address_t& buffer, flag_t flags = 0) const final;

        /**
         * @brief read_from - receive data on a socket whether or not it is connection-oriented.
         * @param flags - formed by ORing one or more of: MSG_CMSG_CLOEXEC, MSG_DONTWAIT, MSG_ERRQUEUE, MSG_OOB, MSG_PEEK, MSG_TRUNC, MSG_WAITALL, MSG_EOR, MSG_TRUNC, MSG_CTRUNC, MSG_ERRQUEUE - defaults to none
         * @return string - the message
         */
        virtual std::string read_from(flag_t flags = 0) final;

        /**
         * @brief write_back - transmit a message to back the socket that has been read/read_from
         * @param buffer - the message string to write
         * @param flags - formed by ORing one or more of: MSG_CONFIRM, MSG_DONTROUTE, MSG_DONTWAIT, MSG_EOR, MSG_MORE, MSG_NOSIGNAL, MSG_OOB - defaults to none.
         * @return long - the number of bytes written
         */
        virtual long write_back(const std::string& buffer, flag_t flags = 0) final;

        /**
         * @brief hostname - returns the standard host name for the local computer
         * @return std::string local machine name
         */
        virtual std::string hostname() const final;

        /**
         * @brief reset - enable kernel reuse addresses and ports that may already be active/tied
         */
        virtual void reset() final;

        /**
         * @brief stop - causes all or part of a full-duplex connection on this socket to be shut down.
         * @param action - SHUT_RD, further receptions will be disallowed; SHUT_WR further transmissions will be disallowed; SHUT_RDWR, further receptions and transmissions will be disallowed.
         */
        virtual void stop(flag_t action) final;

    private:

        /**
         * @brief _assign_address - system call helper converts an Internet address in its standard text format into its numeric binary form
         * @param address - text format Internet address
         * @param port - IP port
         * @return int - Internet address in its numeric binary form
         */
        int _assign_address(const std::string& address, const unsigned short port);

        /**
         * @brief  _make_error_message- helper function to build an error message string
         * @return system specific error message
         */
        std::string _make_error_message();

        /**
         * @brief _reset_socket - system call helper to enable fast restart by enabling kernel to reuse addresses and ports that may be active.
         * @param socket - socket file descriptor
         */
        static void _reset_socket(sockfd_t socket);

        /**
         * @brief _peek - Peeks at an incoming message.
         * The data is treated as unread and the next recv() or similar function shall still return this data.
         * @param socket - socket file descriptor
        */
        static const size_t _peek(sockfd_t socket);

        /**
         * @brief _read - static system call helper receives message from a connected socket, behaviour dictated by flag options.
         * If no messages are available at the socket, the receive calls wait for a message to arrive (unless the socket is nonblocking).
         * @note May be used to receive data on both connectionless and connection-oriented sockets.
         * @param socket - the *connected* socket file descriptor
         * @param flags - formed by ORing one or more of: MSG_CMSG_CLOEXEC, MSG_DONTWAIT, MSG_ERRQUEUE, MSG_OOB, MSG_PEEK, MSG_TRUNC, MSG_WAITALL
         * @return string - normally returns any data available, up to the requested amount, rather than waiting for receipt of the full amount requested.
         */
        static std::string _read(sockfd_t socket, const int flags);

        /**
         * @brief _write - static system call helper transmits a message to a socket, behaviour dictated by flag options.
         * @param socket - the *connected* sending socket file descriptor
         * @param buffer - the std:string to store the message
         * @param flags - formed by ORing one or more of: MSG_CONFIRM, MSG_DONTROUTE, MSG_DONTWAIT, MSG_EOR, MSG_MORE, MSG_NOSIGNAL, MSG_OOB
         * @return long - on success return the number of bytes sent, else -1
         */
        static long _write(sockfd_t socket, const std::string& buffer, const int flags);

        /**
         * @brief _read_from static system call helper receives message from specific address, behaviour dictated by flag options.
         * @param socket - the socket file descriptor
         * @param addr - target address
         * @param flags - action flags
         * @return string - normally returns any data available, up to the requested amount, rather than waiting for receipt of the full amount requested.
         */
        static std::string _read_from(sockfd_t socket, sockaddr_in& addr, const int flags);

        /**
         * @brief _write_back - static system call helper transmits a message back to socket address stored by either accept or read_from, behaviour dictated by flag options.
         * @param socket
         * @param addr
         * @param flags
         * @return long - on success return the number of bytes sent, else -1
         */
        static long _write_back(sockfd_t socket, const std::string& buffer, sockaddr_in& addr, const int flags);

        sockfd_t _socket;

        short _address_family{ 0 };
        int _socket_type{ 0 };
        int _protocol{ 0 };

        struct addrinfo* _long_addr;
        struct addrinfo _hints;
        struct sockaddr_in _raddr;

    };

}   /*! @} */

#endif