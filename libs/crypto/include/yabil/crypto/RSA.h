#pragma once

#include <yabil/bigint/BigInt.h>

#include <algorithm>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

/// @brief RSA cryptosystem functionalities
namespace yabil::crypto::rsa
{

/// @brief RSA Public key definition
struct PublicKey
{
    yabil::bigint::BigInt e;
    yabil::bigint::BigInt n;
};

/// @brief RSA Private key definition
struct PrivateKey
{
    yabil::bigint::BigInt d;
    yabil::bigint::BigInt n;
};

/// @brief Generate RSA public and private keys.
/// @param bits_for_key Number of bits for generated key n (n = pq where p and q are prime numbers)
/// @return \p std::pair of \p PublicKey and \p PrivateKey
std::pair<PublicKey, PrivateKey> generate_keys(unsigned bits_for_key = 1024);

/// @brief Encrypt single byte using RSA public key.
/// @param byte Byte to encrypt
/// @param pub_key RSA Public Key
/// @return \p BigInt result of encryption
yabil::bigint::BigInt encrypt(uint8_t byte, const PublicKey &pub_key);

/// @brief Decrypt single item using RSA private key.
/// @param encrypted Encrypted item to decrypt
/// @param private_key RSA private key
/// @return \p BigInt result of decryption
yabil::bigint::BigInt decrypt(const yabil::bigint::BigInt &encrypted, const PrivateKey &private_key);

class EncryptionStreamWrapper
{
private:
    std::ostream &out;
    const PublicKey &pub_key;

public:
    EncryptionStreamWrapper(std::ostream &out, const PublicKey &pub_key);
    EncryptionStreamWrapper(std::ostream &out, PublicKey &&pub_key);

    template <typename T>
    EncryptionStreamWrapper &operator<<(T data)
    {
        std::ostringstream converted_data;
        converted_data << data;
        for (char chr : converted_data.str())
        {
            const auto encrypted = encrypt(chr, pub_key);
            const auto data_size = encrypted.byte_size();

            out.write(reinterpret_cast<const char *>(&data_size), sizeof(data_size));
            out.write(reinterpret_cast<const char *>(encrypted.raw_data().data()),
                      static_cast<std::streamsize>(data_size));
        }
        return *this;
    }
};

class DecryptionStreamWrapper
{
private:
    std::istream &in;
    const PrivateKey &private_key;

public:
    DecryptionStreamWrapper(std::istream &in, const PrivateKey &private_key);
    DecryptionStreamWrapper(std::istream &in, PrivateKey &&private_key);

    template <typename T>
    DecryptionStreamWrapper &operator>>(T &data)
    {
        std::string result;
        while (!in.eof())
        {
            char item = read_single_encoded_item();
            if (item == ' ') break;
            result.push_back(item);
        }
        std::istringstream in_stream(std::move(result));
        in_stream >> data;
        return *this;
    }

    std::string read_all();

private:
    char read_single_encoded_item();
};

}  // namespace yabil::crypto::rsa
