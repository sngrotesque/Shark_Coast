#include <WukBuffer.hh>

//////////////////////////////////////////////////////////////////////
/**
 * @brief 用于增加可用内存大小
 * @authors SN-Grotesque
 * @note 不需要在函数外部重新分配this->data_size大小，此方法会自动完成
 * @param length 需要增加的长度（非总长度），比如要加16字节，就传入16。
 * @return 无
 */
void wuk::Buffer::expand_memory(wSize length)
{
    if (!this->data) {
        // 如果指针还未使用
        this->data = static_cast<wByte *>(malloc(length));
        if (!this->data) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::expand_memory",
                "Failed to allocate memory for this->data.");
        }
        this->data_offset = this->data;
    } else {
        // 如果指针已使用
        wSize offset_value = this->data_offset - this->data;
        wByte *tmp_ptr = static_cast<wByte *>(realloc(this->data, this->data_size + length));
        if (!tmp_ptr) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::expand_memory",
                "Expanding memory size failed.");
        }
        this->data = tmp_ptr;
        this->data_offset = this->data + offset_value;
    }

    this->data_size += length;
}

/**
 * @brief 用于减少可用内存大小
 * @authors SN-Grotesque
 * @note 不需要在函数外部重新分配this->data_size大小，此方法会自动完成
 * @param length 需要减少的长度（非总长度），比如要减16字节，就传入16。
 * @return 无
 */
void wuk::Buffer::shrink_memory(wSize length)
{
    
}

/**
 * @brief 检查当前已申请的内存空间是否足够
 * @authors SN-Grotesque
 * @note 无
 * @param length 新数据的长度
 * @return 如果足够就返回True，否则False
 */
bool wuk::Buffer::is_memory_sufficient(wSize length)
{
    return (this->data_len + length) > this->data_size;
}

//////////////////////////////////////////////////////////////////////
wuk::Buffer::Buffer()
: data(nullptr), data_offset(nullptr), data_len(), data_size()
{
    
}

wuk::Buffer::Buffer(const wuk::Buffer &other)
: data(nullptr), data_offset(nullptr), data_len(other.data_len), data_size(other.data_size)
{
    wSize offset_val = other.data_offset - other.data;

    this->data = static_cast<wByte *>(malloc(this->data_size));
    if (!this->data) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::Buffer",
            "Failed to allocate memory for this->data.");
    }
    memcpy(this->data, other.data, other.data_len);

    this->data_offset = this->data + offset_val;
}

wuk::Buffer::Buffer(wuk::Buffer &&other)
: data(nullptr), data_offset(nullptr), data_len(other.data_len), data_size(other.data_size)
{
    this->data = other.data;
    this->data_offset = other.data_offset;

    other.data = nullptr;
    other.data_offset = nullptr;
}

wuk::Buffer &wuk::Buffer::operator=(const wuk::Buffer &other)
{
    if(this == &other) {
        return *this;
    }
    free(this->data);
    this->data_len = other.data_len;
    this->data_size = other.data_size;

    this->data = static_cast<wByte *>(malloc(this->data_size));
    if (!this->data) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::operator=",
            "Failed to allocate memory for this->data.");
    }
    memcpy(this->data, other.data, other.data_len);
    this->data_offset = this->data + (other.data_offset - other.data);

    return *this;
}

wuk::Buffer &wuk::Buffer::operator=(wuk::Buffer &&other) noexcept
{
    if (this == &other) {
        return *this;
    }
    free(this->data);
    this->data = other.data;
    this->data_offset = other.data_offset;
    this->data_len = other.data_len;
    this->data_size = other.data_size;

    other.data = nullptr;
    other.data_offset = nullptr;
    other.data_len = 0;
    other.data_size = 0;

    return *this;
}

wuk::Buffer::Buffer(const std::string &content)
: Buffer(reinterpret_cast<const wByte *>(content.c_str()), content.size())
{
    
}

wuk::Buffer &wuk::Buffer::operator=(const std::string &other_string)
{
    free(this->data);

    this->data_len = other_string.length();
    this->data_size = other_string.size();

    this->data = static_cast<wByte *>(malloc(this->data_size));
    if (!this->data) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::operator=",
            "Failed to allocate memory for this->data.");
    }
    memcpy(this->data, other_string.c_str(), this->data_len);

    this->data_offset = this->data + this->data_len;
    return *this;
}

wuk::Buffer &wuk::Buffer::operator=(std::string &&other_string)
{
    free(this->data);

    this->data_len = other_string.length();
    this->data_size = other_string.size();

    this->data = static_cast<wByte *>(malloc(this->data_size));
    if (!this->data) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::operator=",
            "Failed to allocate memory for this->data.");
    }
    memcpy(this->data, other_string.c_str(), this->data_len);
    this->data_offset = this->data + this->data_len;
    other_string.clear();
    return *this;
}

wuk::Buffer::Buffer(const wByte *content, wSize length)
: data(nullptr), data_offset(nullptr), data_len(length), data_size(length)
{
    this->data = static_cast<wByte *>(malloc(this->data_len));
    if (!this->data) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::Buffer",
            "Failed to allocate memory for this->data.");
    }
    memcpy(this->data, content, length);

    this->data_offset = this->data + length;
}

wuk::Buffer::Buffer(wSize memory_size)
: data(nullptr), data_offset(nullptr), data_len(), data_size(memory_size)
{
    this->data = static_cast<wByte *>(malloc(this->data_size));
    if (!this->data) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::Buffer",
            "Failed to allocate memory for this->data.");
    }

    wuk::memory_zero(this->data, this->data_size);

    this->data_offset = this->data;
}

wuk::Buffer::~Buffer()
{
    free(this->data);
}

//////////////////////////////////////////////////////////////////////
bool wuk::Buffer::is_empty()
{
    return (!this->data_len || !this->data);
}

/**
 * @brief 在需要写入指定长度的大小的内容且同时需要指针的情况下调用此方法
 * @authors SN-Grotesque
 * @note 避免临时开辟临时缓冲区造成资源浪费。
 *       recv(fd, buffer.write(2048), 2048, 0);
 * @param length 将要写入的数据内容的长度
 * @return 指向内部数据内容结尾的指针
 */
wByte *wuk::Buffer::append_write(wSize length)
{
    if(this->is_memory_sufficient(length)) {
        this->expand_memory(length);
    }

    this->data_len += length;
    this->data_offset += length;

    return this->data_offset - length;
}

void wuk::Buffer::append(const wByte *content, wSize length)
{
    if(!content) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::Buffer::append",
            "content in nullptr.");
    }

    if(this->is_memory_sufficient(length)) {
        this->expand_memory(length);
    }

    memcpy(this->data_offset, content, length);

    this->data_offset += length;
    this->data_len += length;
}

void wuk::Buffer::append(const std::string content)
{
    this->append(reinterpret_cast<const wByte *>(content.c_str()),
                content.size());
}

void wuk::Buffer::shrink_to_fit()
{
    
}

//////////////////////////////////////////////////////////////////////
const wByte *wuk::Buffer::get_data() const
{
    return this->data;
}

const char *wuk::Buffer::get_cStr()
{
    return reinterpret_cast<const char *>(this->data);
}

wSize wuk::Buffer::get_length()
{
    return this->data_len;
}

wSize wuk::Buffer::get_size()
{
    return this->data_size;
}
