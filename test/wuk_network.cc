#include <config/WukConfig.hh>
#include <config/WukException.hh>
#include <network/WukException.cc>
#include <network/WukSocket1.cc>
#include <WukBuffer.cc>

#include <WukMemory.hh>

#include <iostream>
using namespace std;

void wuknet_server_test()
{
    wuk::net::Socket fd{AF_INET, SOCK_DGRAM, IPPROTO_UDP};
    wuk::net::IPEndPoint client;

    wuk::Buffer recvBuffer;
    wuk::Buffer sendBuffer{"test server message."};

    fd.setsockopt(SOL_SOCKET, SO_REUSEADDR, wuk::net::SockOpt{1});
    fd.bind("0.0.0.0", 49881);

    recvBuffer = fd.recvfrom(4096, client);

    cout << recvBuffer.get_cStr() << endl;

    fd.sendto(sendBuffer, client);

    fd.shutdown(wuk::net::SD_SW::BOTH);
    fd.close();
}

void wuknet_client_test()
{
    wuk::net::Socket fd{AF_INET, SOCK_DGRAM, IPPROTO_UDP};
    wuk::net::IPEndPoint pointer{"127.0.0.1", 49981};

    wuk::Buffer recvBuffer;
    wuk::Buffer sendBuffer{"test client message."};

    fd.sendto(sendBuffer, pointer);
}

// int main()
// {
// #   ifdef WUK_PLATFORM_WINOS
//     WSADATA ws;
//     if (WSAStartup(MAKEWORD(2,2), &ws)) {
//         throw runtime_error("WSAStartup error!" + to_string(WSAGetLastError()));
//     }
// #   endif

//     try {
//         wuknet_server_test();
//     } catch (wuk::Exception &e) {
//         cout << "Error! " << e.what() << endl;
//     }

// #   ifdef WUK_PLATFORM_WINOS
//     WSACleanup();
// #   endif
//     return 0;
// }

///////////////////////////////////////////////////////////////////////////

int main() {
    const char* ip_str_v4 = "192.168.1.1";
    const char* ip_str_v6 = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";

    try {
        cout << wuk::net::IPEndPoint{ip_str_v4, 0}.get_host() << endl;
        cout << wuk::net::IPEndPoint(ip_str_v6, 0).get_host() << endl;
    } catch (wuk::Exception &e) {
        cout << "Error! " << e.what() << endl;
    }

    return 0;
}


