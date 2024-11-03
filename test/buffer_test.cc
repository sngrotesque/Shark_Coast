#include <WukBuffer.hh>
#include <WukMisc.hh>

#include <WukBuffer.cc>
#include <WukMisc.cc>

#include <iostream>
using namespace std;

int main()
{
    // wuk::Buffer buffer{32};
    wuk::Buffer buffer = string{"this is buffer test for std::string1.\n"};
    wuk::Buffer text{"this is first.\n"};

    buffer = text;

    buffer.append("hello, world.\n");
    memcpy(buffer.append_write(13), "SN-Grotesque\n", 13);
    memcpy(buffer.append_write(text.get_length()), text.get_data(), text.get_length());

    wuk::misc::print_pybytes(buffer.get_data(), buffer.get_length(), true);

    wByte *p = const_cast<wByte *>(buffer.get_data());
    p[0] = 0x00;

    printf("length: %zd\n", buffer.get_length());
    printf("sizeof: %zd\n", buffer.get_size());

    return 0;
}
