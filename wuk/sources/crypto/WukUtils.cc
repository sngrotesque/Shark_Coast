#include <crypto/WukUtils.hh>

wuk::crypto::Counter::Counter()
: counter()
{

}

wuk::crypto::Counter::Counter(const wByte *nonce, wU32 size, wSize begin)
: counter()
{
    if (size >= sizeof(this->counter)) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::crypto::Counter::Counter",
                            "Nonce is too long.");
    }
    memcpy(this->counter, nonce, size);

    this->counter[8]  |= (begin >> 56) & 0xff;
    this->counter[9]  |= (begin >> 48) & 0xff;
    this->counter[10] |= (begin >> 40) & 0xff;
    this->counter[11] |= (begin >> 32) & 0xff;
    this->counter[12] |= (begin >> 24) & 0xff;
    this->counter[13] |= (begin >> 16) & 0xff;
    this->counter[14] |= (begin >> 8)  & 0xff;
    this->counter[15] |= begin         & 0xff;
}

wuk::crypto::Counter::Counter(const char *nonce, wU32 size, wSize begin)
// Counter -> Counter(wByte *nonce, wU32 size, wSize begin)
: Counter(reinterpret_cast<const wByte *>(nonce), size, begin)
{

}

wuk::crypto::Counter::Counter(std::string nonce, wSize begin)
// Counter -> Counter(const char *nonce, wU32 size, wSize begin)
: Counter(nonce.c_str(), nonce.size(), begin)
{

}

wuk::crypto::Counter::Counter(const Counter &other) noexcept
{
    memcpy(this->counter, other.counter, sizeof(this->counter));
}

wuk::crypto::Counter::Counter(Counter &&other)
{
    memcpy(this->counter, other.counter, sizeof(other.counter));

    wuk::memory_zero(other.counter, sizeof(other.counter));
}

wuk::crypto::Counter &wuk::crypto::Counter::operator=(const wuk::crypto::Counter &other)
{
    if (this == &other) {
        return *this;
    }

    memcpy(this->counter, other.counter, sizeof(other.counter));

    return *this;
}

wuk::crypto::Counter &wuk::crypto::Counter::operator=(wuk::crypto::Counter &&other)
{
    if (this == &other) {
        return *this;
    }

    memcpy(this->counter, other.counter, sizeof(other.counter));
    wuk::memory_zero(other.counter, sizeof(other.counter));

    return *this;
}

wByte *wuk::crypto::Counter::get() noexcept
{
    return this->counter;
}

void wuk::crypto::Counter::clean() noexcept
{
    wuk::memory_zero(this->counter, sizeof(counter));
}
