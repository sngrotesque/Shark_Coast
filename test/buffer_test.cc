#include <WukBuffer.hh>
#include <WukMisc.hh>

#include <WukBuffer.cc>
#include <WukMisc.cc>

#include <iostream>
using namespace std;

int main()
{
    wuk::Buffer buffer = string{"this is buffer test for std::string1.\n"};

    buffer.append("hello, world.\n");
    memcpy(buffer.append_write(13), "SN-Grotesque\n", 13);

    wuk::misc::print_pybytes(buffer.get_data(), buffer.get_length(), true);

    printf("length: %zd\n", buffer.get_length());
    printf("sizeof: %zd\n", buffer.get_size());

    return 0;
}
