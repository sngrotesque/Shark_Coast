#include <network/WukSocket1.hh>

wuk::net::SockOpt::SockOpt() : val(nullptr), val_len() {}

wuk::net::SockOpt::SockOpt(char *val, socklen_t val_len) : val(val), val_len(val_len) {}

wuk::net::SockOpt::SockOpt(socklen_t val)
{
    this->val = reinterpret_cast<char *>(&val);
    this->val_len = sizeof(val);
}

wuk::net::SockOpt::SockOpt(std::string val)
{
    this->val = const_cast<char *>(val.c_str());
    this->val_len = val.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////

std::string wuk::net::IPEndPoint::_to_string_addr(wI32 family, const void *pAddr) const
{
    char buffer_addr[INET6_ADDRSTRLEN]{};

    if (!inet_ntop(family, pAddr, buffer_addr, ARRAYSIZE(buffer_addr))) {
        wuk::net::exception("wuk::net::IPEndPoint::to_string_addr");
    }

    return std::string{buffer_addr};
}

wuk::net::IPEndPoint::IPEndPoint()
: ai_addr(nullptr), ai_addrlen(sizeof(SOCKADDR)), n_addr(), n_port()
{
    this->ai_addr = wuk::m_alloc<SOCKADDR *>(this->ai_addrlen);
}

wuk::net::IPEndPoint::IPEndPoint(SOCKADDR *addr, socklen_t addrlen)
: ai_addrlen(addrlen), n_addr(), n_port()
{
    this->ai_addr = wuk::m_alloc<SOCKADDR *>(addrlen);
    memcpy(this->ai_addr, addr, addrlen);
}

wuk::net::IPEndPoint::IPEndPoint(std::string addr, wU16 port)
: ai_addr(nullptr), ai_addrlen(), n_addr(addr), n_port(port)
{

}

wuk::net::IPEndPoint::~IPEndPoint()
{
    wuk::m_free(this->ai_addr);
}

std::string wuk::net::IPEndPoint::to_string_addr(const SOCKADDR_IN *ipv4_addr) const
{
    return this->_to_string_addr(AF_INET, &ipv4_addr->sin_addr);
}

std::string wuk::net::IPEndPoint::to_string_addr(const SOCKADDR_IN6 *ipv6_addr) const
{
    return this->_to_string_addr(AF_INET6, &ipv6_addr->sin6_addr);
}

std::string wuk::net::IPEndPoint::to_string_addr(const ADDRINFO *addr) const
{
    if (addr->ai_family == AF_INET) {
        const SOCKADDR_IN *ipv4_addr = reinterpret_cast<SOCKADDR_IN *>(addr->ai_addr);
        return this->to_string_addr(ipv4_addr);
    }
    const SOCKADDR_IN6 *ipv6_addr = reinterpret_cast<SOCKADDR_IN6 *>(addr->ai_addr);
    return this->to_string_addr(ipv6_addr);
}

std::string wuk::net::IPEndPoint::to_string_addr(const SOCKADDR *addr, wI32 family) const
{
    if (family == AF_INET) {
        const SOCKADDR_IN *ipv4_addr = reinterpret_cast<const SOCKADDR_IN *>(addr);
        return this->to_string_addr(ipv4_addr);
    }
    const SOCKADDR_IN6 *ipv6_addr = reinterpret_cast<const SOCKADDR_IN6 *>(addr);
    return this->to_string_addr(ipv6_addr);
}

void wuk::net::IPEndPoint::set_sockaddr(SOCKADDR *addr, socklen_t addrlen)
{
    if (this->ai_addrlen != addrlen) {
        wuk::m_free(this->ai_addr);
        this->ai_addr = wuk::m_alloc<SOCKADDR *>(addrlen);
        this->ai_addrlen = addrlen;
    }
    memcpy(this->ai_addr, addr, this->ai_addrlen);
}

void wuk::net::IPEndPoint::ser_host_port(std::string addr, wU16 port)
{
    this->n_addr = addr;
    this->n_port = port;
}

void wuk::net::IPEndPoint::set_host(std::string addr)
{
    this->n_addr = addr;
}

void wuk::net::IPEndPoint::set_port(wU16 port)
{
    this->n_port = port;
}

void wuk::net::IPEndPoint::get_network_info(wSocket fd, wI32 family)
{
    // 后续将getsockname实现在wuk::net::Socket内部并将当前类的所有内容移植到其中。

    SOCKADDR_STORAGE addr{};
    socklen_t addrlen{};

    wI32 err = getsockname(fd, reinterpret_cast<SOCKADDR *>(&addr), &addrlen);
    if (err == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::IPEndPoint::get_network_info");
    }
}

void wuk::net::IPEndPoint::get_network_info(SOCKADDR *addr, wI32 family)
{
    
}

const SOCKADDR *wuk::net::IPEndPoint::get_ai_addr() const
{
    return this->ai_addr;
}

socklen_t wuk::net::IPEndPoint::get_ai_addrlen() const
{
    return this->ai_addrlen;
}

std::string wuk::net::IPEndPoint::get_n_addr() const
{
    return this->n_addr;
}

wU16 wuk::net::IPEndPoint::get_n_port() const
{
    return this->n_port;
}

////////////////////////////////////////////////////////////////////////////////////////////////

wuk::net::Socket::Socket()
: _timeout(), _fd(), _family(), _type(), _proto(),
_t_size(), laddr(), raddr()
{

}

wuk::net::Socket::Socket(wI32 family, wI32 type, wI32 proto = 0, wSocket other_fd = WUK_NET_ERROR)
: _timeout(), _fd(), _family(family), _type(type), _proto(proto),
_t_size(), laddr(), raddr()
{
    if (static_cast<wI32>(other_fd) == WUK_NET_ERROR) {
        this->_fd = socket(this->_family, this->_type, this->_proto);
        if (static_cast<wI32>(this->_fd) == WUK_NET_ERROR) {
            wuk::net::exception("wuk::net::Socket::Socket");
        }
    } else {
        this->_fd = other_fd;
    }
}

wuk::net::Socket::~Socket()
{

}

ADDRINFO *wuk::net::Socket::get_addr_info(std::string addr, std::string service_name) const
{
    ADDRINFO *result{nullptr};
    ADDRINFO hints{};

    hints.ai_family = this->_family;
    hints.ai_socktype = this->_type;
    hints.ai_protocol = this->_proto;

    wI32 wsaError = getaddrinfo(addr.c_str(), service_name.c_str(),
                                &hints, &result);
    if (wsaError) {
        wuk::net::exception("wuk::net::IPEndPoint::get_addr_info");
    }

    return result;
}

void wuk::net::Socket::setsockopt(wI32 level, wI32 opt_name, wuk::net::SockOpt opt)
{
    wI32 err = ::setsockopt(this->_fd, level, opt_name,
                            static_cast<const char *>(opt.val), opt.val_len);
    if (err == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::setsockopt");
    }
}

void wuk::net::Socket::getsockopt(wI32 level, wI32 opt_name, wuk::net::SockOpt &opt)
{
    wI32 err = ::getsockopt(this->_fd, level, opt_name, opt.val, &opt.val_len);
    if (err = WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::getsockopt");
    }
}

wuk::net::IPEndPoint wuk::net::Socket::getsockname(wSocket fd)
{
    

    wI32 err = ::getsockname(fd, nullptr, nullptr);
}

void wuk::net::Socket::settimeout(double timeout_val)
{
    this->_timeout = timeout_val;

#               if defined(WUK_PLATFORM_WINOS)
    DWORD _time_val = static_cast<DWORD>(this->_timeout * 1e3);
#               elif defined(WUK_PLATFORM_LINUX)
    double int_part{};
    double frac_part{modf(this->_timeout, &int_part)};
    struct timeval _time_val{
        static_cast<time_t>(int_part), static_cast<time_t>(frac_part * 1e6)
    };
#               endif
    wuk::net::SockOpt opt{reinterpret_cast<char *>(&_time_val), sizeof(_time_val)};

    this->setsockopt(SOL_SOCKET, SO_SNDTIMEO, opt);
    this->setsockopt(SOL_SOCKET, SO_RCVTIMEO, opt);
}

void wuk::net::Socket::connect(const std::string addr, const wU16 port)
{
    ADDRINFO *result = this->get_addr_info(addr, std::to_string(port));
    
    wI32 err = ::connect(this->_fd, result->ai_addr, result->ai_addrlen);
    if (err == WUK_NET_ERROR) {
        freeaddrinfo(result);
        wuk::net::exception("wuk::net::Socket::connect");
    }

    // 未实现解析远程地址和当前地址，raddr，laddr。

    freeaddrinfo(result);
}

void wuk::net::Socket::bind(const std::string addr, const wU16 port)
{
    ADDRINFO *result = this->get_addr_info(addr, std::to_string(port));

    wI32 err = ::bind(this->_fd, result->ai_addr, result->ai_addrlen);
    if (err == WUK_NET_ERROR) {
        freeaddrinfo(result);
        wuk::net::exception("wuk::net::Socket::bind");
    }

    // 未实现解析远程地址和当前地址，raddr，laddr。

    freeaddrinfo(result);
}

void wuk::net::Socket::listen(const wI32 backlog)
{
    if (::listen(this->_fd, backlog)) {
        wuk::net::exception("wuk::net::Socket::listen");
    }
}

wuk::net::Socket wuk::net::Socket::accept()
{
    SOCKADDR addr{};
    socklen_t addrlen{};

    wSocket other_fd = ::accept(this->_fd, &addr, &addrlen);

    if (static_cast<wI32>(other_fd) == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::accept");
    }

    return wuk::net::Socket(this->_family, this->_type, this->_proto, other_fd);
}

void wuk::net::Socket::send(const wuk::Buffer buffer, const wI32 flag)
{
    this->_t_size = ::send(this->_fd, buffer.get_cStr(), buffer.get_length(), flag);

    if (this->_t_size == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::send");
    }
}

void wuk::net::Socket::sendall(const wuk::Buffer buffer, const wI32 flag)
{
    wU32 retry_count{5};

    const char *ptr = buffer.get_cStr();
    wU32 size = static_cast<wU32>(buffer.get_length());

    while (size) {
        this->_t_size = ::send(this->_fd, ptr, size, flag);

        if (this->_t_size == WUK_NET_ERROR) {
            if (retry_count) {
                retry_count--;
                continue;
            } else {
                wuk::net::exception("wuk::net::Socket::sendall");
            }
        }

        ptr  += this->_t_size;
        size -= this->_t_size;
    }
}

void wuk::net::Socket::sendto(const wuk::Buffer buffer, wuk::net::IPEndPoint &target, const wI32 flag)
{
    // this->_t_size = ::sendto(this->_fd, buffer.get_cStr(), buffer.get_length(), flag,
    //                         &target.ai_addr, target.ai_addrlen);
    
    // if (this->_t_size == WUK_NET_ERROR) {
    //     wuk::net::exception("wuk::net::Socket::send");
    // }
}

wuk::Buffer wuk::net::Socket::recv(const wI32 length, const wI32 flag)
{
    wuk::Buffer buffer{length};

    this->_t_size = ::recv(this->_fd,
                        reinterpret_cast<char *>(buffer.append_write(length)),
                        length, flag);
    
    if (this->_t_size == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::recv");
    }

    return buffer;
}

wuk::Buffer wuk::net::Socket::recvfrom(const wI32 length, wuk::net::IPEndPoint &from, const wI32 flag)
{
    // wuk::Buffer buffer{length};

    // this->_t_size = ::recvfrom(this->_fd,
    //                         reinterpret_cast<char *>(buffer.append_write(length)),
    //                         length, flag, &from.ai_addr, &from.ai_addrlen);
    
    // if (this->_t_size == WUK_NET_ERROR) {
    //     wuk::net::exception("wuk::net::Socket::recvfrom");
    // }
}

void wuk::net::Socket::shutdown(const wI32 how)
{
    if (::shutdown(this->_fd, how) == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::shutdown");
    }
}

void wuk::net::Socket::shutdown(wuk::net::SD_SW how)
{
    this->shutdown(static_cast<wI32>(how));
}

void wuk::net::Socket::close()
{
#               if defined(WUK_PLATFORM_WINOS)
    if (::closesocket(this->_fd) == WUK_NET_ERROR)
#               elif defined(WUK_PLATFORM_LINUX)
    if (::close(this->_fd) == WUK_NET_ERROR)
#               endif
    {
        wuk::net::exception("wuk::net::Socket::close");
    }
}
