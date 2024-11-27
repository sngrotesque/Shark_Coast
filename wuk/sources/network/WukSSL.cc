#include <network/WukSSL.hh>

void wukSSL_exception(const char *funcName)
{
    wU32 err_code = ERR_get_error();
    char err_msg[256]{};
    ERR_error_string(err_code, err_msg);
    throw wuk::Exception(static_cast<wuk::Error>(err_code), funcName, err_msg);
}

wuk::net::SSL_Socket::SSL_Socket(SSL *_ssl, wuk::net::Socket _fd)
: ssl(_ssl), fd(_fd), transmissionLength()
{

}

void wuk::net::SSL_Socket::connect(const std::string addr, const wU16 port)
{
    this->fd.connect(addr, port);
    if(SSL_connect(this->ssl) != 1) {
        wukSSL_exception("wuk::net::SSL_Socket::connect");
    }
}

void wuk::net::SSL_Socket::send(const std::string content)
{
    const char *buffer = content.c_str();
    wSize length = content.size();

    this->transmissionLength = SSL_write(this->ssl, buffer, length);
    if(this->transmissionLength <= 0) {
        wukSSL_exception("wuk::net::SSL_Socket::send");
    }
}

std::string wuk::net::SSL_Socket::recv(const wS32 length)
{
    char *buffer = wuk::m_alloc<char *>(length);
    if(!buffer) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::net::SSL_Socket::recv",
            "Failed to allocate memory for buffer.");
    }

    this->transmissionLength = SSL_read(this->ssl, buffer, length);
    if(this->transmissionLength <= 0) {
        wukSSL_exception("wuk::net::SSL_Socket::recv");
    }

    std::string result{buffer, (wSize)this->transmissionLength};
    wuk::m_free(buffer);
    return result;
}

///////////////////////////////////////////////////////////////////////////////
wuk::net::SSL_Context::SSL_Context(const SSL_METHOD *method)
: ssl_ctx(nullptr), ssl(nullptr)
{
    if(!method) {
        method = TLS_method();
    }
    if(!(this->ssl_ctx = SSL_CTX_new(method))) {
        wukSSL_exception("wuk::net::SSL_Context::SSL_Context");
    }
    const wByte *ssl_sid = reinterpret_cast<const wByte*>("wukSSL_Context");
    wU32 ssl_sid_len = static_cast<wU32>(strlen(
                        reinterpret_cast<const char *>(ssl_sid)));

    // 指定SSL选项
    wSize options = (SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
#   ifdef WUK_SSL_STRICT
    options |= (SSL_OP_NO_TLSv1                 | SSL_OP_NO_TLSv1_1);
    options |= (SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_NO_RENEGOTIATION);
#   endif
#   ifdef SSL_OP_NO_COMPRESSION
    options |= SSL_OP_NO_COMPRESSION;
#   endif
#   ifdef SSL_OP_CIPHER_SERVER_PREFERENCE
    options |= SSL_OP_CIPHER_SERVER_PREFERENCE;
#   endif
#   ifdef SSL_OP_SINGLE_DH_USE
    options |= SSL_OP_SINGLE_DH_USE;
#   endif
#   ifdef SSL_OP_SINGLE_ECDH_USE
    options |= SSL_OP_SINGLE_ECDH_USE;
#   endif
    SSL_CTX_set_options(this->ssl_ctx, options);

    // 设置密码级别
    SSL_CTX_set_cipher_list(this->ssl_ctx, "HIGH:!aNULL:!eNULL");
    // 设置SSL协议最低版本
    SSL_CTX_set_min_proto_version(this->ssl_ctx, TLS1_2_VERSION);
    // 设置SSL协议模式
    SSL_CTX_set_mode(this->ssl_ctx, SSL_MODE_RELEASE_BUFFERS);
    // 设置SSL协议会话上下文ID
    SSL_CTX_set_session_id_context(this->ssl_ctx, ssl_sid, ssl_sid_len);

    // 根据CTX新建一个SSL对象
    if(!(this->ssl = SSL_new(this->ssl_ctx))) {
        SSL_CTX_free(this->ssl_ctx);
        wukSSL_exception("wuk::net::SSL_Context::SSL_Context");
    }
}

wuk::net::SSL_Context::~SSL_Context()
{
}

wuk::net::SSL_Socket wuk::net::SSL_Context::wrap_socket(wuk::net::Socket fd,
                                                        std::string server_hostname)
{
    if(SSL_set_fd(this->ssl, fd.fd) != 1) {
        wukSSL_exception("wuk::net::SSL_Context::wrap_socket");
    }
    if(SSL_set_tlsext_host_name(this->ssl, server_hostname.c_str()) != 1) {
        wukSSL_exception("wuk::net::SSL_Context::wrap_socket");
    }
    return wuk::net::SSL_Socket(this->ssl, fd);
}

void wuk::net::SSL_Context::destroy()
{
    SSL_free(this->ssl);
    SSL_CTX_free(this->ssl_ctx);
}
