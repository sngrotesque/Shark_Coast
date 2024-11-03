#include <config/WukException.hh>

/*
* // 如果wuk::Exception::Exception中to_string出错，那么取消注释。
* #include <sstream>
* namespace std {
*     template <typename T>
*     std::string std::to_string(T value) {
*         std::ostringstream os;
*         os << value;
*         return os.str();
*     }
* };
*/

std::string get_message(wuk::Error code, const char *f, const char *m)
{
#   ifdef WUK_STD_CPP_20
    return std::format("{0}[{1}]: {2}", f, code, m);
#   else
    return std::string{f} + "[" + std::to_string(code) + "]: " + m;
#   endif
}

wuk::Exception::Exception(wuk::Error code, const char *function, const char *message)
{
    this->output_message = get_message(code, function, message);
}

const char *wuk::Exception::what() const noexcept
{
    return this->output_message.c_str();
}
