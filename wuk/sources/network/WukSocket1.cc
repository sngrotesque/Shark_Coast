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
: ai_addr(nullptr), ai_addrlen(sizeof(SOCKADDR))
{
    this->ai_addr = wuk::m_alloc<SOCKADDR *>(this->ai_addrlen);
}

wuk::net::IPEndPoint::IPEndPoint(SOCKADDR *addr, socklen_t addrlen)
: ai_addr(nullptr), ai_addrlen(addrlen)
{
    this->ai_addr = wuk::m_alloc<SOCKADDR *>(addrlen);

    if (addr) {
        memcpy(this->ai_addr, addr, addrlen);
    }
}

wuk::net::IPEndPoint::IPEndPoint(std::string addr, wU16 port)
: ai_addr(nullptr), ai_addrlen()
{
    if (addr.find(':') != std::string::npos) {
        SOCKADDR_IN6 addrv6{};
        addrv6.sin6_family = AF_INET6;
        addrv6.sin6_port = htons(port);
        if (inet_pton(AF_INET6, addr.c_str(), &addrv6.sin6_addr) != 1) {
            wuk::net::exception("wuk::net::IPEndPoint::IPEndPoint");
        }
        this->ai_addr = wuk::m_alloc<SOCKADDR *>(sizeof(addrv6));
        this->ai_addrlen = sizeof(addrv6);
        memcpy(this->ai_addr, &addrv6, sizeof(addrv6));
    } else {
        SOCKADDR_IN addrv4{};
        addrv4.sin_family = AF_INET;
        addrv4.sin_port = htons(port);
        if (inet_pton(AF_INET, addr.c_str(), &addrv4.sin_addr) != 1) {
            wuk::net::exception("wuk::net::IPEndPoint::IPEndPoint");
        }
        this->ai_addr = wuk::m_alloc<SOCKADDR *>(sizeof(addrv4));
        this->ai_addrlen = sizeof(addrv4);
        memcpy(this->ai_addr, &addrv4, sizeof(addrv4));
    }
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

const SOCKADDR *wuk::net::IPEndPoint::get_ai_addr() const
{
    return this->ai_addr;
}

socklen_t wuk::net::IPEndPoint::get_ai_addrlen() const
{
    return this->ai_addrlen;
}

std::string wuk::net::IPEndPoint::get_host() const
{
    wI32 family_val{};

    if (this->ai_addrlen == sizeof(SOCKADDR_IN)) {
        family_val = AF_INET;
    } else if (this->ai_addrlen == sizeof(SOCKADDR_IN6)) {
        family_val = AF_INET6;
    } else {
        wuk::net::exception("wuk::net::IPEndPoint::get_host unknown protocol.");
    }

    return this->to_string_addr(this->ai_addr, family_val);
}

wU16 wuk::net::IPEndPoint::get_port() const
{
    if (this->ai_addrlen == sizeof(SOCKADDR_IN)) {
        SOCKADDR_IN *ipv4 = reinterpret_cast<SOCKADDR_IN *>(this->ai_addr);
        return ntohs(ipv4->sin_port);
    } else if (this->ai_addrlen == sizeof(SOCKADDR_IN6)) {
        SOCKADDR_IN6 *ipv6 = reinterpret_cast<SOCKADDR_IN6 *>(this->ai_addr);
        return ntohs(ipv6->sin6_port);
    } else {
        wuk::net::exception("wuk::net::IPEndPoint::get_host unknown protocol.");
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

wuk::net::Socket::Socket()
: _timeout(), _fd(), _family(), _type(), _proto(),
_t_size(), laddr(), raddr()
{

}

wuk::net::Socket::Socket(wI32 family, wI32 type, wI32 proto, wSocket other_fd)
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
    if (err == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::getsockopt");
    }
}

wuk::net::IPEndPoint wuk::net::Socket::getsockname(wSocket fd)
{
    SOCKADDR_STORAGE addr{};
    socklen_t addrlen{};

    SOCKADDR *addr_ptr = reinterpret_cast<SOCKADDR *>(&addr);

    wI32 err = ::getsockname(fd, addr_ptr, &addrlen);
    if (err == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::getsockname");
    }

    return wuk::net::IPEndPoint{addr_ptr, addrlen};
}

wuk::net::IPEndPoint wuk::net::Socket::getsockname()
{
    return this->getsockname(this->_fd);
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
    // 此处应使用wuk::net::IPEndPoint
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
    this->_t_size = ::sendto(this->_fd, buffer.get_cStr(), buffer.get_length(), flag,
                            target.get_ai_addr(), target.get_ai_addrlen());
    if (this->_t_size == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::sendto");
    }
}

wuk::Buffer wuk::net::Socket::recv(const wI32 length, const wI32 flag)
{
    wuk::Buffer buffer{static_cast<wSize>(length)};

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
    // 未完成
    wuk::Buffer buffer{static_cast<wSize>(length)};

    SOCKADDR_STORAGE ai_addr{};
    socklen_t ai_addrlen{sizeof(ai_addr)};

    char *buffer_ptr = reinterpret_cast<char *>(buffer.append_write(length));
    SOCKADDR *ai_addr_ptr = reinterpret_cast<SOCKADDR *>(&ai_addr);

    this->_t_size = ::recvfrom(this->_fd, buffer_ptr, length, flag, ai_addr_ptr, &ai_addrlen);
    
    // from.set_sockaddr(ai_addr_ptr, ai_addrlen);

    if (this->_t_size == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::recvfrom");
    }

    return buffer;
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

wSocket wuk::net::Socket::get_socket() const
{
    return this->_fd;
}

wI32 wuk::net::Socket::get_transmission_length() const
{
    return this->_t_size;
}
