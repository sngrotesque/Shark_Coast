#include <crypto/WukFEA_CTR.cc>

#include <WukBuffer.cc>
#include <WukRandom.cc>
#include <WukTime.cc>

#include <openssl/evp.h>

#include <fstream>

class FCipher {
private:
    wuk::crypto::FEA fea;
    wuk::Random rnd;

public:
    FCipher() = default;

    void init_ctx(const char *password)
    {
        wByte KeyWithIV[48]{};
        wByte salt[16]{};
        wByte _ctr[12]{};

        rnd.urandom(salt, sizeof(salt));
        rnd.urandom(_ctr, 12);

        wuk::crypto::Counter counter{_ctr, sizeof(_ctr), 7};

        PKCS5_PBKDF2_HMAC(password, strlen(password), salt, sizeof(salt),
            11246, EVP_sha256(), sizeof(KeyWithIV), KeyWithIV);

        this->fea = wuk::crypto::FEA{KeyWithIV, KeyWithIV + 32, counter};
    }

    void encrypt(const char *in, const char *out)
    {
        std::fstream f_i{in, std::ios::in | std::ios::binary | std::ios::app};
        std::fstream f_o{out, std::ios::out | std::ios::binary};
        wSize file_length{};

        if (!f_i.is_open() || !f_o.is_open()) {
            throw wuk::Exception(wuk::Error::ERR, "FCipher::encrypt",
                "failed to file open.");
        }

        file_length = f_i.tellg(); // 获取文件长度
        if (!file_length) {
            f_i.close();
            f_o.close();
            throw wuk::Exception(wuk::Error::ERR, "FCipher::encrypt",
                "file content is null.");
        }

        f_i.seekg(0); // 复位文件指针至文件头部

        wByte *buffer = (wByte *)malloc(file_length);
        if (!buffer) {
            throw wuk::Exception(wuk::Error::MEMORY, "FCipher::encrypt",
                "");
        }

        f_i.read(reinterpret_cast<char *>(buffer), file_length);

        this->fea.encrypt(buffer, file_length, wuk::crypto::mode::CTR);


    }

    void set_in_path(const char *path);
    void set_out_path(const char *path);


};

int main()
{
    

    return 0;
}
