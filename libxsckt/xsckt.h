#pragma once

#include <string>

/**
 * \addtogroup xsckt
 * @{
 */
namespace xsckt {

	/**
	 * @brief BSD-esque abstract socket interface class for cross platform socket objects 
	 */
	struct interface {

        //------------minimal BSD/POSIX socket semantics------------

        /**
         * @brief bind -  server side, associates a socket with an address.
         * When an xsckt is constructed, it is only given a protocol family, but not assigned an address.
         * This association must be performed before the socket can accept connections from other hosts.
         * It is normally necessary to assign a local address using bind before a SOCK_STREAM socket may receive connections.
         * @param address - text format Internet address
         * @param port - port number
         */
        virtual void bind(const std::string& address, const unsigned short port) = 0;

        /**
         * @brief listen() -  server side, prepares it for incoming connections, *after* a socket has been associated with an address.
         * However, this is only necessary for the stream-oriented (connection-oriented) data modes, i.e., for socket types (SOCK_STREAM, SOCK_SEQPACKET).
         * @return int newly created socket file descriptor
         */
        virtual void listen() = 0;

        /**
         * @brief accept -  server side, when listening for stream-oriented connections, it creates a new *active* socket for each connection and removes the connection from the listening queue.
         * @note datagram sockets do not require processing by accept() since the receiver may immediately respond to the request using the listening socket.
         * @return unsigned int newly created socket file descriptor
         */
        virtual unsigned int accept() = 0;

        /**
         * @brief connect -  client side, establishes a direct communication link to a specific remote host identified by its address and port number. 
         * In case of a TCP socket, it causes an attempt to establish a new TCP connection. 
         * @version 0.5
         * @note on failure throws an exception containing the system call error message.
         * @param address - the address to which datagrams are sent by default, and the only address from which datagrams are received.
         * @param port - port number
         */
        virtual void connect(const std::string& address, const unsigned short port) = 0;

        /**
         * @brief be_non_blocking - set this socket to non-blocking mode.
         * by default sockets are created in blocking mode
         *
         */
        virtual void be_non_blocking() = 0;

        /**
        * @brief peek - Peeks at the incoming data.
        * The data is copied into the buffer, but is not removed from the input queue.
        * The function subsequently returns the amount of data that can be read in a single call to the recv
        * (or recvfrom) function, which may not be the same as the total amount of data queued on the socket.
        */
        virtual const size_t peek() const = 0;

        /**
         * @brief read - read a message from this socket if connected
         * @param flags - formed by ORing one or more of: MSG_CMSG_CLOEXEC, MSG_DONTWAIT, MSG_ERRQUEUE, MSG_OOB, MSG_PEEK, MSG_TRUNC, MSG_WAITALL - defaults to none
         * @return string - the message
         */
        virtual std::string read(const int flags = 0) const = 0;

        /**
         * @brief write - write a message to this socket if connected
         * @param buffer - the message string to write
         * @param flags - formed by ORing one or more of: MSG_CONFIRM, MSG_DONTROUTE, MSG_DONTWAIT, MSG_EOR, MSG_MORE, MSG_NOSIGNAL, MSG_OOB - defaults to none.
         * @return long - the number of bytes written
         */
        virtual long write(const std::string& buffer, const int flags = 0) const = 0;

        /**
         * @brief read_from - receive data on a socket whether or not it is connection-oriented.
         * @param flags - formed by ORing one or more of: MSG_CMSG_CLOEXEC, MSG_DONTWAIT, MSG_ERRQUEUE, MSG_OOB, MSG_PEEK, MSG_TRUNC, MSG_WAITALL, MSG_EOR, MSG_TRUNC, MSG_CTRUNC, MSG_ERRQUEUE - defaults to none
         * @return string - the message
         */
        virtual std::string read_from(const int flags = 0) = 0;

        /**
         * @brief write_back - transmit a message to back the socket that has been read/read_from
         * @param buffer - the message string to write
         * @param flags - formed by ORing one or more of: MSG_CONFIRM, MSG_DONTROUTE, MSG_DONTWAIT, MSG_EOR, MSG_MORE, MSG_NOSIGNAL, MSG_OOB - defaults to none.
         * @return long - the number of bytes written
         */
        virtual long write_back(const std::string& buffer, const int flags = 0) = 0;

        /**
         * @brief hostname - returns the standard host name for the local computer
         * @return std::string local machine name
         */
        virtual std::string hostname() const = 0;

        /**
         * @brief reset - enable kernel reuse addresses and ports that may already be active/tied
         */
        virtual void reset() = 0;

        /**
         * @brief stop - causes all or part of a full-duplex connection on this socket to be shut down.
         * @param action - SHUT_RD, further receptions will be disallowed; SHUT_WR further transmissions will be disallowed; SHUT_RDWR, further receptions and transmissions will be disallowed.
         */
        virtual void stop(const int action) = 0;


        virtual ~interface() = default;

	};

}   /*! @} */