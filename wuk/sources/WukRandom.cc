#include <WukRandom.hh>

constexpr wByte BIT_BYTE_SWAP(wByte i)
{
    return ((i & 0xf) << 4) ^ (i >> 4);
}

wuk::Random::Random()
{
    this->seed();
}

void wuk::Random::seed()
{
    wuk::Time timer;
    srand((unsigned)(timer.time() * 1.e6));
}

wSize wuk::Random::rand()
{
    wSize n = 0;
    wSize num[32]{};
    wU32 count;
    wU32 i;

    for(i = 0; i < 32; ++i) {
        num[i] = ::rand() ^ (::rand() ^ (::rand() & ::rand()));
        n = n + (n ^ num[i]);
    }

    n = n + BIT_BYTE_SWAP(::rand() & 0x0f);

    for(count = 0; count < 32; ++count) {
        for(i = 0; i < 32; ++i) {
            num[i] = ::rand() ^ (::rand() ^ (::rand() & ::rand()));
        }
        n = (n + (n ^ num[count])) ^ count;
        n = (n + ::rand()) ^ BIT_BYTE_SWAP((count + (::rand() ^
                                        (n - num[count]))) & 0xff);
    }

    return n;
}

wSize wuk::Random::randint(wSize min, wSize max)
{
    max = (!(max-min+1))?(max-1):max;
    return this->rand() % (max - min + 1) + min;
}

void wuk::Random::urandom(wByte *buf, wSize size)
{
    if(!buf || !size) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::Random::urandom",
                                        "buf or size is NULL.");
    }

#   if defined(WUK_PLATFORM_WINOS)
    HCRYPTPROV hProv;
    if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        throw wuk::Exception(static_cast<wuk::Error>(GetLastError()), "wuk::Random::urandom",
            "CryptAcquireContext function returned an error code when called.");
    }
    if(!CryptGenRandom(hProv, size, buf)) {
        throw wuk::Exception(static_cast<wuk::Error>(GetLastError()), "wuk::Random::urandom",
            "CryptGenRandom function returned an error code when called.");
    }
    if(!CryptReleaseContext(hProv, 0)) {
        throw wuk::Exception(static_cast<wuk::Error>(GetLastError()), "wuk::Random::urandom",
            "CryptReleaseContext function returned an error code when called.");
    }
#   elif defined(WUK_PLATFORM_LINUX)
    if(getrandom(buf, size, GRND_RANDOM) == EOF) {
        throw wuk::Exception(static_cast<wuk::Error>(errno), "wuk::Random::urandom",
            "getrandom function returned an error code when called.");
    }
#   endif
}

std::string wuk::Random::urandom(wU32 size)
{
    if(!size) {
        return std::string();
    }

    wByte *buffer = wuk::m_alloc<wByte *>(size);
    if(!buffer) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Random::urandom",
            "Failed to allocate memory for buffer.");
    }

    this->urandom(buffer, size);

    std::string result(reinterpret_cast<char *>(buffer), size);
    wuk::m_free(buffer);

    return result;
}
