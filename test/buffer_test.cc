#include <WukBuffer.hh>
#include <WukMisc.hh>

#include <WukBuffer.cc>
#include <WukMisc.cc>

#include <iostream>
using namespace std;

void print_info(wuk::Buffer buffer)
{
    printf("%s[I]%s data:   %p\n",
            wuk::color::fore::cyan,
            wuk::color::all::reset,
            buffer.get_data());

    printf("%s[I]%s length: %zd\n",
            wuk::color::fore::cyan,
            wuk::color::all::reset,
            buffer.get_length());
    printf("%s[I]%s sizeof: %zd\n",
            wuk::color::fore::cyan,
            wuk::color::all::reset,
            buffer.get_size());
}

void method_1()
{
    wuk::Buffer buffer{312};
    // wuk::Buffer buffer = string{"this is buffer test for std::string1.\n"};
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
}

void method_2()
{
    wuk::Buffer buffer{9};
    buffer.append("abc123456.");

    print_info(buffer);

    buffer = buffer + wuk::Buffer{"'operator+() test.'"};

    print_info(buffer);

    buffer.append("done, test.");

    print_info(buffer);

    buffer += wuk::Buffer("operator+=() test.");

    print_info(buffer);

    cout << buffer.get_cStr() << endl;
}

int main()
{
    try {
        method_2();
    } catch (wuk::Exception &e) {
        cout << e.what() << endl;
    }

    return 0;
}
