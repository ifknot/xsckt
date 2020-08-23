#pragma once

/**
 * \addtogroup xsckt
 * @{
 */
namespace xsckt {

    //multi_socket template behaviour selectors
    enum class protocol_t { ANY, TCP, UDP, ICMP, IGMP, RFCOMM, ICMPv6, PGM };
    enum class role_t { client, server, active };
    enum class family_t { IPv4, IPv6, IrDA, Bluetooth };
    enum class socket_t { STREAM, DGRAM, RAW, RDM };
    enum class blocking_t { BLOCKING, NONBLOCKING};

    //stop actions
    enum class action_t { READ, WRITE, READ_AND_WRITE };

    //socket constants
    static const std::string LOOPBACK_ADDR = { "127.0.0.1" };
    static const int DEFAULT_PORT = 5555;
    static const int DEFAULT_BUFFER_SIZE = 512;
    static const int BLUETOOTH_BACKLOG = 4;

}   /*! @} */
