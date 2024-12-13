#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <network/WukException.hh>
#include <WukBuffer.hh>
#include <WukMemory.hh>

#include <cmath>

#if defined(WUK_PLATFORM_LINUX)
#   include <netdb.h>
#   include <unistd.h>
#   include <sys/time.h>
#   include <arpa/inet.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <netinet/ip.h>
#   include <netinet/tcp.h>
#   define WUK_NET_ERROR EOF              // 定义错误代码
typedef struct sockaddr         SOCKADDR;
typedef struct addrinfo         ADDRINFO;
typedef struct sockaddr_in      SOCKADDR_IN;
typedef struct sockaddr_in6     SOCKADDR_IN6;
typedef struct sockaddr_storage SOCKADDR_STORAGE;
typedef wS32   wSocket;                   // wukNet的socket类型
#elif defined(WUK_PLATFORM_WINOS)
#   include <WS2tcpip.h>
#   if defined(_MSC_VER)
#       pragma comment(lib, "WS2_32")
#   endif
#   define WUK_NET_ERROR SOCKET_ERROR     // 定义错误代码
typedef SOCKET wSocket;                   // wukNet的socket类型
#endif

namespace wuk {
    namespace net {
        enum class SD_SW {
#           if defined(WUK_PLATFORM_LINUX)
            RECV = SHUT_RD,
            SEND = SHUT_WR,
            BOTH = SHUT_RDWR
#           elif defined(WUK_PLATFORM_WINOS)
            RECV = SD_RECEIVE,
            SEND = SD_SEND,
            BOTH = SD_BOTH
#           endif
        };

        class LIBWUK_API SockOpt {
        public:
            char *val;
            socklen_t val_len;

            SockOpt();
            SockOpt(char *val, socklen_t val_len);
            SockOpt(socklen_t val);
            SockOpt(std::string val);
        };
    }
}

namespace wuk {
    namespace net {
        class LIBWUK_API IPEndPoint {
        private:
            SOCKADDR *ai_addr;
            socklen_t ai_addrlen;

            std::string _to_string_addr(wI32 family, const void *pAddr) const;

            std::string to_string_addr(const SOCKADDR_IN *ipv4_addr) const;
            std::string to_string_addr(const SOCKADDR_IN6 *ipv6_addr) const;
            std::string to_string_addr(const ADDRINFO *addr) const;
            std::string to_string_addr(const SOCKADDR *addr, wI32 family) const;

        public:
            IPEndPoint();
            IPEndPoint(SOCKADDR *addr, socklen_t addrlen);
            IPEndPoint(std::string addr, wU16 port);
            ~IPEndPoint();

            const SOCKADDR *get_ai_addr() const;
            socklen_t get_ai_addrlen() const;
            std::string get_host() const;
            wU16 get_port() const;
        };
    }
}

namespace wuk {
    namespace net {
        class LIBWUK_API Socket {
        private:
            double _timeout;
            wSocket _fd;
            wI32 _family;
            wI32 _type;
            wI32 _proto;
            wI32 _t_size;

            IPEndPoint laddr; // local addr
            IPEndPoint raddr; // remote addr

        public:
            Socket();
            Socket(wI32 family, wI32 type, wI32 proto = 0, wSocket other_fd = WUK_NET_ERROR);
            ~Socket();

        public:
            ADDRINFO *get_addr_info(std::string addr, std::string service_name) const;

            void setsockopt(wI32 level, wI32 opt_name, wuk::net::SockOpt opt);
            void getsockopt(wI32 level, wI32 opt_name, wuk::net::SockOpt &opt);

            IPEndPoint getsockname(wSocket fd);
            IPEndPoint getsockname();

            void settimeout(double timeout_val);

            void connect(const std::string addr, const wU16 port);
            void bind(const std::string addr, const wU16 port);
            void listen(const wI32 backlog);
            wuk::net::Socket accept();

            void send(const wuk::Buffer buffer, const wI32 flag = 0);
            void sendall(const wuk::Buffer buffer, const wI32 flag = 0);
            void sendto(const wuk::Buffer buffer, wuk::net::IPEndPoint &target, const wI32 flag = 0);

            wuk::Buffer recv(const wI32 length, const wI32 flag = 0);
            wuk::Buffer recvfrom(const wI32 length, wuk::net::IPEndPoint &from, const wI32 flag = 0);

            void shutdown(const wI32 how);
            void shutdown(wuk::net::SD_SW how);
            void close();

        public:
            wSocket get_socket() const;
            wI32 get_transmission_length() const;
        };
    }
}

#endif
