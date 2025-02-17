#include <WukBase64.hh>

#define BASE64PAD '='

constexpr char b64en_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
constexpr wByte b64de_table[256] = {
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255, 62, 255,255,255, 63,
    52 , 53, 54, 55,  56, 57, 58, 59,  60, 61,255,255, 255,  0,255,255,

    255,  0,  1,  2,   3,  4,  5,  6,   7,  8,  9, 10,  11, 12, 13, 14,
    15 , 16, 17, 18,  19, 20, 21, 22,  23, 24, 25,255, 255,255,255,255,
    255, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, 40,
    41 , 42, 43, 44,  45, 46, 47, 48,  49, 50, 51,255, 255,255,255,255,

    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,

    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255};

// Encoding, definition
wSize wuk::Base64::get_encode_length(wSize length)
{
    return (length % 3) ? (length / 3 + 1) * 4 : (length / 3 * 4);
}

// Decoding, definition
wSize wuk::Base64::get_decode_length(wSize length)
{
    // 此处+3的目的是为了不让缓冲区过小
    return (length + 3) / 4 * 3;
}

wuk::Base64::Base64(bool strict_mode)
: strict_mode(strict_mode)
{

}

char *wuk::Base64::encode(const wByte *buffer, wSize &length)
{
    if(!buffer || !length) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::Base64::encode",
            "buffer is NULL.");
    }
    wSize src_index{0}, dst_index{0};
    wSize result_length = this->get_encode_length(length);

    char *result = wuk::m_alloc<char *>(result_length + 1);
    if(!result) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Base64::encode",
            "Failed to allocate memory for result.");
    }

    for(; dst_index < result_length; src_index += 3, dst_index += 4) {
        result[dst_index]   = b64en_table[buffer[src_index] >> 2];
        result[dst_index+1] = b64en_table[((buffer[src_index] & 0x3) << 4) | (buffer[src_index+1] >> 4)];
        result[dst_index+2] = b64en_table[((buffer[src_index+1] & 0xf) << 2) | (buffer[src_index+2] >> 6)];
        result[dst_index+3] = b64en_table[buffer[src_index+2] & 0x3f];
    }

    // 此处的switch不需要break语句！
    switch(length % 3) {
        case 1:
            result[dst_index - 2] = BASE64PAD;
            [[fallthrough]]; // 感到困惑请参考：https://zh.cppreference.com/w/cpp/language/attributes
        case 2:
            result[dst_index - 1] = BASE64PAD;
    }

    result[result_length] = 0x00;
    length = result_length;

    return result;
}

/*
* The original implementation of this function comes from
* lines 387 to 522 in [Python](https://www.python.org/downloads/release/python-3124/)
* code file `Modules/binascii.c`.
* 
* 由于原始代码的实现是为了针对Python使用的，其中使用了一些针对Python的调整。
* 所以这个函数，目前的情况是最优解了，尽管它在Base64编码串被污染的情况下会占用比实际二进制数据更多的内存空间。
* 如果要优化这个问题，三个方法：
* 1. 使用malloc和realloc来管理内存，但是这样的话，必须调整所有代码的内存管理方式以达到统一。
* 2. 在一开始将有效的Base64编码字符从被污染的Base64编码串中提取出来，然后进行解码，这个方法最好使用std::vector。
* 3. https://stackoverflow.com/a/78655704/21376217
*/
wByte *wuk::Base64::decode(const char *buffer, wSize &length)
{
    const wByte *ascii_data = reinterpret_cast<const wByte *>(buffer);
    const wSize  ascii_len  = length;
    bool  padding_started   = 0;

    wSize  bin_len  = this->get_decode_length(ascii_len);
    wByte *bin_data = wuk::m_alloc<wByte *>(bin_len + 1);
    if(!bin_data) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Base64::decode",
            "Failed to allocate memory for bin_data.");
    }
    wByte *bin_data_start = bin_data;

    wByte leftchar = 0; // 定义一个变量来存储上一次迭代中剩余的字符位
    wU32  quad_pos = 0; // 定义一个变量来跟踪当前处理到Base64编码块中的哪个位置（0到3）
    wU32  pads     = 0; // 定义一个变量来计数填充字符的数量
    wByte this_ch;      // 用于储存单个传入的已编码字符

    if(strict_mode && (ascii_len > 0) && (*ascii_data == BASE64PAD)) {
        wuk::m_free(bin_data_start);
        throw wuk::Exception(wuk::Error::ERR, "wuk::Base64::decode",
            "Leading padding not allowed.");
    }

    for(wSize i = 0; i < ascii_len; ++i) {
        this_ch = ascii_data[i];

        if(this_ch == BASE64PAD) {
            padding_started = true;

            if(strict_mode && (!quad_pos)) {
                wuk::m_free(bin_data_start);
                throw wuk::Exception(wuk::Error::ERR, "wuk::Base64::decode",
                    "Excess padding not allowed.");
            }

            if((quad_pos >= 2) && (quad_pos + (++pads) >= 4)) {
                /**
                 * pad序列意味着我们不应该解析更多的输入。在这一点上，我们已经解释了来自quad的数据。
                 * 在严格模式下，如果填充后有多余的数据，则会引发错误。
                 */
                if(strict_mode && ((i + 1) < ascii_len)) {
                    wuk::m_free(bin_data_start);
                    throw wuk::Exception(wuk::Error::ERR, "wuk::Base64::decode",
                        "Excess data after padding.");
                }

                goto done;
            }

            continue;
        }

        this_ch = b64de_table[this_ch];
        if(this_ch == 255) {
            if(strict_mode) {
                wuk::m_free(bin_data_start);
                throw wuk::Exception(wuk::Error::ERR, "wuk::Base64::decode",
                    "Only base64 data is allowed.");
            }
            continue;
        }

        if(strict_mode && padding_started) {
            wuk::m_free(bin_data_start);
            throw wuk::Exception(wuk::Error::ERR, "wuk::Base64::decode",
                "Discontinuous padding not allowed.");
        }

        pads = 0;

        switch(quad_pos) {
        case 0:
            quad_pos = 1;
            leftchar = this_ch;
            break;
        case 1:
            quad_pos = 2;
            *bin_data++ = (leftchar << 2) | (this_ch >> 4);
            leftchar = this_ch & 0xf;
            break;
        case 2:
            quad_pos = 3;
            *bin_data++ = (leftchar << 4) | (this_ch >> 2);
            leftchar = this_ch & 0x3;
            break;
        case 3:
            quad_pos = 0;
            *bin_data++ = (leftchar << 6) | (this_ch);
            leftchar = 0;
            break;
        }
    }

    if(quad_pos) {
        if(quad_pos == 1) {
            wSize val = (bin_data - bin_data_start) / 3 * 4 + 1;
            wuk::m_free(bin_data_start);
#           ifndef WUK_STD_CPP_20
            char err_msg[256]{};
            snprintf(err_msg, sizeof(err_msg),
                    "Invalid base64-encoded string: "
                    "number of data characters (%zd) cannot be 1 more "
                    "than a multiple of 4", val);
#           else
            std::string _msg = std::format(
                    "Invalid base64-encoded string: "
                    "number of data characters ({0}) cannot be 1 more "
                    "than a multiple of 4", val);
            const char *err_msg = _msg.c_str();
#           endif
            throw wuk::Exception(wuk::Error::ERR, "wuk::Base64::decode",
                                err_msg);
        } else {
            throw wuk::Exception(wuk::Error::ERR, "wuk::Base64::decode",
                "Incorrect padding.");
        }
    }

done:
    length = bin_data - bin_data_start;

    wByte *result_bin = wuk::m_realloc<wByte *>(bin_data_start, length + 1);
    if (!result_bin) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::Base64::decode",
            "Failed to allocate memory for result_bin.");
    }

    result_bin[length] = 0x0;
    return result_bin;
}

std::string wuk::Base64::encode(std::string _buffer)
{
    if(_buffer.empty()) {
        return std::string();
    }

    wByte *buffer = reinterpret_cast<wByte *>(const_cast<char *>(_buffer.data()));
    wSize length = _buffer.size();
    char *result = this->encode(buffer, length);

    std::string _result{result, length};
    wuk::m_free(result);

    return _result;
}

std::string wuk::Base64::decode(std::string _buffer)
{
    if(_buffer.empty()) {
        return std::string();
    }

    const char *buffer = _buffer.c_str();
    wSize length = _buffer.size();
    wByte *result = this->decode(buffer, length);

    std::string _result{reinterpret_cast<char *>(result), length};
    wuk::m_free(result);

    return _result;
}

wuk::Buffer wuk::Base64::encode(wuk::Buffer _buffer)
{
    if (_buffer.is_empty()) {
        return wuk::Buffer{};
    }

    wByte *buffer = const_cast<wByte *>(_buffer.get_data());
    wSize length = _buffer.get_length();
    char *result = this->encode(buffer, length);

    wuk::Buffer _result{reinterpret_cast<wByte *>(result), length};
    wuk::m_free(result);

    return _result;
}

wuk::Buffer wuk::Base64::decode(wuk::Buffer _buffer)
{
    if (_buffer.is_empty()) {
        return wuk::Buffer{};
    }

    const char *buffer = _buffer.get_cstr();
    wSize length = _buffer.get_length();
    wByte *result = this->decode(buffer, length);

    wuk::Buffer _result{reinterpret_cast<wByte *>(result), length};
    wuk::m_free(result);

    return _result;
}
