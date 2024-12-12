#include <config/WukConfig.hh>
#include <config/WukException.hh>
#include <WukMemory.hh>

#include <ws2tcpip.h>
#include <iostream>
using namespace std;

#include <network/WukSocket1.cc>

class Socket {
private:
    int s_family;
    int s_type;
    int s_proto;

    SOCKET fd;

    wuk::net::IPEndPoint local;
    wuk::net::IPEndPoint remote;

    Socket(int family, int socktype, int proto)
    : s_family(family), s_type(socktype), s_proto(proto)
    {
        this->fd = socket(this->s_family, this->s_type, this->s_proto);
        if (this->fd == SOCKET_ERROR) {
            throw wuk::Exception(GetLastError(), "Socket::Socket", "create socket error.");
        }
    }

    ADDRINFO *get_addrinfo(const char *server_name, const char *service_name)
    {
        ADDRINFO hints{}, *result{nullptr};

        hints.ai_family = this->s_family;
        hints.ai_socktype = this->s_type;
        hints.ai_protocol = this->s_proto;

        int err = getaddrinfo(server_name, service_name, &hints, &result);
        if (err) {
            throw wuk::Exception(err, "Socket::get_addrinfo", "getaddrinfo error.");
        }

        return result;
    }

    ADDRINFO *get_addrinfo(std::string server_host, wU16 server_port)
    {
        return this->get_addrinfo(server_host.c_str(), to_string(server_port).c_str());
    }

    void connect(std::string server_host, wU16 server_port)
    {
        ADDRINFO *address_resolution_result = this->get_addrinfo(server_host, server_port);

        int err = ::connect(this->fd, address_resolution_result->ai_addr,
                            address_resolution_result->ai_addrlen);
        if (err == SOCKET_ERROR) {
            throw wuk::Exception(err, "Socket::connect", "connect error.");
        }

        remote.addrinfo_set.set_sockaddr(address_resolution_result->ai_addr,
                                        address_resolution_result->ai_addrlen);
        // remote.addrinfo_set.ai_addr = remote.to_string_addr(address_resolution_result->ai_addr);


        free(address_resolution_result);
    }

};




int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif






#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
