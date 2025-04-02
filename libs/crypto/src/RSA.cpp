#include <yabil/bigint/BigInt.h>
#include <yabil/crypto/RSA.h>
#include <yabil/crypto/RandomEngine.h>
#include <yabil/math/Math.h>

#include <algorithm>
#include <bit>
#include <cstring>

#include <yabil/bigint/BigIntBase.h>

namespace yabil::crypto::rsa
{

std::pair<PublicKey, PrivateKey> generate_keys(random::RandomEngine &random_engine, bigint::BigInt p, bigint::BigInt q)
{
    const auto n = p * q;
    const auto phi = (--p) * (--q);

    yabil::bigint::BigInt e{65537};
    while (e >= phi || yabil::math::gcd(e, phi) != yabil::bigint::BigInt(1))
    {
        const auto max_bits = phi.byte_size() * 8 - std::countl_zero(phi.raw_data().back());
        const auto bits_to_generate =
            max_bits < 32
                ? random_engine.random_digit(
                      std::uniform_int_distribution<bigint::bigint_base_t>(max_bits / 2, max_bits))
                : 16 + random_engine.random_digit(std::uniform_int_distribution<bigint::bigint_base_t>(0, 16));
        e = random_engine.random_prime(static_cast<std::size_t>(bits_to_generate));
    }
    const auto d = yabil::math::mod_inverse(e, phi);
    return {{e, n}, {d, n}};
}

yabil::bigint::BigInt encrypt(uint8_t byte, const PublicKey &pub_key)
{
    return yabil::math::pow(yabil::bigint::BigInt(byte), pub_key.e, pub_key.n);
}

yabil::bigint::BigInt decrypt(const yabil::bigint::BigInt &encrypted, const PrivateKey &private_key)
{
    return yabil::math::pow(encrypted, private_key.d, private_key.n);
}

EncryptionStreamWrapper::EncryptionStreamWrapper(std::ostream &out, const PublicKey &pub_key)
    : out(out),
      pub_key(pub_key)
{
}
EncryptionStreamWrapper::EncryptionStreamWrapper(std::ostream &out, PublicKey &&pub_key) : out(out), pub_key(pub_key) {}

DecryptionStreamWrapper::DecryptionStreamWrapper(std::istream &in, const PrivateKey &private_key)
    : in(in),
      private_key(private_key)
{
}

DecryptionStreamWrapper::DecryptionStreamWrapper(std::istream &in, PrivateKey &&private_key)
    : in(in),
      private_key(private_key)
{
}

std::string DecryptionStreamWrapper::read_all()
{
    std::string result;
    while (!in.eof())
    {
        result.push_back(read_single_encoded_item());
    }
    result.erase(std::find(result.begin(), result.end(), '\0'), result.end());
    return result;
}

char DecryptionStreamWrapper::read_single_encoded_item()
{
    uint64_t encrypted_item_size = 0;
    in.read(reinterpret_cast<char *>(&encrypted_item_size), sizeof(uint64_t));
    std::vector<yabil::bigint::bigint_base_t> raw_data_buffer(encrypted_item_size);

    in.read(reinterpret_cast<char *>(raw_data_buffer.data()), static_cast<std::streamsize>(encrypted_item_size));
    const auto decrypted = decrypt(yabil::bigint::BigInt(std::move(raw_data_buffer)), private_key);
    return static_cast<char>(decrypted.to<int64_t>());
}

}  // namespace yabil::crypto::rsa
