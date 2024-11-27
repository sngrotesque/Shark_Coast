/**
 * 此模块的开发注意事项：
 * 1. 如果涉及直接的数据传入，如`Buffer::Buffer(wByte *content, wSize length)`这个构造函数，那么
 *    必须要将`data_offset`指针置于`data`之后，偏移长度为传入的数据的长度。
 * 2. 在任何时候，`data_offset`指向地址都不应该比`data`小，但是是否有必要增加一个判断，还有待商榷。
 */
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_BUFFER
#define WUK_CPP_BUFFER
#include <config/WukException.hh>
#include <WukMemory.hh>

namespace wuk {
    class LIBWUK_API Buffer {
    private:
        wByte *data;
        wByte *data_offset; // 在当前已申请空间的情况下写入数据时使用（指向数据末端用于追加写入）

        wSize data_len;     // 代表实际长度
        wSize data_size;    // 代表已申请的内存空间长度

        // 用于增加可用内存大小
        void expand_memory(wSize length);
        // 用于减少可用内存大小
        void shrink_memory(wSize length); // 未实现
        // 检查当前已申请的内存空间是否足够
        bool is_memory_sufficient(wSize length);

    public:
        // 构造函数
        Buffer();
        Buffer(const wuk::Buffer &other);
        Buffer(wuk::Buffer &&other);

        // 拷贝赋值运算符
        wuk::Buffer &operator=(const wuk::Buffer &other);
        // 移动赋值运算符
        wuk::Buffer &operator=(wuk::Buffer &&other) noexcept;

        // 兼容std::string
        Buffer(const std::string &content);
        wuk::Buffer &operator=(const std::string &other_string);
        wuk::Buffer &operator=(std::string &&other_string);

        Buffer operator+(const Buffer &other);
        Buffer &operator+=(const Buffer other);

        // 给予数据的构造函数
        Buffer(const wByte *content, wSize length);
        // 申请指定大小内存空间备用的构造函数
        Buffer(wSize memory_size);

        // 析构函数
        ~Buffer();

        // 判断是否为空
        bool is_empty();
        // 在需要写入指定长度的大小的内容且同时需要指针的情况下调用此方法
        wByte *append_write(wSize length);
        // 追加写入，可用于直接追加和已申请空间的情况下
        void append(const wByte *content, wSize length);
        void append(const std::string content);

        // 这个方法不应该被实现，留在这是为了提醒以后使用运算符重载将其实现
        // void append(const wuk::Buffer buffer);
        // 尝试性实现了+和+=，出现了一个奇怪的BUG，打算后续重新实现了

        // 将占用的内存空间与实际使用的内存空间保持同步（防止无意义的内存占用）
        void shrink_to_fit(); // 未实现

        // 属性
        const wByte *get_data() const;
        const char *get_cStr();
        wSize get_length();
        wSize get_size();
    };
}

#endif
#endif
