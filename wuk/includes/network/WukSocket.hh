#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <network/WukNetwork.hh>
#include <network/WukIPEndPoint.hh>
#include <network/WukSocketOptions.hh>
#include <WukBuffer.hh>

namespace wuk {
    namespace net {
        class LIBWUK_API Socket {
        private:
            double _timeout; // 超时时间
            wI32 _p_size;    // 单次传输的包的长度

        private:
            IPEndPoint client_info;
            IPEndPoint listen_info;

        private:
            wSocket _fd;

            wI32 _family;
            wI32 _type;
            wI32 _proto;

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
            wSocket get_fileno() const;
            wI32 get_transmission_length() const;
        };
    }
}

#endif
